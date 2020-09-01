#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <atlstr.h>
#include "HPSocket.h"


// 发送数据
void WsServerSend(CONNID dwConnID, char *body);
// 接收数据
void WsServerRecvCallback(CONNID dwConnID, char *data);
// 监听端口
BOOL WsServerListen();
// 初始化
void WsServerInit();
// 关闭监听
void WsServerClose();

/* Listener Class */
class WebSocketServer : public CHttpServerListener
{
public:
	virtual EnHandleResult OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);
	virtual EnHandleResult OnConnect(ITcpServer* pSender, CONNID dwConnID) { return HR_OK; }

	virtual EnHttpParseResult OnHeadersComplete(IHttpServer* pSender, CONNID dwConnID) { return HPR_OK; }
	virtual EnHttpParseResult OnBody(IHttpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength) { return HPR_OK; }
	virtual EnHttpParseResult OnMessageComplete(IHttpServer* pSender, CONNID dwConnID) { return HPR_OK; }
	virtual EnHttpParseResult OnParseError(IHttpServer* pSender, CONNID dwConnID, int iErrorCode, LPCSTR lpszErrorDesc) { return HPR_OK; }

	virtual EnHandleResult OnPrepareListen(ITcpServer* pSender, SOCKET soListen) { return HR_IGNORE; }
	virtual EnHandleResult OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient) { return HR_IGNORE; }
	virtual EnHandleResult OnHandShake(ITcpServer* pSender, CONNID dwConnID) { return HR_IGNORE; }
	virtual EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, int iLength) { return HR_IGNORE; }
	virtual EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength) { return HR_IGNORE; }
	virtual EnHandleResult OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength) { return HR_IGNORE; }
	virtual EnHandleResult OnShutdown(ITcpServer* pSender) { return HR_IGNORE; }

	virtual EnHttpParseResult OnMessageBegin(IHttpServer* pSender, CONNID dwConnID) { return HPR_OK; }
	virtual EnHttpParseResult OnRequestLine(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszMethod, LPCSTR lpszUrl) { return HPR_OK; }
	virtual EnHttpParseResult OnStatusLine(IHttpServer* pSender, CONNID dwConnID, USHORT usStatusCode, LPCSTR lpszDesc) { return HPR_OK; }
	virtual EnHttpParseResult OnHeader(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszName, LPCSTR lpszValue) { return HPR_OK; }
	virtual EnHttpParseResult OnChunkHeader(IHttpServer* pSender, CONNID dwConnID, int iLength) { return HPR_OK; }
	virtual EnHttpParseResult OnChunkComplete(IHttpServer* pSender, CONNID dwConnID) { return HPR_OK; }
	virtual EnHttpParseResult OnUpgrade(IHttpServer* pSender, CONNID dwConnID, EnHttpUpgradeType enUpgradeType);
	
	virtual EnHandleResult OnWSMessageHeader(IHttpServer* pSender, CONNID dwConnID, BOOL bFinal, BYTE iReserved, BYTE iOperationCode, const BYTE lpszMask[4], ULONGLONG ullBodyLen) { return HR_OK; }
	virtual EnHandleResult OnWSMessageBody(IHttpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
	virtual EnHandleResult OnWSMessageComplete(IHttpServer* pSender, CONNID dwConnID);
private:
	CStringA GetHeaderSummary(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszSep = "  ", int iSepCount = 0, BOOL bWithContentLength = TRUE);
};
