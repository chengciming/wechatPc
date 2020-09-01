#include "stdafx.h"
#include "resource.h"
#include "Config.h"
#include "LoginInfo.h"
#include "Package.h"
#include "WechatOffset.h"
#include "Common.h"
#include "Login.h"
#include "Send.h"
#include "Struct.h"
#include <stdio.h>
#include <windows.h>
#include <string>
#include <tchar.h> 

using namespace std;

VOID SendWechatUser(Package *package)
{
	LoginInfo *info = new LoginInfo;

	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();

	HWND hwndDlg = GetGlobalHwnd();

	// 获取微信ID
	int wxidLength = (int)*((DWORD*)(winAddress + WX_USER_ID + 0x10));
	sprintf_s(info->wxid, "%s", (CHAR*)(winAddress + WX_USER_ID));
	if (strlen(info->wxid) != wxidLength) {  // 指针
		sprintf_s(info->wxid, "%s", (CHAR*)(*((DWORD *)(winAddress + WX_USER_ID))));
	}

	// 获取微信号
	DWORD pUsername;
	// 旧的微信，特殊偏移
	if (strstr(info->wxid, "wxid_") == NULL) {
		sprintf_s(info->username, "%s", (CHAR*)(winAddress + WX_USER_USERNAME_OLD));
		pUsername = winAddress + WX_USER_USERNAME_OLD;
	}
	else {
		sprintf_s(info->username, "%s", (CHAR*)(winAddress + WX_USER_USERNAME));
		pUsername = winAddress + WX_USER_USERNAME;
	}
	int usernameLength = (int)*((DWORD*)(pUsername + 0x10));
	if (strlen(info->username) != usernameLength) {  // 指针
		sprintf_s(info->username, "%s", (CHAR*)(*((DWORD *)pUsername)));
	}

	// 获取微信昵称
	int nicknameLength = (int)*((DWORD*)(winAddress + WX_USER_NICKNAME + 0x10));
	sprintf_s(info->nickname, "%s", (CHAR*)(winAddress + WX_USER_NICKNAME));
	if (strlen(info->nickname) != nicknameLength) {
		sprintf_s(info->nickname, "%s", (CHAR*)(*((DWORD *)(winAddress + WX_USER_NICKNAME))));
	}

	// 获取微信头像链接地址
	DWORD pHeadUrl = winAddress + WX_USER_HEAD_URL;
	sprintf_s(info->headUrl, "%s", (CHAR*)(*((DWORD *)pHeadUrl)));

	// 获取微信手机号码
	sprintf_s(info->mobile, "%s", (CHAR*)(winAddress + WX_USER_MOBILE));

	// 获取邮箱地址
	int emailLength = (int)*((DWORD*)(winAddress + WX_USER_EMAIL + 0x10));
	sprintf_s(info->email, "%s", (CHAR*)(winAddress + WX_USER_EMAIL));
	if (strlen(info->email) != emailLength) {  // 指针
		sprintf_s(info->email, "%s", (CHAR*)(*((DWORD *)(winAddress + WX_USER_EMAIL))));
	}

	// 获取性别：1=男，2=女，0=未知
	wchar_t sex[8] = { 0 };
	memcpy(sex, (void*)(winAddress + WX_USER_SEX), 1);
	if (sex[0] == 1) {
		info->sex = 1;
	}
	else if (sex[0] == 2) {
		info->sex = 2;
	}
	else {
		info->sex = 0;
	}

	// 获取国籍
	sprintf_s(info->nation, "%s", (CHAR*)(winAddress + WX_USER_NATIONALITY));

	// 获取省份
	sprintf_s(info->province, "%s", (CHAR*)(winAddress + WX_USER_PROVINCE));

	// 获取城市
	sprintf_s(info->city, "%s", (CHAR*)(winAddress + WX_USER_CITY));

	// 获取个性签名
	int signLength = (int)*((DWORD*)(winAddress + WX_USER_SIGN + 0x10));
	sprintf_s(info->sign, "%s", (CHAR*)(winAddress + WX_USER_SIGN));
	if (strlen(info->sign) != signLength) {
		sprintf_s(info->sign, "%s", (CHAR*)(*(DWORD*)(winAddress + WX_USER_SIGN)));
	}

	// 获取微信登录的设备
	sprintf_s(info->device, "%s", (CHAR*)(winAddress + WX_USER_DEVICE));

	// 发送数据
	Send::SendLoginInfo(info, package);
}

VOID ReadWechatUser()
{
	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();

	HWND hwndDlg = GetGlobalHwnd();

	// 获取微信ID
	CHAR wxid[0x100] = {0};
	int wxidLength = (int)*((DWORD*)(winAddress + WX_USER_ID + 0x10));
	sprintf_s(wxid, "%s", (CHAR*)(winAddress + WX_USER_ID));
	if (strlen(wxid) != wxidLength) {  // 指针
		DWORD pWxid = winAddress + WX_USER_ID;
		sprintf_s(wxid, "%s", (CHAR*)(*((DWORD *)pWxid)));
	}
	SetDlgItemText(hwndDlg, TEXT_ID_WXID, UTF8ToUnicode(wxid));

	// 获取微信号
	CHAR username[0x100] = { 0 };
	DWORD pUsername;
	// 旧的微信，特殊偏移
	if (strstr(wxid, "wxid_") == NULL) {
		sprintf_s(username, "%s", (CHAR*)(winAddress + WX_USER_USERNAME_OLD));
		pUsername = winAddress + WX_USER_USERNAME_OLD;
	}
	else {
		sprintf_s(username, "%s", (CHAR*)(winAddress + WX_USER_USERNAME));
		pUsername = winAddress + WX_USER_USERNAME;
	}
	int usernameLength = (int)*((DWORD*)(pUsername + 0x10));
	if (strlen(username) != usernameLength) {  // 指针
		sprintf_s(username, "%s", (CHAR*)(*((DWORD *)pUsername)));
	}
	SetDlgItemText(hwndDlg, TEXT_ID_USERNAME, UTF8ToUnicode(username));

	// 获取微信昵称
	CHAR nickname[0x100] = { 0 };
	int nicknameLength = (int)*((DWORD*)(winAddress + WX_USER_NICKNAME + 0x10));
	sprintf_s(nickname, "%s", (CHAR*)(winAddress + WX_USER_NICKNAME));
	if (strlen(nickname) != nicknameLength) {
		sprintf_s(nickname, "%s", (CHAR*)(*((DWORD *)(winAddress + WX_USER_NICKNAME))));
	}

	SetDlgItemText(hwndDlg, TEXT_ID_NICKNAME, UTF8ToUnicode(nickname));

	// 获取微信头像链接地址
	CHAR headUrl[0x256] = { 0 };
	DWORD pHeadUrl = winAddress + WX_USER_HEAD_URL;
	sprintf_s(headUrl, "%s", (CHAR*)(*((DWORD *)pHeadUrl)));
	SetDlgItemText(hwndDlg, TEXT_ID_HEAD_URL, UTF8ToUnicode(headUrl));

	// 获取微信手机号码
	CHAR mobile[0x32] = { 0 };
	sprintf_s(mobile, "%s", (CHAR*)(winAddress + WX_USER_MOBILE));
	SetDlgItemText(hwndDlg, TEXT_ID_MOBILE, UTF8ToUnicode(mobile));

	// 获取邮箱地址
	CHAR email[0x32] = { 0 };
	int emailLength = (int)*((DWORD*)(winAddress + WX_USER_EMAIL + 0x10));
	sprintf_s(email, "%s", (CHAR*)(winAddress + WX_USER_EMAIL));
	if (strlen(email) != emailLength) {  // 指针
		DWORD pEmail = winAddress + WX_USER_EMAIL;
		sprintf_s(email, "%s", (CHAR*)(*((DWORD *)pEmail)));
	}
	SetDlgItemText(hwndDlg, TEXT_ID_EMAIL, UTF8ToUnicode(email));

	// 获取性别
	wchar_t sex[8] = { 0 };
	memcpy(sex, (void*)(winAddress + WX_USER_SEX), 1);
	if (sex[0] == 1) {
		wcscpy_s(sex, wcslen(L"男") + 1, L"男");
	}
	else if (sex[0] == 2) {
		wcscpy_s(sex, wcslen(L"女") + 1, L"女");
	}
	else {
		wcscpy_s(sex, wcslen(L"未知") + 1, L"未知");
	}
	SetDlgItemText(hwndDlg, TEXT_ID_SEX, sex);

	// 获取国籍
	CHAR nation[0x32] = { 0 };
	sprintf_s(nation, "%s", (CHAR*)(winAddress + WX_USER_NATIONALITY));
	SetDlgItemText(hwndDlg, TEXT_ID_NATION, UTF8ToUnicode(nation));

	// 获取省份
	CHAR province[0x32] = { 0 };
	sprintf_s(province, "%s", (CHAR*)(winAddress + WX_USER_PROVINCE));
	SetDlgItemText(hwndDlg, TEXT_ID_PROVINCE, UTF8ToUnicode(province));

	// 获取城市
	CHAR city[0x32] = { 0 };
	sprintf_s(city, "%s", (CHAR*)(winAddress + WX_USER_CITY));
	SetDlgItemText(hwndDlg, TEXT_ID_CITY, UTF8ToUnicode(city));

	// 获取个性签名
	CHAR sign[0x32] = { 0 };
	int signLength = (int)*((DWORD*)(winAddress + WX_USER_SIGN + 0x10));
	sprintf_s(sign, "%s", (CHAR*)(winAddress + WX_USER_SIGN));
	if (strlen(sign) != signLength) {
		sprintf_s(sign, "%s", (CHAR*)(*(DWORD*)(winAddress + WX_USER_SIGN)));
	}
	SetDlgItemText(hwndDlg, TEXT_ID_SIGN, UTF8ToUnicode(sign));

	// 获取微信登录的设备
	CHAR device[0x32] = { 0 };
	sprintf_s(device, "%s", (CHAR*)(winAddress + WX_USER_DEVICE));
	SetDlgItemText(hwndDlg, TEXT_ID_DEVICE, UTF8ToUnicode(device));

	// 跳转到二维码界面
	/*
	GotoQrCode();
	// 获取二维码值
	CHAR qrCodeUrl[0x100] = { 0 };
	DWORD pCode = 0;
	while (TRUE) {
		pCode = *((DWORD *)(winAddress + WX_QRCODE_URL_CODE));
		if (pCode && strlen((CHAR*)pCode) > 0) {
			sprintf_s(qrCodeUrl, "http://weixin.qq.com/x/%s", (CHAR*)pCode);
			break;
		}
	}
	SetDlgItemText(hwndDlg, TEXT_ID_LOGIN_QRCODE, UTF8ToUnicode(qrCodeUrl));
	*/
}
