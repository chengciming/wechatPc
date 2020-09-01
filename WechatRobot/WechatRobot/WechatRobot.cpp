// WechatRobot.cpp : 定义应用程序的入口点。
//
#include "stdafx.h"
#include "Common.h"
#include "WechatRobot.h"
#include "WechatService.h"
#include "WechatMultiOpen.h"
#include "WebSocketClient.h"
#include "WebSocketListen.h"

/*
起始函数 - 主函数
*/
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	HANDLE hObject = ::CreateMutex(NULL, FALSE, L"WeChatRoBot_Hook_Mutex");
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		if (hObject != 0) {
			MessageBox(NULL, L"您已经开启一个了，不要重复开启！", L"温馨提示：", 0);
			CloseHandle(hObject);
		}
		TerminateProcess(GetCurrentProcess(), 0);
		return 0;
	}

	DialogBox(hInstance, MAKEINTRESOURCE(ID_INJECTION), NULL, &Dlgproc);
	return 0;
}

/*
监控WebSocket
*/
DWORD WINAPI WebSocketClientMonitor(HWND hwndDlg)
{
	Sleep(5000);  // 等待5秒才连接服务器端，目的是为了已经登录的微信端上报
	WsClientInit();  // 初始化
	return TRUE;
}
/*
监控WebSocket
*/
DWORD WINAPI WebSocketMonitor(HWND hwndDlg)
{
	WsServerInit();  // 初始化
	return TRUE;
}
/*
界面事件触发 - 回调
*/
INT_PTR CALLBACK Dlgproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 初始化
	if(uMsg == WM_INITDIALOG) {
		// 开一个线程开始监听WebSocket
		HANDLE sThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WebSocketMonitor, hwndDlg, NULL, 0);
		if (sThread != 0) {
			CloseHandle(sThread);
		}
		// 开一个线程开始连接服务端WebSocket
		HANDLE cThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WebSocketClientMonitor, hwndDlg, NULL, 0);
		if (cThread != 0) {
			CloseHandle(cThread);
		}
	}
	// 关闭窗口
	else if (uMsg == WM_CLOSE) {
		// 先关闭websocket监听
		WsServerClose();
		// 关闭界面
		EndDialog(hwndDlg, 0);
	}
	// 界面事件
	else if (uMsg == WM_COMMAND) {
		// 启动微信，并且注入
		if (wParam == ID_START_WECHAT) {
			// 开启微信
			StartWechat();
		}
		// 关闭所有微信
		else if (wParam == ID_CLOSE_ALL_WECHAT) {
			CloseAllWeChat();
		}
	}
	
	return FALSE;
}

