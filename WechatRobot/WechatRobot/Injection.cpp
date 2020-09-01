// Injection.cpp : 定义注入方法。
//
#include "stdafx.h"
#include "Injection.h"
#include "Config.h"
#include "Common.h"
#include <direct.h>
#include <stdio.h>


/*
注入DLL
*/
INT_PTR InjectDll(DWORD pid)
{
	// 获取微信进程的PID
	if(pid == 0) {
		DWORD pid = ProcessNameFindPid(WECHAT_PROCESS_NAME);
		if (pid == 0) {
			return FALSE;
		}
	}
	
	// 检查到已经注入就不要再注入了
	INT_PTR result = CheckInject(pid);
	if (result == TRUE || result == NULL) {
		// 打开进程
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (hProcess == NULL) {
			return FALSE;
		}
		// 获取DLL路径
		CHAR dllPath[MAX_PATH] = { 0 };
		GetDllPath(dllPath, MAX_PATH);

		if (strlen(dllPath) <= 0) {
			MessageBox(NULL, L"DLL文件不存在！", L"温馨提示：", 0);
			return FALSE;
		}
		// 写入内存
		LPVOID dllAddress = writeMemory(hProcess, dllPath);
		if (dllAddress == NULL) {
			return FALSE;
		}
		// 加载DLL
		if (loadLibrary(hProcess, dllAddress) == FALSE) {
			return FALSE;
		}
		CloseHandle(hProcess);
	}
	
	return TRUE;
}

/*
检查是否已经注入
*/
INT_PTR CheckInject(DWORD dwProcessid)
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	// 初始化模块信息结构体
	MODULEENTRY32 me32 = { sizeof(MODULEENTRY32) };
	// 创建模块快照 1 快照类型 2 进程ID
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessid);
	// 如果句柄无效就返回false
	if (hModuleSnap == INVALID_HANDLE_VALUE) {
		return NULL;
	}
	// 通过模块快照句柄获取第一个模块的信息
	if (!Module32First(hModuleSnap, &me32)) {
		//获取失败则关闭句柄
		CloseHandle(hModuleSnap);
		return NULL;
	}
	do {
		if (wcscmp(me32.szModule, _T(DLL_NAME)) == 0) {
			return FALSE;
		}
	} while (Module32Next(hModuleSnap, &me32));
	return TRUE;
}

/*
卸载注入
*/
INT_PTR UnjectDll()
{
	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(hProcess, &pe32) != TRUE) {
		return FALSE;
	}

	HMODULE Kernel32 = GetModuleHandle(L"Kernel32.dll");
	if (Kernel32 == 0) {
		return FALSE;
	}
	int wechatCount = 0;
	do {
		if (wcscmp(pe32.szExeFile, _T(WECHAT_PROCESS_NAME)) != 0) {
			continue;
		}
		DWORD dwPid = pe32.th32ProcessID;

		//遍历模块
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);

		if (hSnap == INVALID_HANDLE_VALUE) {
			continue;
		}
		MODULEENTRY32 ME32 = { 0 };
		ME32.dwSize = sizeof(MODULEENTRY32);
		BOOL isNext = Module32First(hSnap, &ME32);
		BOOL flag = FALSE;
		while (isNext) {
			if (wcscmp(ME32.szModule, _T(DLL_NAME)) == 0) {
				flag = TRUE;
				break;
			}
			isNext = Module32Next(hSnap, &ME32);
		}
		if (flag != TRUE) {
			CloseHandle(hSnap);
			continue;
		}
		//打开目标进程
		HANDLE hPro = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
		if (!hPro) {
			CloseHandle(hSnap);
			continue;
		}
		//获取FreeLibrary函数地址
		FARPROC pFun = GetProcAddress(Kernel32, "FreeLibraryA");
		//创建远程线程执行FreeLibrary
		HANDLE hThread = CreateRemoteThread(hPro, NULL, 0, (LPTHREAD_START_ROUTINE)pFun, ME32.modBaseAddr, 0, NULL);
		if (hThread != NULL) {
			//WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);
		}
		CloseHandle(hPro);
		CloseHandle(hSnap);
	} while (Process32Next(hProcess, &pe32));

	return TRUE;
}


