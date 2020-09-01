#include "stdafx.h"
#include "Config.h"
#include "Md5.h"
#include "Common.h"
#include <Windows.h>
#include <io.h>
#include <string.h>
#include <string>
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
#include <vector>
#include <cmath>

using namespace std;

// 微信基址
DWORD weChatWinAddress = 0;
// 窗口句柄
HWND globalHwnd;

/*
获取微信基址
*/
DWORD GetWechatWinAddress()
{
	if (weChatWinAddress == 0) {
		HMODULE winAddress = LoadLibrary(L"WeChatWin.dll");
		weChatWinAddress = (DWORD)winAddress;
	}
	return weChatWinAddress;
}

/*
设置窗口句柄
*/
VOID SetGlobalHwnd(HWND hwndDlg) 
{
	globalHwnd = hwndDlg;
}

/*
获取窗口句柄
*/
HWND GetGlobalHwnd() 
{
	return globalHwnd;
}

//读取内存中的字符串
//存储格式
//xxxxxxxx:字符串地址（memAddress）
//xxxxxxxx:字符串长度（memAddress +4）
LPCWSTR GetMsgByAddress(DWORD memAddress)
{
	//获取字符串长度
	DWORD msgLength = *(DWORD*)(memAddress + 4);
	if (msgLength == 0)
	{
		WCHAR* msg = new WCHAR[1];
		msg[0] = 0;
		return msg;
	}

	WCHAR* msg = new WCHAR[msgLength + 1];
	ZeroMemory(msg, msgLength + 1);

	//复制内容
	wmemcpy_s(msg, msgLength + 1, (WCHAR*)(*(DWORD*)memAddress), msgLength + 1);
	return msg;
}

/*
获取随机数
*/
int GetRand(int number) 
{
	srand((unsigned int)time(NULL));//通过系统时间初始化随机数种子//如果不设置随机数种子,则每次rand()函数的起始值都一样.
	int n = random(number) % 181 + 20;// rand()为随机数函数, 产生[0,int型最大值)之间的整数数, 此值依系统不同而不同, 一般为2147483648, 可认为无穷// 通过rand()%181+20转化为[20,200]之间的整数 (注:rand()%181产生[0,180]之间的整数)
	return n; //返回这个整数
}

/*
获取当前时间戳 - 精切到毫秒
*/
long long CreateTimestamp()
{
	timeb t;
	ftime(&t);
	return t.time * 1000 + t.millitm;
}

// 创建唯一值
string CreateUniqueId()
{
	long long intTimestamp = CreateTimestamp();
	char str[70] = { 0 };
	sprintf_s(str, sizeof(str), "%lld%d", intTimestamp, GetRand(8));


	MD5 *md5 = new MD5();
	string res = md5->encode(str);
	// 转大写
	transform(res.begin(), res.end(), res.begin(), ::toupper);
	return res;
	//sprintf_s(uniqueId, 33, "%s", res.c_str());
}

//UTF-8到GB2312的转换
char* UTF8ToGB2312(char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

//GB2312到UTF-8的转换
char* GB2312ToUTF8(char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

/*
编码转换
 */
char* UnicodeToUtf8(wchar_t* unicode)
{
	int len;
	len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
	char* szUtf8 = (char*)malloc(len + 1);
	if (szUtf8 != 0) {
		memset(szUtf8, 0, len + 1);
	}
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, szUtf8, len, NULL, NULL);
	return szUtf8;
}

/*
编码转换
*/
wchar_t* UTF8ToUnicode(char* str)
{
	int    textlen = 0;
	wchar_t* result;
	textlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	result = (wchar_t*)malloc((textlen + 1) * sizeof(wchar_t));
	if (result != 0)
	{
		memset(result, 0, (textlen + 1) * sizeof(wchar_t));
	}
	MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)result, textlen);
	return    result;
}

// char*转wchar*
wchar_t *CharToWChar(char *cStr)
{
	size_t len = strlen(cStr) + 1;
	size_t converted = 0;
	wchar_t *wStr;
	wStr = (wchar_t*)malloc(len * sizeof(wchar_t));
	mbstowcs_s(&converted, wStr, len, cStr, _TRUNCATE);
	return wStr;
}

// wchar*转char*
CHAR *WcharToChar(wchar_t *wStr)
{
	size_t len = wcslen(wStr) + 1;
	size_t converted = 0;
	char *cStr;
	cStr = (char*)malloc(len * sizeof(char));
	wcstombs_s(&converted, cStr, len, wStr, _TRUNCATE);
	return cStr;
}
