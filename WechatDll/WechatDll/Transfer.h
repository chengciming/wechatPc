#pragma once

// 收款结构体
struct CllectMoneyStruct
{
	wchar_t* ptransferid;
	int transferidLen;
	int transferidMaxLen;
	char full[0x8] = { 0 };
	wchar_t* pwxid;
	int wxidLen;
	int wxidMaxLen;
	char full2[0x8] = { 0 };
};

using namespace std;

// 收款
void CollectMoney(wchar_t* wxid, wchar_t* transferid);
// 自动收款
void AutoCollectMoney(wchar_t* wxid, wstring msg);