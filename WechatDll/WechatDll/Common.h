#pragma once
#include "stdafx.h"
#include "Config.h"
#include "Common.h"
#include <Windows.h>
#include <io.h>
#include <string.h>
#include <stdio.h>
#include <direct.h>
#include <TlHelp32.h>
#include <stdlib.h>
#include <algorithm>
#include <time.h>
#include <sys/timeb.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <atlstr.h>

#define random(x) (rand()%x)

using namespace std;

// 获取微信基址
DWORD GetWechatWinAddress();
// 设置窗口句柄
VOID SetGlobalHwnd(HWND hwndDlg);
// 获取窗口句柄
HWND GetGlobalHwnd();
// 读取内存中的字符串
LPCWSTR GetMsgByAddress(DWORD memAddress);
// 获取随机数
int GetRand(int number);
// 获取当前时间戳 - 精切到毫秒
long long CreateTimestamp();
// 创建唯一值
string CreateUniqueId();
// UTF8转GB2312
char* UTF8ToGB2312(char* utf8);
// GB2312转UTF8
char* GB2312ToUTF8(char* gb2312);
// UNICODE转UTF8
char* UnicodeToUtf8(wchar_t* unicode);
// UTF8转UNICODE
wchar_t* UTF8ToUnicode(char* str);
// char*转wchar*
wchar_t *CharToWChar(char *cStr);
// wchar*转char*
CHAR *WcharToChar(wchar_t *wStr);
