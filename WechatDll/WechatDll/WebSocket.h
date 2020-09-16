#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <atlstr.h>
#include "HPSocket.h"

using namespace std;

// 初始化
void WsClientInit();
// 发送心跳
void WsClientSendHeartBeat();
// 发送数据
void WsClientSend(char *body);
// 接收数据
void WsClientRecvCallback(char *data);
// 持续连接服务端
BOOL WsClientKeepConnect(int time);
// 重新连接服务端
void WsClientReConnect(int time);
// 连接服务端
BOOL WsClientConnect();
// 关闭服务端连接
void WsClientClose();
// 发送数据包让服务端关闭连接
void WsClientSafeClose();
// 发送握手包
void WsClientHandShake();


// WebSocketClient class
class WebSocketClient : public CHttpClientListener
{
private:
	// 是否正在接收数据 - 被分包需要记录
	bool RecvState = FALSE;
	// 当前接收到的数据 - 被分包需要记录
	char *RecvData = { 0 };
	// 当前的包长度 - 被分包需要记录
	long long int RecvDataLength = 0;

	virtual EnHandleResult OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);
	virtual EnHandleResult OnConnect(ITcpClient* pSender, CONNID dwConnID) { return HR_OK; }
	
	virtual EnHttpParseResult OnHeadersComplete(IHttpClient* pSender, CONNID dwConnID);
	virtual EnHttpParseResult OnBody(IHttpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) { return HPR_OK; }
	virtual EnHttpParseResult OnMessageComplete(IHttpClient* pSender, CONNID dwConnID) { return HPR_OK; }
	virtual EnHttpParseResult OnParseError(IHttpClient* pSender, CONNID dwConnID, int iErrorCode, LPCSTR lpszErrorDesc) { return HPR_OK; }

	virtual EnHandleResult OnWSMessageHeader(IHttpClient* pSender, CONNID dwConnID, BOOL bFinal, BYTE iReserved, BYTE iOperationCode, const BYTE lpszMask[4], ULONGLONG ullBodyLen) { return HR_OK; }
	virtual EnHandleResult OnWSMessageBody(IHttpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
	virtual EnHandleResult OnWSMessageComplete(IHttpClient* pSender, CONNID dwConnID);
	virtual CStringA GetHeaderSummary(IHttpClient* pHttpClient, LPCSTR lpszSep, int iSepCount, BOOL bWithContentLength);
};
