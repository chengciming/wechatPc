#include "stdafx.h"
#include "Config.h"
#include "Common.h"
#include "Md5.h"
#include "ReadConfig.h"
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

using namespace std;

ConfigStruct Setting;

LPVOID writeMemory(HANDLE hProcess, char *path)
{
	// 申请内存
	LPVOID dllAddress = VirtualAllocEx(hProcess, NULL, strlen(path), MEM_COMMIT, PAGE_READWRITE);
	if (dllAddress == NULL) {
		return NULL;
	}

	// 写入内存
	if (WriteProcessMemory(hProcess, dllAddress, path, strlen(path), NULL) == 0) {
		return NULL;
	}
	return dllAddress;
}

/*
加载DLL
*/
INT_PTR loadLibrary(HANDLE hProcess, LPVOID dllAddress)
{
	HMODULE k32 = GetModuleHandle(L"Kernel32.dll");
	LPVOID loadAddress = GetProcAddress(k32, "LoadLibraryA");
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadAddress, dllAddress, 0, NULL);
	if (hThread == NULL) {
		return FALSE;
	}
	return TRUE;
}

/*
查找进程ID
*/
DWORD ProcessNameFindPid(LPCSTR processName)
{
	// 获取进程快照
	HANDLE processAll = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	// 遍历进程
	PROCESSENTRY32 processInfo = { 0 };
	processInfo.dwSize = sizeof(PROCESSENTRY32);

	do {
		if (strcmp(processName, (char *)processInfo.szExeFile) == 0) {
			return processInfo.th32ProcessID;
		}
	} while (Process32Next(processAll, &processInfo));

	return NULL;
}
// 获取DLL路径
void GetDllPath(char *dllPath, int dllSize)
{
	// 获取当前目录
	string strUrl = GetExePath();

	sprintf_s(dllPath, dllSize, "%s\\%s", strUrl.c_str(), DLL_NAME);
	if (_access(dllPath, 0) == -1) {
		return ;
	}
	return ;
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
	char str[70] = {0};
	sprintf_s(str, sizeof(str), "%lld%d", intTimestamp, GetRand(8));
	

	MD5 *md5 = new MD5();
	string res = md5->encode(str);
	// 转大写
	transform(res.begin(), res.end(), res.begin(), ::toupper);
	return res;
	//sprintf_s(uniqueId, 33, "%s", res.c_str());
}
// 获取当前执行文件的目录
string GetExePath(void)
{
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	(strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串  
	string path = szFilePath;

	return path;
}
// 读取配置文件
string ReadConfig(string key)
{
	string value = "";
	
	if (Setting.isRead == FALSE) {
		string strUrl = GetExePath();
		CHAR configPath[MAX_PATH] = { 0 };
		sprintf_s(configPath, "%s\\%s", strUrl.c_str(), CONFIG_PATH);
		if (_access(configPath, 0) == -1) {
			MessageBox(NULL, L"配置文件不存在！", L"温馨提示：", NULL);
			return value;
		}
		Config configSettings(configPath);
		Setting.ConfigSetting = configSettings;
		Setting.isRead = TRUE;
	}

	value = Setting.ConfigSetting.Read(key, value);

	return value;
}