// Injection.cpp : 定义微信服务管控。
//
#include "stdafx.h"
#include "WechatMultiOpen.h"
#include "WechatService.h"
#include "Config.h"
#include "Common.h"
#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <TlHelp32.h>
#include "WechatRobot.h"

INT_PTR StartWechat()
{
	char dllPath[MAX_PATH] = {0};
	GetDllPath(dllPath, MAX_PATH);

	if (strlen(dllPath) <= 0) {
		MessageBox(NULL, L"DLL文件不存在！", L"温馨提示：", 0);
		return FALSE;
	}
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Tencent\\WeChat", 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS) {
		MessageBox(NULL, L"注册表打开失败！", L"温馨提示：", 0);
		return FALSE;
	}
	CHAR szProductType[MAX_PATH] = {};
	DWORD dwBufLen = MAX_PATH;
	if (RegQueryValueEx(hKey, L"InstallPath", NULL, NULL, (LPBYTE)szProductType, &dwBufLen) != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		MessageBox(NULL, L"注册表查询失败！", L"温馨提示：", 0);
		return FALSE;
	}

	RegCloseKey(hKey);
	wchar_t weChatexe[MAX_PATH] = {0};
	swprintf_s(weChatexe, L"%s\\%s", (wchar_t*)szProductType, _T(WECHAT_PROCESS_NAME));

	if (_access(WcharToChar(weChatexe), 0) == -1)
	{
		MessageBox(NULL, L"WeChat.exe不存在注册表安装目录！", L"温馨提示：", 0);
		return FALSE;
	}
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	// 启动微信
	CreateProcess(weChatexe, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);
	
	ResumeThread(pi.hThread);

	// 检查进程是否正常并且最多等待1秒
	WaitForSingleObject(pi.hProcess, 1000);

	// 多开解锁
	PatchWeChat();
	// 开始注入
	InjectDll(pi.dwProcessId);
	
	// 返回进程ID
	return pi.dwProcessId;
}

/**
 * 结束所有微信进程
 * @param
 * @return BOOL
 */
INT_PTR CloseAllWeChat()
{
	HANDLE ProcesssAll = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 proessInfo = { 0 };
	proessInfo.dwSize = sizeof(PROCESSENTRY32);
	do
	{
		if (wcscmp(_T(WECHAT_PROCESS_NAME), proessInfo.szExeFile) == 0) {
			CloseProcess(proessInfo.th32ProcessID);  // 关闭进程
		}
	} while (Process32Next(ProcesssAll, &proessInfo));

	return TRUE;
}

/**
 * 结束指定进程
 * @param
 * @return BOOL
 */
INT_PTR CloseProcess(int processId)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processId);
	if (hProcess != NULL) {
		TerminateProcess(hProcess, 0);
	}

	return TRUE;
}
