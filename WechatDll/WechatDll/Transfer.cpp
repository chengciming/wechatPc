#include "stdafx.h"
#include "resource.h"
#include "Config.h"
#include "WechatOffset.h"
#include "Common.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <tchar.h> 
#include <iostream>
#include <sstream>
#include <iomanip>
#include <strstream>
#include "Transfer.h"

using namespace std;

// 收款
void CollectMoney(wchar_t* wxid, wchar_t* transferid)
{
	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();

	CllectMoneyStruct cellect;
	cellect.ptransferid = transferid;
	cellect.transferidLen = wcslen(transferid) + 1;
	cellect.transferidMaxLen = wcslen(transferid) * 2;
	cellect.pwxid = wxid;
	cellect.wxidLen = wcslen(wxid) + 1;
	cellect.wxidMaxLen = wcslen(wxid) * 2;

	char* asmBuff = (char*)&cellect.ptransferid;

	DWORD dwCall1 = winAddress + TRANSFER_CALL1;
	DWORD dwCall2 = winAddress + TRANSFER_CALL2;
	
	__asm
	{
		sub esp, 0x30;
		mov ecx, esp;
		mov eax, asmBuff;
		push eax;
		call dwCall1;
		call dwCall2;
		add esp, 0x30;
	}
}
// 自动收款
void AutoCollectMoney(wchar_t* wxid, wstring msg)
{
	int pos1 = msg.find(L"<transferid>");
	int pos2 = msg.find(L"]]></transferid>");
	wstring noneed = L"<transferid><![CDATA[";
	int noneedLen = noneed.length();
	wstring transferid;
	transferid = msg.substr(pos1 + noneedLen, (pos2 - pos1) - noneedLen);
	CollectMoney(wxid, (wchar_t*)transferid.c_str());
}