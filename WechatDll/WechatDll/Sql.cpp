#include "stdafx.h"
#include "Common.h"
#include "Sql.h"
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <atlstr.h>
#include <Shlwapi.h>
#include <CommCtrl.h>
#include <list>
#pragma comment(lib,"Shlwapi.lib")
using namespace std;

list<DbHandle> DbHandleList;
DWORD databaseJumpAddr = 0;

// 将获取到的数据库句柄压入list内
VOID databaseHwnd(int dbAddress, int dbHandle)
{
	DbHandle Db = { 0 };
	Db.handler = dbHandle;
	sprintf_s(Db.path, "%s", (char*)dbAddress);
	DbHandleList.push_back(Db);
	//HWND dbSelect = GetDlgItem(getGlobalHwnd(), DATABASE_SELECT);
	//assert(dbSelect);
	//wchar_t address[0x100] = { 0 };
	//swprintf_s(address, L"%S", (char*)dbAddress);
	//wstring dbPath = wstring(address);
	//int pos1 = dbPath.find(L"\\Msg\\");
	//wstring dbName = dbPath.substr(pos1 + 5);
	//MessageBox(NULL, dbName.c_str(),L"",NULL);
	//SendMessage(dbSelect, CB_ADDSTRING, 0, (LPARAM)dbName.c_str());
}
__declspec(naked) void databaseDeclspec()
{
	__asm
	{
		mov esi, dword ptr ss : [ebp - 0x14]
		add esp, 0x8
		pushad
		push[ebp - 0x14]
		push[ebp - 0x24]
		call databaseHwnd
		add esp, 8
		popad
		jmp databaseJumpAddr
	}
}
void ListenDbHandle()
{
	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();
	//HOOK数据库句柄
	DWORD hookAddress = winAddress + 0x481DD3;
	databaseJumpAddr = hookAddress + 6;
	BYTE dbJmpCode[6] = { 0xE9 };
	dbJmpCode[5] = 0x90;
	*(DWORD*)&dbJmpCode[1] = (DWORD)databaseDeclspec - hookAddress - 5;
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)hookAddress, dbJmpCode, 6, NULL);
}
// 执行SQL
int runSql(string dbName, string sqlStr, sqlite3_callback callBack)
{
	for (auto& db : getDbHandleList()) {
		if (StrStrA(db.path, dbName.c_str())) {
			// 获取微信基址
			DWORD winAddress = GetWechatWinAddress();
			DWORD callAddr = winAddress + 0x8C6B70;
			char *sql = (char*)sqlStr.c_str();
			DWORD *asmHandle = (DWORD*)db.handler;
			DWORD *asmCallback = (DWORD*)callBack;
			__asm {
				mov eax, asmHandle
				push 0x0
				push 0x0
				push asmCallback
				push sql
				push eax
				call callAddr  // sqlite3_exec
				add esp, 0x14
			}
		}
	}
	return 1;
}

// 返回数据库句柄列表
list<DbHandle> getDbHandleList()
{
	return DbHandleList;
}
VOID clearDbHandleList()
{
	DbHandleList.clear();
	return;
}