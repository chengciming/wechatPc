#include "stdafx.h"
#include <TlHelp32.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi")
#include "Common.h"
#include "WechatMultiOpen.h"
#include <stdio.h>


typedef NTSTATUS(NTAPI *NTQUERYOBJECT)(
	_In_opt_   HANDLE Handle,
	_In_       OBJECT_INFORMATION_CLASS ObjectInformationClass,
	_Out_opt_  PVOID ObjectInformation,
	_In_       ULONG ObjectInformationLength,
	_Out_opt_  PULONG ReturnLength
	);


typedef NTSTATUS
(NTAPI *ZWQUERYSYSTEMINFORMATION)(
	IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
	OUT PVOID SystemInformation,
	IN ULONG SystemInformationLength,
	OUT PULONG ReturnLength OPTIONAL
	);
ZWQUERYSYSTEMINFORMATION ZwQuerySystemInformation = (ZWQUERYSYSTEMINFORMATION)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "ZwQuerySystemInformation");
NTQUERYOBJECT    NtQueryObject = (NTQUERYOBJECT)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtQueryObject");


/*
进程提权
*/
BOOL ElevatePrivileges()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	tkp.PrivilegeCount = 1;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return FALSE;
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
	{
		return FALSE;
	}

	return TRUE;
}


/*
复制句柄
*/
HANDLE DuplicateHandleEx(DWORD pid, HANDLE h, DWORD flags)
{
	HANDLE hHandle = NULL;

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (hProc)
	{
		if (!DuplicateHandle(hProc,
			(HANDLE)h, GetCurrentProcess(),
			&hHandle, 0, FALSE, /*DUPLICATE_SAME_ACCESS*/flags))
		{
			hHandle = NULL;
		}
	}

	CloseHandle(hProc);
	return hHandle;
}


/*
获取PID
*/
int GetProcIds(CONST CHAR *Name, DWORD* Pids)
{
	PROCESSENTRY32 pe32 = { sizeof(pe32) };
	int num = 0;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap)
	{
		if (Process32First(hSnap, &pe32))
		{
			do {
				if (!wcscmp(CharToWChar((char*)Name), pe32.szExeFile))
				{
					if (Pids)
					{
						Pids[num++] = pe32.th32ProcessID;
					}
				}
			} while (Process32Next(hSnap, &pe32));
		}
		CloseHandle(hSnap);
	}

	return num;
}


/*
是否是目标PID
*/
BOOL IsTargetPid(DWORD Pid, DWORD* Pids, int num)
{
	for (int i = 0; i < num; i++)
	{
		if (Pid == Pids[i])
		{
			return TRUE;
		}
	}
	return FALSE;
}


/*
修改微信
*/
int PatchWeChat()
{
	DWORD dwSize = 0;
	PVOID pbuffer = NULL;
	NTSTATUS Status;
	DWORD nIndex = 0;
	DWORD dwFlags = 0;
	char szName[512] = { 0 };

	ElevatePrivileges();

	DWORD Pids[100] = { 0 };

	DWORD Num = GetProcIds("WeChat.exe", Pids);
	if (Num == 0) {
		return FALSE;
	}

	if (!ZwQuerySystemInformation) {
		return FALSE;
	}

	pbuffer = VirtualAlloc(NULL, 0x1000, MEM_COMMIT, PAGE_READWRITE);

	if (!pbuffer) {
		if (NULL != pbuffer) {
			VirtualFree(pbuffer, 0, MEM_RELEASE);
		}
		return FALSE;
	}

	Status = ZwQuerySystemInformation(SystemHandleInformation, pbuffer, 0x1000, &dwSize);

	if (!NT_SUCCESS(Status))
	{
		if (STATUS_INFO_LENGTH_MISMATCH != Status) {
			if (NULL != pbuffer) {
				VirtualFree(pbuffer, 0, MEM_RELEASE);
			}
			return FALSE;
		} else {
			// 这里大家可以保证程序的正确性使用循环分配稍好
			if (NULL != pbuffer) {
				VirtualFree(pbuffer, 0, MEM_RELEASE);
			}

			if (dwSize * 2 > 0x4000000) {  // MAXSIZE
				if (NULL != pbuffer) {
					VirtualFree(pbuffer, 0, MEM_RELEASE);
				}
				return FALSE;
			}

			pbuffer = VirtualAlloc(NULL, dwSize * 2, MEM_COMMIT, PAGE_READWRITE);

			if (!pbuffer) {
				if (NULL != pbuffer) {
					VirtualFree(pbuffer, 0, MEM_RELEASE);
				}
				return FALSE;
			}

			Status = ZwQuerySystemInformation(SystemHandleInformation, pbuffer, dwSize * 2, NULL);

			if (!NT_SUCCESS(Status)) {
				if (NULL != pbuffer) {
					VirtualFree(pbuffer, 0, MEM_RELEASE);
				}
				return FALSE;
			}
		}
	}

	PSYSTEM_HANDLE_INFORMATION1 pHandleInfo = (PSYSTEM_HANDLE_INFORMATION1)pbuffer;

	for (nIndex = 0; nIndex < pHandleInfo->NumberOfHandles; nIndex++) {
		if (IsTargetPid(pHandleInfo->Handles[nIndex].UniqueProcessId, Pids, Num)) {
			HANDLE hHandle = DuplicateHandleEx(pHandleInfo->Handles[nIndex].UniqueProcessId,
				(HANDLE)pHandleInfo->Handles[nIndex].HandleValue,
				DUPLICATE_SAME_ACCESS
			);
			if (hHandle == NULL) continue;

			Status = NtQueryObject(hHandle, ObjectNameInformation, szName, 512, &dwFlags);

			if (!NT_SUCCESS(Status)) {
				CloseHandle(hHandle);
				continue;
			}

			CHAR Name[1024] = { 0 };

			sprintf_s(Name, "%s", UnicodeToUtf8((wchar_t*)szName));
			
			if (strstr(Name, "_WeChat_App_Instance_Identity_Mutex_Name") != NULL) {
				CloseHandle(hHandle);

				hHandle = DuplicateHandleEx(pHandleInfo->Handles[nIndex].UniqueProcessId,
					(HANDLE)pHandleInfo->Handles[nIndex].HandleValue,
					DUPLICATE_CLOSE_SOURCE
				);

				if (hHandle) {
					CloseHandle(hHandle);
				}
				if (NULL != pbuffer) {
					VirtualFree(pbuffer, 0, MEM_RELEASE);
				}
				return FALSE;
			}

			CloseHandle(hHandle);
		}

	}

	if (NULL != pbuffer)
	{
		VirtualFree(pbuffer, 0, MEM_RELEASE);
	}

	return FALSE;
}