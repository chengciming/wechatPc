#include "stdafx.h"
#include "Common.h"
#include "Md5.h"
#include "Base64.h"
#include "Sha1.h"
#include "OpCode.h"
#include "Package.h"
#include "WebSocket.h"
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <atlstr.h>
#include "HPSocket.h"
#include "Receive.h"

using namespace std;

// 当前微信ID
char ClientWechatId[33] = {0};

IHttpClient *pClient;
WebSocketClient *cClient;

Receive *ReceiveObject = NULL;

void WsClientSendHeartBeat()
{
	static const BYTE MASK_KEY[] = { 0x1, 0x2, 0x3, 0x4 };

	pClient->SendWSMessage(TRUE, 0, 0x9, MASK_KEY);
}
// 发送数据
void WsClientSend(char *body)
{
	static const BYTE MASK_KEY[] = { 0x1, 0x2, 0x3, 0x4 };

	USES_CONVERSION;
	// 数据转utf8
	CStringA strBodyA(body);

	pClient->SendWSMessage(TRUE, 0, 0x1, MASK_KEY, (BYTE*)(LPCSTR)strBodyA, strBodyA.GetLength());
}
// 接收数据
void WsClientRecvCallback(char *data)
{
	// 初始化数据包
	Package *package = new Package();
	package->SetConText(data);
	// 检查数据包必要参数
	if (package->Check() == FALSE) {
		//MessageBox(NULL, L"收到错误的数据包！", L"温馨提示：", NULL);
		return;
	}
	// 处理消息
	if (ReceiveObject == NULL) {
		ReceiveObject = new Receive;
	}
	ReceiveObject->Handle(package);
}
// 持续连接服务端
BOOL WsClientKeepConnect(int time)
{
	bool result = WsClientConnect();
	if (result == FALSE) {
		Sleep(time);
		return WsClientKeepConnect(time);
	}
	return TRUE;
}
// 重新连接服务端
void WsClientReConnect(int time)
{
	Sleep(time);
	WsClientConnect();
}
// 初始化
void WsClientInit()
{
	cClient = new WebSocketClient;
	pClient = HttpClient_Creator::Create(cClient);
	WsClientConnect();
}
// 连接服务端
BOOL WsClientConnect()
{
	pClient->Stop();
	pClient = HttpClient_Creator::Create(cClient);
	// 链接服务器配置
	string serverAddress(WEBSOCKET_IP_ADDRESS);
	string serverPort(WEBSOCKET_PORT);

	CString strAddress(serverAddress.c_str());
	CString strPort(serverPort.c_str());
	USHORT usPort = (USHORT)_ttoi(strPort);

	// 开始连接
	if (pClient->Start(strAddress, usPort) && pClient->GetConnectionID()) {
		// 发送握手包
		WsClientHandShake();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
// 关闭服务端连接
void WsClientClose()
{
	pClient->Stop();
}
// 发送数据包让服务端关闭连接
void WsClientSafeClose()
{
	static const BYTE MASK_KEY[] = { 0x1, 0x2, 0x3, 0x4 };
	// 请求服务端关闭连接
	pClient->SendWSMessage(TRUE, 0, 0x8, MASK_KEY, nullptr, 0);
}
// 发送握手包
void WsClientHandShake()
{
	Sleep(100);

	BYTE OutSHA1Buf[80];
	char key[40];
	char processId[10];

	USES_CONVERSION;

	// 生成key
	sprintf_s(key, sizeof(key), "%d", GetRand(8));
	int len = SHA1_String((unsigned char*)key, strlen(key), OutSHA1Buf);
	base64_encode(OutSHA1Buf, 20, key);

	// 组装进程ID
	sprintf_s(processId, sizeof(processId), "%d", GetCurrentProcessId());

	// 握手
	CString body;
	body += "GET / HTTP/1.1\r\n";
	body += "Host: WechatRobot.Domain\r\n";
	body += "Sec-WebSocket-Protocol: Robot\r\n";
	body += "Connection: Upgrade\r\n";
	body += "Upgrade: websocket\r\n";
	body += "Origin: WechatRobot\r\n";
	body += "Sec-WebSocket-Version: 13\r\n";
	if (strlen(ClientWechatId) > 0) {
		body += "Wechat-Id: ";
		body += ClientWechatId;
		body += "\r\n";
	}
	body += "Process-Id: ";
	body += processId;
	body += "\r\n";
	body += "Sec-WebSocket-Key: ";
	body += key;
	body += "\r\n\r\n";
	CStringA strBodyA = W2A(body);
	pClient->Send((BYTE*)(LPCSTR)strBodyA, strBodyA.GetLength());
}

// ------------------------------------------------------------------------------------------------------------- //

EnHandleResult WebSocketClient::OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
	// 重新连接服务端
	WsClientReConnect(3000);
	//MessageBoxA(NULL,"CLOSE","",NULL);
	return HR_OK;
}

// ------------------------------------------------------------------------------------------------------------- //

EnHandleResult WebSocketClient::OnWSMessageBody(IHttpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
	// 新分配内存空间
	if (this->RecvState == FALSE) {
		if ((this->RecvData = (char *)malloc((iLength + 1) * sizeof(char))) == NULL) {
			return HR_OK;
		}
		this->RecvState = TRUE;
		memcpy(this->RecvData, pData, iLength);
	}
	// 动态增加内存大小
	else {
		this->RecvData = (char *)realloc(this->RecvData, (size_t)((this->RecvDataLength + iLength + 1) * sizeof(char)));
		if (this->RecvData == NULL) {
			free(this->RecvData);
			this->RecvState = FALSE;
			return HR_OK;
		}
		// 追加内存
		for (int i = 0; i < iLength; i++) {
			this->RecvData[this->RecvDataLength + i] = pData[i];
		}
	}

	this->RecvDataLength += iLength;

	return HR_OK;
}

EnHandleResult WebSocketClient::OnWSMessageComplete(IHttpClient* pSender, CONNID dwConnID)
{
	// 整合数据
	this->RecvData[this->RecvDataLength] = '\0';
	// 处理接收到的数据
	WsClientRecvCallback(this->RecvData);
	// 释放分包储存的资源
	this->RecvDataLength = 0;
	free(this->RecvData);
	this->RecvState = FALSE;

	// 数据包的code
	BYTE iOperationCode;
	pSender->GetWSMessageState(nullptr, nullptr, &iOperationCode, nullptr, nullptr, nullptr);
	if (iOperationCode == 0x8)
		return HR_ERROR;

	return HR_OK;
}

// ------------------------------------------------------------------------------------------------------------- //

EnHttpParseResult WebSocketClient::OnHeadersComplete(IHttpClient* pSender, CONNID dwConnID)
{
	// 获取分配过来的微信ID
	LPCSTR wechatIdPtr = nullptr;
	if (pSender->GetHeader("Wechat-Id", &wechatIdPtr)) {
		CStringA wechatIdStr(wechatIdPtr);
		sprintf_s(ClientWechatId, sizeof(ClientWechatId), "%s", wechatIdStr.GetString());
	}

	// 发送准备好的消息
	//Package *package = new Package;
	//package->SetOpCode(OpCode::OPCODE_READY);
	//rapidjson::StringBuffer pack = package->GetConText();
	//WsClientSend((char*)pack.GetString());

	return HPR_OK;
}

// ------------------------------------------------------------------------------------------------------------- //
CStringA WebSocketClient::GetHeaderSummary(IHttpClient* pHttpClient, LPCSTR lpszSep, int iSepCount, BOOL bWithContentLength)
{
	CStringA SEP1;

	for (int i = 0; i < iSepCount; i++)
		SEP1 += lpszSep;

	CStringA SEP2(SEP1);
	SEP2 += lpszSep;

	CStringA strResult;

	strResult.AppendFormat("%s[Status Fields]%s", SEP1, CRLF);
	strResult.AppendFormat("%s%13s: %u%s", SEP2, "Status Code", pHttpClient->GetStatusCode(), CRLF);

	DWORD dwHeaderCount = 0;
	pHttpClient->GetAllHeaders(nullptr, dwHeaderCount);

	strResult.AppendFormat("%s[Response Headers]%s", SEP1, CRLF);

	if (dwHeaderCount == 0)
		strResult.AppendFormat("%s(no header)%s", SEP2, CRLF);
	else
	{
		unique_ptr<THeader[]> headers(new THeader[dwHeaderCount]);
		pHttpClient->GetAllHeaders(headers.get(), dwHeaderCount);

		for (DWORD i = 0; i < dwHeaderCount; i++)
			strResult.AppendFormat("%s%s: %s%s", SEP2, headers[i].name, headers[i].value, CRLF);
	}

	EnHttpUpgradeType enUpgType = pHttpClient->GetUpgradeType();
	LPCSTR lpszUpgrade = enUpgType != HUT_NONE ? "true" : "false";
	LPCSTR lpszKeepAlive = pHttpClient->IsKeepAlive() ? "true" : "false";

	strResult.AppendFormat("%s[Basic Info]%s", SEP1, CRLF);
	strResult.AppendFormat("%s%13s: %u%s", SEP2, "Status Code", pHttpClient->GetStatusCode(), CRLF);
	strResult.AppendFormat("%s%13s: %s%s", SEP2, "IsUpgrade", lpszUpgrade, CRLF);
	if (enUpgType != HUT_NONE)
		strResult.AppendFormat("%s%13s: %d%s", SEP2, "UpgradeType", enUpgType, CRLF);
	strResult.AppendFormat("%s%13s: %s%s", SEP2, "IsKeepAlive", lpszKeepAlive, CRLF);
	if (bWithContentLength)
		strResult.AppendFormat("%s%13s: %lld%s", SEP2, "ContentLength", pHttpClient->GetContentLength(), CRLF);
	strResult.AppendFormat("%s%13s: %s%s", SEP2, "ContentType", pHttpClient->GetContentType(), CRLF);

	return strResult;
}