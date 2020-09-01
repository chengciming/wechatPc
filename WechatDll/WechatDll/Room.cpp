#include "stdafx.h"
#include "resource.h"
#include "Config.h"
#include "WechatOffset.h"
#include "Common.h"
#include "Struct.h"
#include <stdio.h>
#include <windows.h>
#include <string>
#include <tchar.h> 
#include <sstream>
#include <iomanip>
#include <strstream>
#include <list>
#include <iostream>
#include <map>
#include "Friends.h"
#include "Room.h"



/*==================================获取群成员列表开始=======================================*/
void GetRoomUserList(wchar_t* chatroomwxid)
{
	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();

	//1.先获取群成员的微信ID
	
	//准备缓冲区
	DWORD dwWxidArr = 0;	//保存微信ID数据的地址
	char buff[0x164] = { 0 };
	char userListBuff[0x174] = { 0 };

	//构造数据
	GetRoomUserwxStr pWxid = { 0 };
	pWxid.pStr = chatroomwxid;
	pWxid.strLen = wcslen(chatroomwxid);
	pWxid.strMaxLen = wcslen(chatroomwxid) * 2;
	char * asmWxid = (char *)&pWxid.pStr;

	//调用call
	DWORD dwCall1 = winAddress + WX_GET_ROOM_USER_WXID_CALL1;
	DWORD dwCall2 = winAddress + WX_GET_ROOM_USER_WXID_CALL2;
	DWORD dwCall3 = winAddress + WX_GET_ROOM_USER_WXID_CALL3;
	DWORD dwCall4 = winAddress + WX_GET_ROOM_USER_WXID_CALL4;

	//获取群成员
	__asm {
		lea ecx, buff[16]
		call dwCall1
		lea eax, buff[16]
		push eax
		mov ebx, asmWxid
		push ebx
		call dwCall2
		mov ecx, eax
		call dwCall3
		lea eax, buff
		push eax
		lea ecx, buff[16]
		call dwCall4
		mov dwWxidArr, eax
	}

	//拿到微信ID
	wchar_t test[0x100] = { 0 };
	wchar_t tempWxid[0x100] = { 0 };
	DWORD userList = *((DWORD *)dwWxidArr);
	DWORD testTmp = dwWxidArr + 0xB0;
	int Len = *((int *)testTmp);

	for (int i = 0; i < Len; i++)
	{
		DWORD temWxidAdd = userList + (i * 20);
		swprintf_s(tempWxid, L"%s", (wchar_t*)*((LPVOID *)temWxidAdd));

		//2.再通过微信ID获取群成员信息
		userInfo user = GetUserInfoByWxid(tempWxid);
		MessageBox(NULL, user.wxid, L"好", NULL);
		// user=用户想起结构
	}
}
/*==================================获取群成员列表结束=======================================*/

/*==================================添加群成员开始=======================================*/
void AddRoomUser(wchar_t* chatroomwxid, wchar_t* wxid)
{
	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();

	DWORD dwCallAddr1 = winAddress + WX_ADD_ROOM_MEMBER_CALL1;
	DWORD dwCallAddr2 = winAddress + WX_ADD_ROOM_MEMBER_CALL2;
	DWORD dwCallAddr3 = winAddress + WX_ADD_ROOM_MEMBER_CALL3;
	DWORD dwCallAddr4 = winAddress + WX_ADD_ROOM_MEMBER_CALL4;

	DWORD dwParam = winAddress + WX_ADD_ROOM_MEMBER_PARAM1;
	DWORD dwParam2 = winAddress + WX_ADD_ROOM_MEMBER_PARAM2;
	
	//群ID结构体
	struct tempChatRoom
	{
		char fill2[0x8] = { 0 };
		wchar_t* str;
		int strLen = 0;
		int maxLen = 0;
		char fill[0x8] = { 0 };
	};


	//构造需要的数据
	AddRoomUserWxid wxidStruct = { 0 };
	wxidStruct.str = wxid;
	wxidStruct.strLen = wcslen(wxid) * 2;
	wxidStruct.maxLen = wcslen(wxid) * 4;

	tempChatRoom chatroomStruct = { 0 };
	chatroomStruct.str = chatroomwxid;
	chatroomStruct.strLen = wcslen(chatroomwxid) * 2;
	chatroomStruct.maxLen = wcslen(chatroomwxid) * 4;

	char wxidBuff[0xC] = { 0 };
	char tempWxid[0x14] = { 0 };
	char tempBuff[0x14] = { 0 };

	char* pWxid = (char*)&wxidStruct.str;
	char* pChatRoom = (char*)&chatroomStruct.fill2;

	__asm {
		pushad;
		lea esi, wxidBuff;
		mov ecx, esi;
		mov eax, pWxid;
		push eax;
		call dwCallAddr1;
		push 0;
		push dwParam;
		lea ecx, tempWxid;
		call dwCallAddr2;
		sub esp, 0x14;
		mov ecx, pChatRoom;
		mov eax, esp;
		push eax;
		call dwCallAddr3;
		push esi;
		mov ecx, dwParam2;
		call dwCallAddr4;
		popad;
	}
}
/*==================================添加群成员结束=======================================*/

/*==================================发送群公告开始=======================================*/
void SetRoomAnnouncement(wchar_t* chatroomwxid, wchar_t* Announcement)
{
	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();

	//获取WeChatWin的基址
	DWORD callAdrress = winAddress + WX_SET_ROOM_ANNOUNCEMENT;
	//组装数据
	CHAR bufferA[0xD0] = { 0 };
	DWORD* bufA = (DWORD*)& bufferA;

	CHAR buffer[0xD0] = { 0 };
	DWORD* buf = (DWORD*)& buffer;

	buf[0] = (DWORD)chatroomwxid;
	buf[1] = wcslen(chatroomwxid);
	buf[2] = wcslen(chatroomwxid) * 2;
	buf[3] = 0;
	buf[4] = 0;

	buf[0 + 5] = (DWORD)Announcement;
	buf[1 + 5] = wcslen(Announcement);
	buf[2 + 5] = wcslen(Announcement) * 2;
	buf[3 + 5] = 0;
	buf[4 + 5] = 0;

	bufA[0] = (DWORD)& buffer;
	bufA[1] = bufA[0] + 0x60;
	bufA[2] = bufA[0] + 0x60;

	DWORD r_esp = 0;
	__asm
	{
		//保存堆栈寄存器
		mov r_esp, esp

		//654B85B1 | .  8D85 30FFFFFF lea eax, [local.52]    //  lea eax,dword ptr ss:[ebp-0xD0]
		//654B85B7 | .  50            push eax
		//654B85B8 | .E8 43FF2000   call WeChatWi.0F411450;  发布群公告
		lea eax, bufferA
		push eax
		call callAdrress

		//恢复堆栈寄存器
		mov eax, r_esp
		mov esp, eax
	}
}
/*==================================发送群公告结束=======================================*/



/*==================================退出群聊开始=======================================*/
void QuitRoom(wchar_t* chatroomwxid)
{
	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();
	DWORD dwCallAddr = winAddress + WX_QUIT_ROOM;

	//构造数据
	QuiteRoomStructWxid structWxid = { 0 };
	structWxid.pWxid = chatroomwxid;
	structWxid.length = wcslen(chatroomwxid);
	structWxid.maxLength = wcslen(chatroomwxid) * 2;

	DWORD* asmMsg = (DWORD*)&structWxid.pWxid;

	__asm {
		mov ebx, asmMsg;
		push ebx;
		call dwCallAddr;
	}
}
/*==================================退出群聊结束=======================================*/

/*==================================修改群名称开始=======================================*/
void SetRoomName(wchar_t* roomwxid, wchar_t* roomname)
{
	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();
	
	SetRoomNameStruct pRoomwxid = { 0 };
	pRoomwxid.pStr = roomwxid;
	pRoomwxid.strLen = wcslen(roomwxid) + 1;
	pRoomwxid.strMaxLen = (wcslen(roomwxid) + 1) * 2;

	SetRoomNameStruct pRoomname = { 0 };
	pRoomname.pStr = roomname;
	pRoomname.strLen = wcslen(roomname) + 1;
	pRoomname.strMaxLen = (wcslen(roomname) + 1) * 2;

	char* asmWxid = (char*)&pRoomwxid.pStr;
	char* asmRoomname = (char*)&pRoomname.pStr;

	DWORD dwCall1 = winAddress + WX_SET_ROOM_NAME;


	__asm
	{
		mov edx, asmRoomname;
		mov ecx, asmWxid;
		call dwCall1;
	}
}
/*==================================修改群名称结束=======================================*/

/*==================================发送艾特消息开始=======================================*/
void SendRoomAtMsg(wchar_t* chatroomid, wchar_t* memberwxid, wchar_t* membernickname, wchar_t* msg)
{
	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();
	DWORD callAddress_SendText = winAddress + SEND_MESSAGE_CALL;


	TEXT_WX wxId(chatroomid);
	wchar_t tempmsg[100] = { 0 };
	swprintf_s(tempmsg, L"@%s %s", membernickname, msg);
	TEXT_WX wxMsg(tempmsg);

	TEXT_WXID wxAtId;
	wxAtId.pWxid = memberwxid;
	wxAtId.length = wcslen(memberwxid);
	wxAtId.maxLength = wcslen(memberwxid) * 2;
	wxAtId.fill1 = 0;
	wxAtId.fill2 = 0;
	//DWORD* asmWxid = (DWORD*)& wxAtId.pWxid;

	ROOM_AT roomAt;
	roomAt.at_WxidList = (DWORD)& wxAtId.pWxid;
	roomAt.at_end1 = roomAt.at_WxidList + 5 * 4;
	roomAt.at_end2 = roomAt.at_end1;

	//定义一个缓冲区
	BYTE buff[0x81C] = { 0 };

	//执行汇编调用
	__asm
	{
		lea edx, wxId

		//传递参数
		push 0x1

		lea eax, roomAt
		push eax

		//微信消息内容
		lea ebx, wxMsg

		push ebx
		lea ecx, buff

		//调用函数
		call callAddress_SendText

		//平衡堆栈
		add esp, 0xC
	}
}
/*==================================发送艾特消息结束=======================================*/


/*==================================删除群成员开始=======================================*/
void DelRoomUser(wchar_t* roomid, wchar_t* memberwxid)
{
	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();

	//拿到call的数据地址
	DWORD dwCall1 = winAddress + WX_DEL_ROOM_USER_CALL1;
	DWORD dwCall2 = winAddress + WX_DEL_ROOM_USER_CALL2;
	DWORD dwCall3 = winAddress + WX_DEL_ROOM_USER_CALL3;

	//组装群ID结构体
	DelRoomIdStruct roomiddata;
	roomiddata.roomid = roomid;
	roomiddata.roomidLen = wcslen(roomid) + 1;
	roomiddata.roomidMaxLen = (wcslen(roomid) + 1) * 2;

	//组装微信ID结构体
	wchar_t wxidbuff[0xD0] = { 0 };
	DWORD* dwBuff = (DWORD*)&wxidbuff;
	dwBuff[0] = (DWORD)memberwxid;
	dwBuff[1] = wcslen(memberwxid);
	dwBuff[2] = wcslen(memberwxid) * 2;
	dwBuff[3] = 0;
	dwBuff[4] = 0;

	wchar_t datatbuffer[0xD0] = { 0 };
	DWORD* dwDatabuf = (DWORD*)&datatbuffer;
	dwDatabuf[0] = (DWORD)& wxidbuff;
	dwDatabuf[1] = dwDatabuf[0] + 0x14;
	dwDatabuf[2] = dwDatabuf[0] + 0x14;

	//调用删除群成员call
	__asm
	{
		pushad;
		sub esp, 0x14;
		lea eax, roomiddata.roomid;
		mov ecx, esp;
		push eax;
		call dwCall1;
		lea edi, datatbuffer;
		push edi;
		call dwCall2;
		mov ecx, eax;
		call dwCall3;
		popad;
	}
}
/*==================================删除群成员结束=======================================*/


/*==================================创建群聊开始=======================================*/
void CreateRoom(wchar_t *wxid1, wchar_t *wxid2)
{
	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();

	//调用call
	DWORD dwParam = winAddress + WX_CREATE_ROOM_PARAM;
	DWORD dwCall = winAddress + WX_CREATE_ROOM_CALL;

	//组装微信ID结构体
	wchar_t wxidbuff[0xD0] = { 0 };
	DWORD* dwBuff = (DWORD*)&wxidbuff;
	dwBuff[0] = (DWORD)wxid1;
	dwBuff[1] = wcslen(wxid1);
	dwBuff[2] = wcslen(wxid1) * 2;
	dwBuff[3] = 0;
	dwBuff[4] = 0;
	dwBuff[5] = (DWORD)wxid2;
	dwBuff[6] = wcslen(wxid2);
	dwBuff[7] = wcslen(wxid2) * 2;
	dwBuff[8] = 0;
	dwBuff[9] = 0;

	wchar_t databuffer[0xD0] = { 0 };
	DWORD* dwDatabuf = (DWORD*)&databuffer;
	dwDatabuf[0] = (DWORD)&wxidbuff;
	dwDatabuf[1] = dwDatabuf[0] + 0x28;
	dwDatabuf[2] = dwDatabuf[0] + 0x28;
	
	//获取群成员
	__asm {
		sub esp, 0x14
		mov ecx, dwParam
		lea esi, databuffer
		push esi
		call dwCall
	}
}
/*==================================创建群聊结束=======================================*/
