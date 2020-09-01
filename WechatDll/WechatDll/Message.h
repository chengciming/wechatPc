#pragma once
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <tchar.h> 
#include <iostream>
#include <sstream>
#include <iomanip>
#include <strstream>
#include <queue>
#include "Send.h"

using namespace std;

// 消息结构体
struct wxMessageStruct
{
	wchar_t * text;
	DWORD textLength;
	DWORD textLen;
};
// 发送XML消息结构
struct SendXmlCardWxString
{
	wchar_t * pStr;
	DWORD strLen;
	DWORD strMaxLen;
	char fillbuff[0x8] = { 0 };
};
//发送图片的结构体
struct SendImageWxidStruct
{
	wchar_t* pWxid;
	DWORD length = 0;
	DWORD maxLength = 0;
};
//发送图片的结构体
struct SendImageFileStruct
{
	wchar_t* filePath;
	DWORD length = 0;
	DWORD maxLength = 0;
	char fillbuff[0x18] = { 0 };
};
//发送xml文章内容结构体
struct SendXmlStruct
{
	wchar_t* content;
	DWORD strLen;
	DWORD strMaxLen;
	char fillbuff[0x8] = { 0 };
};
//发送xml文章接收人微信ID结构体
struct SendXmlWxidStruct
{
	wchar_t* wxid;
	DWORD wxidLen;
	DWORD wxidMaxLen;
	char file[0x8] = { 0 };
};
//发送xml文章发送人微信ID结构体
struct SendXmlFromWxidStruct
{
	wchar_t* wxid;
	DWORD wxidLen;
	DWORD wxidMaxLen;
	char file[0x8] = { 0 };
};
// 发送xml文章本地图片结构体
struct SendXmlImageStruct
{
	wchar_t* filePath;
	DWORD pathLen;
	DWORD pathMaxLen;
	char file[0x18] = { 0 };
};
//发送附件文件微信ID
struct SendAttachWxidStruct
{
	wchar_t* str;
	DWORD strLen = 0;
	DWORD maxLen = 0;
};
//发送附件文件结构体
struct SendAttachFileStruct
{
	wchar_t* str;
	DWORD strLen = 0;
	DWORD maxLen = 0;
	char file[0x18] = { 0 };
};

// 发送文本消息
VOID SendTextMessage(wchar_t *wxid, wchar_t *message);
// 监听消息
VOID ListenMessage();
// 发送XML名片
void SendXmlCardMessage(wchar_t* RecverWxid, wchar_t* xmlData);
// 发送图片
void SendImageMessage(wchar_t* wxid, wchar_t* filepath);
// 发送xml
void SendXmlMessage(DWORD type, wchar_t* recverWxid, wchar_t* fromWxid, wchar_t* filePath, wchar_t* xmlData);
// 发送附件文件
void SendFileMessage(wchar_t* wxid, wchar_t* filepath);