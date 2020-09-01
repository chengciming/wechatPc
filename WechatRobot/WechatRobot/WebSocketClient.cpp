#include "stdafx.h"
#include "Common.h"
#include "WechatMultiOpen.h"
#include "Sha1.h"
#include "Base64.h"
#include "Package.h"
#include "SocketStruct.h"
#include "WechatService.h"
#include "WebSocketClient.h"
#include "WebSocketListen.h"
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <atlstr.h>
#include "HPSocket.h"

using namespace std;

IHttpClient *pClient;
WebSocketClient *cClient;

// 发送数据
void WsClientSend(char *body)
{
	static const BYTE MASK_KEY[] = { 0x1, 0x2, 0x3, 0x4 };

	USES_CONVERSION;
	// 数据转utf8
	CStringA strBodyA(body);
	if (pClient) {
		pClient->SendWSMessage(TRUE, 0, 0x1, MASK_KEY, (BYTE*)(LPCSTR)strBodyA, strBodyA.GetLength());
	}
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

	int opCode = package->GetOpCode();
	char *wechatId = package->GetWechatId();
	if (opCode == OpCode::OPCODE_WECHAT_OPEN && strlen(wechatId) > 0) {  // 新开一个微信客户端
		// 开启微信
		int processId = StartWechat();
		// 绑定进程与微信ID
		if (processId > 0) {
			AddWechatProcess(processId, wechatId);
		}
		return;
	}

	// 找出对应的微信客户端
	CONNID dwConnID = GetSocketConnectId(wechatId);
	// 发送给微信客户端
	if (dwConnID != 0) {
		WsServerSend(dwConnID, data);
	}
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
	string serverAddress = ReadConfig("wesocket_server_address");
	string serverPort = ReadConfig("wesocket_server_port");
	if (serverAddress.length() <= 0) {
		serverAddress = WEBSOCKET_DEFAULT_SERVER_IP_ADDRESS;
	}
	if (serverPort.length() <= 0) {
		serverPort = WEBSOCKET_DEFAULT_SERVER_IP_PORT;
	}

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

	USES_CONVERSION;

	// 生成key
	sprintf_s(key, 40, "%d", GetRand(8));
	int len = SHA1_String((unsigned char*)key, strlen(key), OutSHA1Buf);
	base64_encode(OutSHA1Buf, 20, key);

	// 握手
	CString body;
	body += "GET / HTTP/1.1\r\n";
	body += "Host: WechatRobot.Domain\r\n";
	body += "Sec-WebSocket-Protocol: Robot\r\n";
	body += "Connection: Upgrade\r\n";
	body += "Upgrade: websocket\r\n";
	body += "Origin: WechatRobot\r\n";
	body += "Sec-WebSocket-Version: 13\r\n";
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
// 告诉服务器端准备好了，顺便上报已经连接上来的终端
DWORD WINAPI SendReady()
{
	

	return TRUE;
}
EnHttpParseResult WebSocketClient::OnHeadersComplete(IHttpClient* pSender, CONNID dwConnID)
{
	// 开一个线程等待告诉服务器端准备好了
	//HANDLE cThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SendReady, NULL, NULL, 0);
	//if (cThread != 0) {
	//	CloseHandle(cThread);
	//}

	// 回复客户端已经准备好的数据包
	Package *package = new Package;
	package->SetOpCode(OpCode::OPCODE_READY);

	// 获取微信客户端列表，上报
	CString wechatIdList = GetAllSocketWechatIdList();
	USES_CONVERSION;
	CStringA wechatIdListA = W2A(wechatIdList);

	// 设置body
	rapidjson::Value newBody(rapidjson::kObjectType); // 创建一个Object类型的元素
	// 登录状态
	rapidjson::Value wechatIdListJson(rapidjson::kStringType);
	wechatIdListJson.SetString(wechatIdListA.GetString(), wechatIdListA.GetLength());
	newBody.AddMember("wechatIdList", wechatIdListJson, package->json.GetAllocator());
	package->SetBody(newBody);

	rapidjson::StringBuffer pack = package->GetConText();
	WsClientSend((char*)pack.GetString());

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