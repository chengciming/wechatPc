#include "stdafx.h"
#include "Config.h"
#include "Common.h"
#include "Sha1.h"
#include "Base64.h"
#include "BufferPtr.h"
#include "OpCode.h"
#include "Package.h"
#include "SocketStruct.h"
#include "WebSocketClient.h"
#include "WebSocketListen.h"
#include "WechatService.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <atlstr.h>
#include "HPSocket.h"


IHttpServer* pServer;
WebSocketServer *cServer;

// 发送数据
void WsServerSend(CONNID dwConnID, char *body)
{
	// 转义
	size_t bodyLength = strlen(body);
	// 初始化消息
	CBufferPtr* pBuffer = new CBufferPtr(bodyLength);
	pBuffer->Copy((BYTE*)body, bodyLength);
	// 获取标识
	BYTE iReserved, iOperationCode;
	pServer->GetWSMessageState(dwConnID, nullptr, &iReserved, &iOperationCode, nullptr, nullptr, nullptr);
	// 发送消息
	pServer->SendWSMessage(dwConnID, TRUE, iReserved, iOperationCode, pBuffer->Ptr(), (int)pBuffer->Size());
	// 释放消息资源
	pBuffer->Free();
}
// 接收数据
void WsServerRecvCallback(CONNID dwConnID, char *data)
{
	// 初始化数据包
	Package *package = new Package();
	package->SetConText(data);
	// 检查数据包必要参数
	/* 这里不做判断，因为dll端没有appid */
	/*
	if (package->Check() == FALSE) {
		MessageBox(NULL, L"收到错误的数据包！", L"温馨提示：", NULL);
		return;
	}
	*/
	int opCode = package->GetOpCode();

	// 微信退出了，关闭进程
	if (opCode == OpCode::OPCODE_WECHAT_QUIT) {
		Value *pBody = package->GetBody();
		Value &body = *pBody;
		if (!body.HasMember("processId") || !body.HasMember("loginStatus")) {
			return;
		}
		Value& processId = body["processId"];
		Value& loginStatus = body["loginStatus"];

		// 未退出不操作
		if (loginStatus.GetInt() != 0) {
			return;
		}

		// 关闭进程
		CloseProcess(processId.GetInt());
		// 修改发回服务器端通知退出登录
		body.RemoveMember("processId");
		package->SetBody(body);
	}

	// 赋值微信客户端ID
	char wechatId[33] = { 0 };
	GetSocketWechatId(dwConnID, wechatId);

	if (strlen(wechatId)) {
		package->SetWechatId(wechatId);
	}
	rapidjson::StringBuffer context = package->GetConText();
	const char *json = context.GetString();
	// 发送给服务器端
	WsClientSend((char *)json);
}
// 监听端口
BOOL WsServerListen()
{
	CString strBindAddr = WEBSOCKET_LISTEN_IP_ADDRESS;
	const USHORT port = atoi(WEBSOCKET_LISTEN_IP_PORT);
	// 启动监听
	if (!pServer->Start(strBindAddr, port)) {
		MessageBox(NULL, L"启动Websocket监听失败！", L"温馨提示：", NULL);
		return FALSE;
	}
	return TRUE;
}
// 初始化
void WsServerInit()
{
	// 创建socket储存列表
	CreateSocketList();
	// 初始化websocket
	cServer = new WebSocketServer;
	pServer = HttpServer_Creator::Create(cServer);
	// 开始监听端口
	WsServerListen();
}
// 关闭监听
void WsServerClose()
{
	pServer->Stop();
}

// ------------------------------------------------------------------------------------------------------------- //

EnHandleResult WebSocketServer::OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
	// 删除Socket储存的信息
	DeleteSocket(dwConnID);

	CBufferPtr* pBuffer = nullptr;
	pSender->GetConnectionExtra(dwConnID, (PVOID*)&pBuffer);

	if (pBuffer) delete pBuffer;

	return HR_OK;
}

// ------------------------------------------------------------------------------------------------------------- //

EnHttpParseResult WebSocketServer::OnUpgrade(IHttpServer* pSender, CONNID dwConnID, EnHttpUpgradeType enUpgradeType)
{
	if (enUpgradeType == HUT_HTTP_TUNNEL) {
		pSender->SendResponse(dwConnID, HSC_OK, "Connection Established");
	}
	else if (enUpgradeType == HUT_WEB_SOCKET) {
		// 获取进程ID，并且储存socket链接
		LPCSTR processIdPtr = nullptr;
		if (!pSender->GetHeader(dwConnID, "Process-Id", &processIdPtr)) {
			return HPR_ERROR;
		}
		CStringA processIdStr(processIdPtr);
		int processId = atoi(processIdStr.GetString());
		// 获取微信ID，短线后重新链接的
		LPCSTR wechatIdPtr = nullptr;
		if (pSender->GetHeader(dwConnID, "Wechat-Id", &wechatIdPtr)) {
			CStringA wechatIdStr(wechatIdPtr);
			AddWechatProcess(processId, (char *)wechatIdStr.GetString());
		}

		// 添加进程连接
		AddSocket(dwConnID, processId);

		// 构造回复信息
		int iHeaderCount = 2;
		THeader header[] = { {"Connection", "Upgrade"},
							{"Upgrade", "WebSocket"},
							{nullptr, nullptr},
							{nullptr, nullptr},
							{nullptr, nullptr} };

		LPCSTR lpszAccept = nullptr;

		if (!pSender->GetHeader(dwConnID, "Sec-WebSocket-Key", &lpszAccept))
			return HPR_ERROR;

		CStringA strAccept;

		BYTE OutSHA1Buf[80];
		char key[40];

		CStringA strKey(lpszAccept);
		strKey.Append("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");

		// 生成key
		int len = SHA1_String((unsigned char*)strKey.GetString(), strKey.GetLength(), OutSHA1Buf);
		base64_encode(OutSHA1Buf, 20, key);
		strAccept = key;

		header[2].name = "Sec-WebSocket-Accept";
		header[2].value = strAccept;
		++iHeaderCount;

		CStringA strFirst;
		LPCSTR lpszProtocol = nullptr;

		if (pSender->GetHeader(dwConnID, "Sec-WebSocket-Protocol", &lpszProtocol)) {
			int i = 0;
			CStringA strProtocol(lpszProtocol);
			strFirst = strProtocol.Tokenize(", ", i);

			if (!strFirst.IsEmpty()) {
				header[3].name = "Sec-WebSocket-Protocol";
				header[3].value = strFirst;
				++iHeaderCount;
			}
		}
		
		// 加上微信ID
		char wechatId[33] = { 0 };
		GetSocketWechatIdByProcessId(processId, wechatId);
		header[4].name = "Wechat-Id";
		header[4].value = wechatId;
		++iHeaderCount;

		pSender->SendResponse(dwConnID, HSC_SWITCHING_PROTOCOLS, nullptr, header, iHeaderCount);
		pSender->SetConnectionExtra(dwConnID, new CBufferPtr);
	}

	return HPR_OK;
}

// ------------------------------------------------------------------------------------------------------------- //

EnHandleResult WebSocketServer::OnWSMessageBody(IHttpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
	// 储存数据
	AddSocketRecvData(dwConnID, pData, iLength);

	return HR_OK;
}

EnHandleResult WebSocketServer::OnWSMessageComplete(IHttpServer* pSender, CONNID dwConnID)
{
	BYTE iOperationCode;
	pSender->GetWSMessageState(dwConnID, nullptr, nullptr, &iOperationCode, nullptr, nullptr, nullptr);

	// 关闭连接
	if (iOperationCode == 0x8) {
		pSender->Disconnect(dwConnID);
		return HR_OK;
	}
	
	// Ping
	if (iOperationCode == 0x9) {
		return HR_OK;
	}

	// 整合数据
	long long int RecvDataLength = GetSocketRecvDataLength(dwConnID);
	char *data;
	if ((data = (char *)malloc((size_t)(RecvDataLength + 1) * sizeof(char))) == NULL) {
		return HR_OK;
	}
	GetSocketRecvData(dwConnID, data);
	// 处理接收到的数据
	WsServerRecvCallback(dwConnID, data);
	free(data);
	// 释放分包储存的资源
	FreeSocketRecvData(dwConnID);

	return HR_OK;
}

// ------------------------------------------------------------------------------------------------------------- //

CStringA WebSocketServer::GetHeaderSummary(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszSep, int iSepCount, BOOL bWithContentLength)
{
	CStringA SEP1;

	for (int i = 0; i < iSepCount; i++)
		SEP1 += lpszSep;

	CStringA SEP2(SEP1);
	SEP2 += lpszSep;

	CStringA strResult;

	USHORT usUrlFieldSet = pSender->GetUrlFieldSet(dwConnID);

	strResult.AppendFormat("%s[URL Fields]%s", SEP1, CRLF);
	strResult.AppendFormat("%s%8s: %s%s", SEP2, "SCHEMA", pSender->GetUrlField(dwConnID, HUF_SCHEMA), CRLF);
	strResult.AppendFormat("%s%8s: %s%s", SEP2, "HOST", pSender->GetUrlField(dwConnID, HUF_HOST), CRLF);
	strResult.AppendFormat("%s%8s: %s%s", SEP2, "PORT", pSender->GetUrlField(dwConnID, HUF_PORT), CRLF);
	strResult.AppendFormat("%s%8s: %s%s", SEP2, "PATH", pSender->GetUrlField(dwConnID, HUF_PATH), CRLF);
	strResult.AppendFormat("%s%8s: %s%s", SEP2, "QUERY", pSender->GetUrlField(dwConnID, HUF_QUERY), CRLF);
	strResult.AppendFormat("%s%8s: %s%s", SEP2, "FRAGMENT", pSender->GetUrlField(dwConnID, HUF_FRAGMENT), CRLF);
	strResult.AppendFormat("%s%8s: %s%s", SEP2, "USERINFO", pSender->GetUrlField(dwConnID, HUF_USERINFO), CRLF);

	DWORD dwHeaderCount = 0;
	pSender->GetAllHeaders(dwConnID, nullptr, dwHeaderCount);

	strResult.AppendFormat("%s[Request Headers]%s", SEP1, CRLF);

	if (dwHeaderCount == 0)
		strResult.AppendFormat("%s(no header)%s", SEP2, CRLF);
	else
	{
		unique_ptr<THeader[]> headers(new THeader[dwHeaderCount]);
		//VERIFY(pSender->GetAllHeaders(dwConnID, headers.get(), dwHeaderCount));
		pSender->GetAllHeaders(dwConnID, headers.get(), dwHeaderCount);

		for (DWORD i = 0; i < dwHeaderCount; i++)
			strResult.AppendFormat("%s%s: %s%s", SEP2, headers[i].name, headers[i].value, CRLF);
	}

	DWORD dwCookieCount = 0;
	pSender->GetAllCookies(dwConnID, nullptr, dwCookieCount);

	strResult.AppendFormat("%s[Cookies]%s", SEP1, CRLF);

	if (dwCookieCount == 0)
		strResult.AppendFormat("%s(no cookie)%s", SEP2, CRLF);
	else
	{
		unique_ptr<TCookie[]> cookies(new TCookie[dwCookieCount]);
		//VERIFY(pSender->GetAllCookies(dwConnID, cookies.get(), dwCookieCount));
		pSender->GetAllCookies(dwConnID, cookies.get(), dwCookieCount);

		for (DWORD i = 0; i < dwCookieCount; i++)
			strResult.AppendFormat("%s%s: %s%s", SEP2, cookies[i].name, cookies[i].value, CRLF);
	}

	EnHttpUpgradeType enUpgType = pSender->GetUpgradeType(dwConnID);
	LPCSTR lpszUpgrade = enUpgType != HUT_NONE ? "true" : "false";
	LPCSTR lpszKeepAlive = pSender->IsKeepAlive(dwConnID) ? "true" : "false";

	strResult.AppendFormat("%s[Basic Info]%s", SEP1, CRLF);
	strResult.AppendFormat("%s%13s: %s%s", SEP2, "Method", pSender->GetMethod(dwConnID), CRLF);
	strResult.AppendFormat("%s%13s: %s%s", SEP2, "IsUpgrade", lpszUpgrade, CRLF);
	if (enUpgType != HUT_NONE)
		strResult.AppendFormat("%s%13s: %d%s", SEP2, "UpgradeType", enUpgType, CRLF);
	strResult.AppendFormat("%s%13s: %s%s", SEP2, "IsKeepAlive", lpszKeepAlive, CRLF);
	if (bWithContentLength)
		strResult.AppendFormat("%s%13s: %lld%s", SEP2, "ContentLength", pSender->GetContentLength(dwConnID), CRLF);
	strResult.AppendFormat("%s%13s: %s%s", SEP2, "ContentType", pSender->GetContentType(dwConnID), CRLF);

	return strResult;
}
