#pragma once
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <atlstr.h>
#include <cstdlib>
#include <ctime>
#include "HPSocket.h"


typedef struct SocketList
{
	// 进程ID
	int processId = 0;
	// 链接ID
	CONNID dwConnID = 0;
	// 微信ID
	char wechatId[33] = {0};

	// 是否正在接收数据 - 被分包需要记录
	bool RecvState = FALSE;
	// 当前接收到的数据 - 被分包需要记录
	char *RecvData = { 0 };
	// 当前的包长度 - 被分包需要记录
	long long int RecvDataLength = 0;

	SocketList *next;
}SocketList;

// 创建列表
void CreateSocketList();
// 获取所有微信ID列表，字符串，用逗号隔开
CString GetAllSocketWechatIdList();
// 删除Socket
void DeleteSocket(CONNID dwConnID);
// 新添加一个进程，绑定微信ID
void AddWechatProcess(int processId, char *wechatId);
// 新添加一个Socket
void AddSocket(CONNID dwConnID, int processId);
// 获取socket的微信客户端ID - 进程ID
void GetSocketWechatIdByProcessId(int processId, char *wechatId);
// 获取socket的微信客户端ID
void GetSocketWechatId(CONNID dwConnID, char *wechatId);
// 获取socket的微信客户端ID
CONNID GetSocketConnectId(char *wechatId);
// 获取socket的微信客户端ID
bool GetSocketRecvState(CONNID dwConnID);
// 获取socket的消息数据储存
void GetSocketRecvData(CONNID dwConnID, char *recvData);
// 添加消息数据
void AddSocketRecvData(CONNID dwConnID, const BYTE* pData, int iLength);
// 获取socket的消息数据储存长度
long long int GetSocketRecvDataLength(CONNID dwConnID);
// 释放分包储存的资源
void FreeSocketRecvData(CONNID dwConnID);