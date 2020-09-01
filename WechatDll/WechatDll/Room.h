#pragma once
#include <stdio.h>
#include <string>
using namespace std;

//群加成员微信ID的结构体
struct AddRoomUserWxid
{
	wchar_t* str;
	int strLen = 0;
	int maxLen = 0;
	char file[0x8] = { 0 };
};
// 获取群成员的微信ID
struct GetRoomUserwxStr
{
	wchar_t * pStr;
	int strLen;
	int strMaxLen;
	int fill = 0;
	int fill2 = 0;
};
// 退出群聊结构
struct QuiteRoomStructWxid
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
// 修改群名称结构
struct SetRoomNameStruct
{
	wchar_t* pStr;
	int strLen;
	int strMaxLen;
	char full[0x8] = { 0 };
};
// 删除群成员结构
struct DelRoomIdStruct
{
	wchar_t* roomid;
	int roomidLen;
	int roomidMaxLen;
	int full = 0;
	int full2 = 0;
};
//发送艾特消息需要的数据结构
class TEXT_WX
{
public:
	wchar_t* pWxid = nullptr;
	DWORD length = 0;
	DWORD maxLength = 0;
	DWORD fill1 = 0;
	DWORD fill2 = 0;
	wchar_t wxid[1024] = { 0 };

	TEXT_WX(wstring wsWxid)
	{
		const wchar_t* temp = wsWxid.c_str();
		wmemcpy(wxid, temp, wsWxid.length());
		length = wsWxid.length();
		maxLength = wsWxid.capacity();
		fill1 = 0;
		fill2 = 0;
		pWxid = wxid;
	}
};
class ROOM_AT
{
public:
	DWORD at_WxidList = 0;
	DWORD at_end1 = 0;
	DWORD at_end2 = 0;
};
class  TEXT_WXID
{
public:
	wchar_t* pWxid = nullptr;
	DWORD length = 0;
	DWORD maxLength = 0;
	DWORD fill1 = 0;
	DWORD fill2 = 0;
};


// 获取群成员列表
void GetRoomUserList(wchar_t* chatroomwxid);
// 添加群成员
void AddRoomUser(wchar_t* chatroomwxid, wchar_t* wxid);
// 发送群公告
void SetRoomAnnouncement(wchar_t* chatroomwxid, wchar_t* Announcement);
// 退出群聊
void QuitRoom(wchar_t* chatroomwxid);
// 修改群名称
void SetRoomName(wchar_t* roomwxid, wchar_t* roomname);
// 发送艾特消息
void SendRoomAtMsg(wchar_t* chatroomid, wchar_t* memberwxid, wchar_t* membernickname, wchar_t* msg);
// 删除群成员
void DelRoomUser(wchar_t* roomid, wchar_t* memberwxid);
// 创建群聊
void CreateRoom(wchar_t *wxid1, wchar_t *wxid2);