#include "stdafx.h"
#include "Config.h"
#include "Common.h"
#include "SocketStruct.h"
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <atlstr.h>
#include <cstdlib>
#include <ctime>
#include "HPSocket.h"


// 全局储存Socket列表
SocketList *socketList;

// 创建列表
void CreateSocketList()     /*链表每一个节点都是指向  SocketListPointStruct结构的指针，所以返回值是SocketListPointStruct *,n是指创建的链表的节点数目*/
{
	SocketList *normal, *next;/*创建头节点，其他节点，和尾节点*/
	next = new SocketList;

	/*分配内存*/
	socketList = next;/*最开始最后一个节点就是头节点，注意因为通过指针可以直接对地址上的东西进行操作，此时end和head指向同一个地址，对end所指向地址进行操作，等同于对head地址所做的操作*/
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		normal = new SocketList;

		/* 往新节点存入数据，注意我们只给后面的节点存入数据，head不存数据*/
		next->next = normal;/*往end后增添新节点*/
		next = next->next;
	}
	next->next = NULL;/*链表的最后指向一个新地址*/
}
// 获取所有微信ID列表，字符串，用逗号隔开
CString GetAllSocketWechatIdList()
{
	CString list;

	int number = 0;
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p && strlen(p->wechatId) > 0) {
			if (number == 0) {
				list += p->wechatId;
			}
			else {
				list += ",";
				list += p->wechatId;
			}
			number++;
		}
		if (p->next) {
			p = p->next;
		}
	}
	return list;
}
// 删除Socket
void DeleteSocket(CONNID dwConnID)
{
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p && p->dwConnID == dwConnID) {
			if (p->RecvDataLength > 0) {
				// 释放分包储存的资源
				p->RecvDataLength = 0;
				free(p->RecvData);
				p->RecvState = FALSE;
			}
			p->dwConnID = 0;
			sprintf_s(p->wechatId, 33, "%s", "");
			p->next = p->next->next;
			break;
		}
		if (p->next) {
			p = p->next;
		}
	}
}
// 新添加一个进程，绑定微信ID
void AddWechatProcess(int processId, char *wechatId) 
{
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p != NULL && p->processId == 0) {
			p->processId = processId;
			sprintf_s(p->wechatId, 33, "%s", wechatId);
			break;
		}
		if (p->next) {
			p = p->next;
		}
	}
}
// 新添加一个Socket
void AddSocket(CONNID dwConnID, int processId)
{
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p != NULL && p->processId == processId) {
			p->dwConnID = dwConnID;
			break;
		}
		if (p->next) {
			p = p->next;
		}
	}
}
// 获取socket的微信客户端ID - 进程ID
void GetSocketWechatIdByProcessId(int processId, char *wechatId) 
{
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p && p->processId == processId) {
			sprintf_s(wechatId, 33, "%s", p->wechatId);
			break;
		}
		if (p->next) {
			p = p->next;
		}
	}
}
// 获取socket的微信客户端ID
void GetSocketWechatId(CONNID dwConnID, char *wechatId)
{
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p && p->dwConnID == dwConnID) {
			sprintf_s(wechatId, 33, "%s", p->wechatId);
			break;
		}
		if (p->next) {
			p = p->next;
		}
	}
}
// 获取socket的微信客户端ID
CONNID GetSocketConnectId(char *wechatId)
{
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p && !strncmp(p->wechatId, wechatId, 33)) {
			return p->dwConnID;
		}
		if (p->next) {
			p = p->next;
		}
	}
	return 0;
}
// 获取socket的微信客户端ID
bool GetSocketRecvState(CONNID dwConnID)
{
	bool RecvState = FALSE;
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p && p->dwConnID == dwConnID) {
			RecvState = p->RecvState;
			break;
		}
		if (p->next) {
			p = p->next;
		}
	}
	return RecvState;
}
// 获取socket的消息数据储存
void GetSocketRecvData(CONNID dwConnID, char *recvData)
{
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p && p->dwConnID == dwConnID) {
			p->RecvData[p->RecvDataLength] = '\0';
			sprintf_s(recvData, (size_t)(p->RecvDataLength + 1), "%s", p->RecvData);
			return;
		}
		if (p->next) {
			p = p->next;
		}
	}
	return;
}
// 添加消息数据
void AddSocketRecvData(CONNID dwConnID, const BYTE* pData, int iLength)
{
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p && p->dwConnID == dwConnID) {
			// 新分配内存空间
			if (p->RecvState == FALSE) {
				if ((p->RecvData = (char *)malloc((iLength + 1) * sizeof(char))) == NULL) {
					return;
				}
				p->RecvState = TRUE;
				memcpy(p->RecvData, pData, iLength);
			}
			// 动态增加内存大小
			else {
				p->RecvData = (char *)realloc(p->RecvData, (size_t)((p->RecvDataLength + iLength + 1) * sizeof(char)));
				if (p->RecvData == NULL) {
					free(p->RecvData);
					p->RecvState = FALSE;
					return;
				}
				// 追加内存
				for (int i = 0; i < iLength; i++) {
					p->RecvData[p->RecvDataLength + i] = pData[i];
				}
			}
			p->RecvDataLength += iLength;

			break;
		}
		if (p->next) {
			p = p->next;
		}
	}
}
// 获取socket的消息数据储存长度
long long int GetSocketRecvDataLength(CONNID dwConnID)
{
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p && p->dwConnID == dwConnID) {
			return p->RecvDataLength;
		}
		if (p->next) {
			p = p->next;
		}
	}
	return 0;
}
// 释放分包储存的资源
void FreeSocketRecvData(CONNID dwConnID)
{
	SocketList *p = socketList;
	for (int i = 0; i < WEBSOCKET_LISTEN_MAX_LINK; i++) {
		if (p && p->dwConnID == dwConnID) {
			// 释放分包储存的资源
			p->RecvDataLength = 0;
			free(p->RecvData);
			p->RecvState = FALSE;
			break;
		}
		if (p->next) {
			p = p->next;
		}
	}
}