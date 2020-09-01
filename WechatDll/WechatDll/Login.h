#pragma once
#include "stdafx.h"

// 退出登录
void DoLogout();
// 获取登录状态
int CheckLoginStatus();
// 获取登录状态和登录二维码
bool SendLoginStatus(bool sendQrcode = TRUE);
// 跳转到二维码界面
void GotoQrCode();