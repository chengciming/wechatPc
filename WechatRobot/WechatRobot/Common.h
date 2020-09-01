#pragma once
#include <Windows.h>
#include <io.h>
#include <string.h>
#include <stdio.h>
#include <direct.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdlib>

#define random(x) (rand()%x)

using namespace std;

// 获取DLL路径
void GetDllPath(char *dllPath, int dllSize);

// 写入内存
LPVOID writeMemory(HANDLE hProcess, char *path);

// 加载DLL
INT_PTR loadLibrary(HANDLE hProcess, LPVOID dllAddress);

// 查找进程ID
DWORD ProcessNameFindPid(LPCSTR processName);

// UTF-8到GB2312的转换
char* UTF8ToGB2312(char* utf8);
// GB2312到UTF-8的转换
char* GB2312ToUTF8(char* gb2312);
// 编码转换
char* UnicodeToUtf8(wchar_t* unicode);
// 编码转换
wchar_t* UTF8ToUnicode(char* str);
// char*转wchar*
wchar_t *CharToWChar(char *cStr);
// wchar*转char*
CHAR *WcharToChar(wchar_t *wStr);
// 获取随机数
int GetRand(int number);
// 获取时间戳 - 精切到毫秒
long long CreateTimestamp();
// 创建唯一值
string CreateUniqueId();
// 获取当前执行文件的目录
string GetExePath(void);
// 读取配置文件
string ReadConfig(string key);