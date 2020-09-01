#pragma once
#include "Package.h"

// 同意加好友的参数：v1
struct v1Info
{
	int fill = 0;
	wchar_t *v1;
	int v1Len;
	int maxV1Len;
	char fill2[0x41C] = { 0 };
	DWORD v2 = 0;
};
// 同意加好友的参数：v2
struct v2Info
{
	char fill[0x24C] = {0};
	DWORD fille3 = 0x25;
	char fill4[0x40] = { 0 };
	wchar_t *v2;
	int v2Len;
	int maxV2Len;
	char fill2[0x8] = { 0 };
};
// 加好友的参数
struct wechatText
{
	wchar_t *pStr;
	int strLen;
	int strMaxLen;
};
// 好友详情
struct userInfo
{
	wchar_t wxid[0x100] = { 0 };
	wchar_t username[0x100] = { 0 };
	wchar_t nickname[0x100] = { 0 };
	wchar_t remark[0x200] = { 0 };
	wchar_t avatar[0x200] = { 0 };
	wchar_t v1[0x100] = { 0 };
};

//删除好友结构体
struct DeleteUserStructWxid
{
	//发送的文本消息指针
	wchar_t* pWxid;
	//字符串长度
	DWORD length;
	//字符串最大长度
	DWORD maxLength;

	//补充两个占位数据
	DWORD fill1;
	DWORD fill2;
};
//通过推荐卡片加好友结构体
struct AddUserFromCardStruct
{
	wchar_t* pStr;
	int strLen;
	int strMaxLen;
};
// 设置好友备注结构
struct SetRemarkStruct
{
	wchar_t *remark;
	int remarkLen;
	int maxRemarkLen;
};
// 查询数据库好友列表
void selectFriendList();
// 监听好友列表更新
void ListFriendsList();
// 发送好友列表
void SendList(Package *package = NULL, int page = 1, int pageSize = 0, int total = 0);
// 同意加好友请求
void VerifyFriend(PWCHAR v1, PWCHAR v2);
// 加好友
void AddFriend(PWCHAR wxid, PWCHAR message);
// 获取好友详情
userInfo GetUserInfoByWxid(wchar_t* userwxid);
// 删除好友
void DeleteUser(wchar_t* wxid);
// 通过推荐卡片添加好友
void AddUserFromCard(wchar_t* v1, wchar_t* msg);
// 设置好友备注
void SetRemark(wchar_t *wxid, wchar_t *remark);