#include "stdafx.h"
#include "resource.h"
#include "Config.h"
#include "LoginInfo.h"
#include "WechatOffset.h"
#include "Common.h"
#include "Struct.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <tchar.h> 
#include <iostream>
#include <sstream>
#include <iomanip>
#include <strstream>
//#include "Download.h"
#include "Message.h"
#include "Send.h"

using namespace std;

// 消息队列
typedef queue<WebsocketMessageStruct> DataQueue;
// 全局队列
DataQueue *SendDataQueue;

//跳回地址
DWORD jumBackAddress = 0;
//我们要提取的寄存器内容
DWORD r_esp = 0;
DWORD recvMessageCall = 0;

/*==================================消息监听开始=======================================*/
VOID RecieveMsg(DWORD esp)
{
	WebsocketMessageStruct *message = new WebsocketMessageStruct;
	
	//[[esp]]
	//信息块位置
	DWORD** msgAddress = (DWORD * *)esp;

	//消息类型[[esp]]+0x30
	message->msgType = (int)*((DWORD*)(**msgAddress + 0x30));
	/*
	switch (message->msgType) {
		case 0x01:  // 文字
			break;
		case 0x03:  // 图片
			break;
		case 0x22:  // 语音
			break;
		case 0x25:  // 好友确认
			break;
		case 0x28:  // POSSIBLEFRIEND_MSG
			break;
		case 0x2A:  // 名片
			break;
		case 0x2B:  // 视频
			break;
		case 0x2F:  // 石头剪刀布 | 表情图片
			break;
		case 0x30:  // 位置
			break;
		case 0x31:  // 共享实时位置、文件、转账、链接
			break;
		case 0x32:  // VOIPMSG
			break;
		case 0x33:  // 微信初始化
			break;
		case 0x34:  // VOIPNOTIFY
			break;
		case 0x35:  // VOIPINVITE
			break;
		case 0x3E:  // 小视频
			break;
		case 0x270F:  // SYSNOTICE
			break;
		case 0x2710:  // 红包、系统消息
			break;
		case 0x2712:  // 撤回消息
			break;
		default:  // 未知

			break;
	}
	*/

	//dc [[[esp]] + 0x16C]
	//判断是群消息还是好友消息
	wstring msgSource = _T("");
	msgSource.append(GetMsgByAddress(**msgAddress + 0x16C));
	if (msgSource.find(L"</membercount>") == wstring::npos) {
		// 收到好友消息
		message->msgSource = 0;
	} else {
		// 收到群消息
		message->msgSource = 1;
	}
	//好友消息
	if (message->msgSource == 0) {
		// 好友微信ID
		wstring wxid = GetMsgByAddress(**msgAddress + 0x40);
		sprintf_s(message->wxid, 0x100, "%s", UnicodeToUtf8((wchar_t*)wxid.c_str()));
	} else {
		// 群ID
		wstring roomId = GetMsgByAddress(**msgAddress + 0x40);
		sprintf_s(message->roomId, 0x100, "%s", UnicodeToUtf8((wchar_t*)roomId.c_str()));
		// 消息发送者
		wstring wxid = GetMsgByAddress(**msgAddress + 0x118);
		sprintf_s(message->wxid, 0x100, "%s", UnicodeToUtf8((wchar_t*)wxid.c_str()));
	}

	// 相关信息
	sprintf_s(message->msgSourceXml, 0x2048, "%s", UnicodeToUtf8((wchar_t*)msgSource.c_str()));

	// 消息内容
	wstring content = GetMsgByAddress(**msgAddress + 0x68);
	sprintf_s(message->content, 0x8092, "%s", UnicodeToUtf8((wchar_t*)content.c_str()));

	// 消息ID
	wstring msgId = GetMsgByAddress(**msgAddress + 0x12C);
	sprintf_s(message->msgId, 0x100, "%s", UnicodeToUtf8((wchar_t*)msgId.c_str()));

	// 是否自己发的消息：0=否，1=是
	message->isOwner = (int)*((DWORD*)(**msgAddress + 0x34));
	
	//_WaitingSendData *a = new _WaitingSendData;
	WebsocketMessageStruct a = *message;
	SendDataQueue->push(a);

	// 0x140  缩略图相对路径   supper-busy\FileStorage\Image\Thumb\2020-03\0adce5xxx.dat
	// 0x154  大图相对路径     supper-busy\FileStorage\Image\2020-03\0adce5d47857xxxx.dat
}



/**
 * 被写入到hook点的接收消息裸函数
 */
__declspec(naked) void RecieveMsgHook()
{
	__asm {
		// 补充被覆盖的代码
		// 0F4B28C3    B9 20FA7810     mov ecx,WeChatWi.1078FA20
		mov ecx, recvMessageCall

		//提取esp寄存器内容，放在一个变量中
		//这里使用全局变量 r_esp，将导致丢失消息，原因：多线程情况下，新数据覆盖旧数据
		//在这里，可以将esp直接压入堆栈(esp变动多)，然后在RecieveMsg函数中进行计算
		mov r_esp, esp

		//保存寄存器
		pushad
		pushf
	}

	//调用接收消息的函数
	RecieveMsg(r_esp);

	//恢复现场
	__asm
	{
		popf
		popad

		//跳回被HOOK指令的下一条指令
		jmp jumBackAddress
	}
}
/*
消费队列 - 负责发送队列的消息回服务端
*/
DWORD WINAPI QueueSendMesssage()
{
	while (TRUE) {
		if (SendDataQueue->size() <= 0) {
			Sleep(200);
			continue;
		}
		WebsocketMessageStruct message = SendDataQueue->front();
		SendDataQueue->pop();
		Send::SendWxMessage(&message);
	}
	
	return TRUE;
}
/*
注入消息监听
*/
VOID ListenMessage()
{
	// 开启一个线程，负责发送队列的消息
	SendDataQueue = new DataQueue;  // 初始化队列
	// 开启线程
	HANDLE cThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueueSendMesssage, NULL, NULL, 0);
	if (cThread != 0) {
		CloseHandle(cThread);
	}

	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();

	//WeChatWin.dll + LISTEN_MESSAGE_CALL
	int hookAddress = winAddress + LISTEN_MESSAGE_HOOK;
	recvMessageCall = winAddress + LISTEN_MESSAGE_CALL;

	//跳回的地址
	jumBackAddress = hookAddress + 5;

	//组装跳转数据
	BYTE jmpCode[5] = { 0 };
	jmpCode[0] = 0xE9;

	//新跳转指令中的数据=跳转的地址-原地址（HOOK的地址）-跳转指令的长度
	*(DWORD*)& jmpCode[1] = (DWORD)RecieveMsgHook - hookAddress - 5;

	//覆盖指令 
	// 5A665373   .  B9 B8716D5B   mov ecx,WeChatWi.5B6D71B8
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)hookAddress, jmpCode, 5, 0);
}
/*==================================消息监听结束=======================================*/

/*==================================发送文本消息开始=======================================*/
VOID SendTextMessage(wchar_t *wxid, wchar_t *message)
{
	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();

	DWORD sendCallAddress = winAddress + SEND_MESSAGE_CALL;

	// 接收人的微信ID
	wxMessageStruct wxWxid = { 0 };
	wxWxid.text = wxid;
	wxWxid.textLength = wcslen(wxid);
	wxWxid.textLen = wcslen(wxid) * 2;

	// 微信消息
	wxMessageStruct wxMessage = {0};
	wxMessage.text = message;
	wxMessage.textLength = wcslen(message);
	wxMessage.textLen = wcslen(message) * 2;
	
	// @的那个人
	//wxMessageStruct wxToWxid = {0};
	//wxMessage.text = toWxid;
	//wxMessage.textLength = wcslen(toWxid);
	//wxMessage.textLen = wcslen(toWxid) * 2;

	// 取微信ID和微信消息的内存地址
	char *pWxid = (char *)&wxWxid.text;
	char *pMsg = (char *)&wxMessage.text;
	//char *pToWxId = (char *)&wxToWxid.text;

	char buff[0x87C] = {0};

	__asm {
		pushad
		mov edx, pWxid
		mov eax, 0x0
		push 0x1
		push eax
		mov ebx, pMsg
		push ebx
		lea ecx, buff
		call sendCallAddress
		add esp, 0xC
		popad
	}
}
/*==================================发送文本消息结束=======================================*/

/*==================================发送XML名片开始=======================================*/
void SendXmlCardMessage(wchar_t* recverWxid, wchar_t* xmlData)
{
	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();
	
	SendXmlCardWxString pWxid = { 0 };
	pWxid.pStr = recverWxid;
	pWxid.strLen = wcslen(recverWxid);
	pWxid.strMaxLen = wcslen(recverWxid) * 2;

	SendXmlCardWxString pXml = { 0 };
	pXml.pStr = xmlData;
	pXml.strLen = wcslen(xmlData);
	pXml.strMaxLen = wcslen(xmlData) * 2;

	char* asmWxid = (char *)&pWxid.pStr;
	char* asmXml = (char *)&pXml.pStr;
	char buff[0x20C] = { 0 };
	DWORD callAdd = winAddress + WX_SEND_XML_CARD;
	
	__asm {
		pushad
		mov eax, asmXml
		push 0x2A
		mov edx, asmWxid
		push 0x0
		push eax
		lea ecx, buff
		call callAdd
		add esp, 0xC
		popad
	}
}
/*==================================发送XML名片结束=======================================*/


/*==================================发送图片开始=======================================*/
void SendImageMessage(wchar_t* wxid, wchar_t* filepath)
{
	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();

	//组装微信ID的数据结构
	SendImageWxidStruct imagewxid;
	imagewxid.pWxid = wxid;
	imagewxid.length = wcslen(wxid);
	imagewxid.maxLength = wcslen(wxid) * 2;

	//组装文件路径的数据结构
	SendImageFileStruct imagefilepath;
	imagefilepath.filePath = filepath;
	imagefilepath.length = wcslen(filepath);
	imagefilepath.maxLength = wcslen(filepath) * 2;

	char buff[0x45C] = { 0 };

	DWORD dwCall1 = winAddress + WX_SEND_IMAGE_CALL1;
	DWORD dwCall2 = winAddress + WX_SEND_IMAGE_CALL2;

	//微信发送图片调用了GidCreateBimapFileCM 之后图片一直被占用 无法删除 patch掉下面的代码
	unsigned char oldcode[5] = { 0 };
	// {0x31,0xC0} = xor eax,eax
	// {0xC3} = return
	unsigned char fix[5] = { 0x31,0xC0,0xC3,0x90,0x90 };
	DWORD dwPathcAddr = winAddress + WX_SEND_IMAGE_PATCH_ADDR;
	//修改代码段属性
	DWORD dwOldAttr = 0;
	VirtualProtect((LPVOID)dwPathcAddr, 5, PAGE_EXECUTE_READWRITE, &dwOldAttr);
	//先保存原有指令
	memcpy(oldcode, (LPVOID)dwPathcAddr, 5);
	//再Patch掉
	memcpy((LPVOID)dwPathcAddr, fix, 5);
	__asm
	{
		lea ebx, imagefilepath;
		push ebx;
		lea eax, imagewxid;
		push eax;
		lea eax, buff;
		push eax;
		push 0x0;
		call dwCall1;
		add esp, 0x4;
		mov ecx, eax;
		call dwCall2;
	}
	//调用完之后恢复
	memcpy((LPVOID)dwPathcAddr, oldcode, 5);
	//恢复属性
	VirtualProtect((LPVOID)dwPathcAddr, 5, dwOldAttr, &dwOldAttr);
}
/*==================================发送图片结束=======================================*/

/*==================================发送xml文章开始=======================================*/
void SendXmlMessage(DWORD type, wchar_t* recverWxid, wchar_t* fromWxid, wchar_t* filePath, wchar_t* xmlData)
{
	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();

	SendXmlWxidStruct wxid = { 0 };
	wxid.wxid = recverWxid;
	wxid.wxidLen = wcslen(recverWxid);
	wxid.wxidMaxLen = wcslen(recverWxid) * 2;

	SendXmlFromWxidStruct fWxid = { 0 };
	fWxid.wxid = fromWxid;
	fWxid.wxidLen = wcslen(fromWxid);
	fWxid.wxidMaxLen = wcslen(fromWxid) * 2;

	SendXmlImageStruct file = { 0 };
	file.filePath = filePath;
	file.pathLen = wcslen(filePath);
	file.pathMaxLen = wcslen(filePath) * 2;

	SendXmlStruct xml = { 0 };
	xml.content = xmlData;
	xml.strLen = wcslen(xmlData);
	xml.strMaxLen = wcslen(xmlData) * 2;

	char buff1[0x88] = { 0 };
	char buff2[0x2D8] = { 0 };
	char buff3[0xF4] = { 0 };
	char buff4[0x60] = { 0 };

	DWORD call1 = winAddress + WX_SEND_XML_CALL1;
	DWORD call2 = winAddress + WX_SEND_XML_CALL2;
	DWORD call3 = winAddress + WX_SEND_XML_CALL3;
	DWORD call4 = winAddress + WX_SEND_XML_CALL4;
	DWORD call5 = winAddress + WX_SEND_XML_CALL5;
	DWORD call6 = winAddress + WX_SEND_XML_CALL6;
	DWORD param = winAddress + WX_SEND_XML_PARAM;
	DWORD call7 = winAddress + WX_SEND_XML_CALL7;
	int typeInt = type;
	__asm {
		pushad
		lea ebx, wxid
		push typeInt  // 0x5是链接分享，0x21是小程序分享
		lea eax, buff1
		push eax
		lea eax, file
		push eax
		lea eax, xml
		push eax
		push ebx
		lea edx, fWxid
		lea ecx, buff2
		call call1
		add esp, 0x14
		lea eax, buff3
		xor bl, bl
		push eax
		lea ecx, buff2
		call call2
		mov edx, eax
		lea ecx, buff4
		call call3
		lea ecx, buff3
		call call4
		lea ecx, buff4
		call call5
		mov ecx, 0x1
		cmovne ebx, ecx
		lea ecx, buff1
		call call6
		lea ecx, file
		call call6
		push param
		push param
		mov dl, bl
		lea ecx, buff2
		call call7
		add esp, 0x8
		popad
	}
}
/*==================================发送xml文章结束=======================================*/

/*==================================发送附件开始=======================================*/
void SendFileMessage(wchar_t* wxid, wchar_t* filepath)
{
	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();
	
	DWORD dwCall1 = winAddress + WX_SEND_ATTACH_CALL1;
	DWORD dwCall2 = winAddress + WX_SEND_ATTACH_CALL2;
	DWORD dwCall3 = winAddress + WX_SEND_ATTACH_CALL3;
	DWORD dwCall4 = winAddress + WX_SEND_ATTACH_CALL4;
	DWORD dwParams = winAddress + WX_SEND_ATTACH_PARAM;

	char buff[0x45C] = { 0 };
	SendAttachWxidStruct wxidStruct = { 0 };
	wxidStruct.str = wxid;
	wxidStruct.strLen = wcslen(wxid);
	wxidStruct.maxLen = wcslen(wxid) * 2;

	SendAttachFileStruct filePathStruct = { 0 };
	filePathStruct.str = filepath;
	filePathStruct.strLen = wcslen(filepath);
	filePathStruct.maxLen = wcslen(filepath) * 2;

	char* pFilePath = (char*)&filePathStruct.str;
	char* pWxid = (char*)&wxidStruct.str;

	__asm {
		pushad
		push dword ptr ss : [ebp - 0x5C];
		sub esp, 0x14;
		mov ecx, esp;
		push - 0x1;
		push dwParams;
		call dwCall1;
		sub esp, 0x14;
		mov ecx, esp;
		push pFilePath;
		call dwCall2;
		sub esp, 0x14;
		mov ecx, esp;
		push pWxid;
		call dwCall2;
		lea eax, buff;
		push eax;
		call dwCall3;
		mov ecx, eax;
		call dwCall4;
		popad
	}
}
/*==================================发送附件结束=======================================*/


