#pragma once
#include "Config.h"

// 界面触发回调
INT_PTR CALLBACK Dlgproc(_In_ HWND hwndDlg, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
// 显示界面
DWORD WINAPI ShowDialog(HMODULE hModule);
// 监控微信
DWORD WINAPI Monitor(HMODULE hModule);
// 监控WebSocket
DWORD WINAPI WebSocketClientMonitor(HMODULE hModule);