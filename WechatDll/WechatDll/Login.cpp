#include "stdafx.h"
#include "resource.h"
#include "Config.h"
#include "Common.h"
#include "WechatOffset.h"
#include "Send.h"
#include "Login.h"

// 退出登录
void DoLogout()
{
	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();
	DWORD dwCallAddr = winAddress + LOGOUT;

	__asm {
		pushad
		push 0x0
		call dwCallAddr;
		popad
	}
}

// 获取登录状态
int CheckLoginStatus()
{
	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();
	// 检查是否已经登陆
	wchar_t isLogin[8] = { 0 };
	memcpy(isLogin, (void*)(winAddress + ISLOGIN), 1);
	if (isLogin[0] == 1) {
		// 返回已登录
		return 1;
	}
	return 0;
}

bool SendLoginStatus(bool sendQrcode) 
{
	// 检查是否已经登陆
	if (CheckLoginStatus() == 1) {
		// 通知服务器端登录成功
		Send::SendLoginStatus(1);
		// 返回已登录
		return TRUE;
	}

	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();

	// 获取二维码值
	CHAR qrCodeUrl[0x100] = { 0 };
	DWORD pCode = 0;
	while (TRUE) {
		pCode = *((DWORD *)(winAddress + WX_QRCODE_URL_CODE));
		if (pCode && strlen((CHAR*)pCode) > 0) {
			sprintf_s(qrCodeUrl, "http://weixin.qq.com/x/%s", (CHAR*)pCode);
			if (sendQrcode == TRUE) {
				Send::SendLoginQrcode(0, qrCodeUrl);  // 发送回服务器端
			}
			break;
		}
	}

	// 返回未登录
	return FALSE;
}

/*
跳转到二维码窗口
*/
void GotoQrCode()
{
	return;
	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();

	// 检查是否已经登陆
	if (CheckLoginStatus() == 1) {
		return;  // 已经登陆，不再跳转
	}
	DWORD dwCallAddr1 = winAddress + GOTO_QRCODE_1;
	DWORD dwCallAddr2 = winAddress + GOTO_QRCODE_2;

	__asm {
		pushad
		call dwCallAddr1;
		mov ecx, eax;
		call dwCallAddr2;
		popad
	}
}