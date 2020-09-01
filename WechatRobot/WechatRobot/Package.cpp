#include "stdafx.h"
#include "Common.h"
#include "PJson.h"
#include "OpCode.h"
#include "Package.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <atlstr.h>

using namespace rapidjson;
using namespace std;

/** @var array 允许的操作指令 */
int allowOpCode[] = {
	/** @var int 响应指令 */
	0x0,  // ws客户端准备完毕
	0x1,  // 操作成功
	0x2,  // 操作失败
	/** @var int 业务指令 */
	0x10,  // 新开一个微信客户端
	0x11,  // 退出一个微信
	0x12,  // 获取登录状态
	0x20,  // 发送文本消息
	0x21,  // 发送图片消息
	0x22,  // 发送附件消息
	0x23,  // 发送名片消息
	0x24,  // 发送xml消息
	0x30,  // wxid加好友
	0x31,  // v1加好友
	0x32,  // 删除好友
	0x33,  // v1+v2同意好友请求
	0x34,  // 好友列表
	0x35,  // 修改备注
	0x40,  // 创建群聊
	0x41,  // 修改群名称
	0x42,  // 发送群公告
	0x43,  // 获取群成员列表
	0x44,  // 拉好友入群
	0x45,  // 删除群成员
	0x46,  // 艾特群成员
	0x47,  // 退出群聊
	0x50,  // 收款
	0x90,  // 返回登录二维码地址
	0x91,  // 返回当前登录的微信号信息
	0x92,  // 返回接收的微信消息
};

Package::Package()
{
	this->json.SetObject(); // 将当前的Document设置为一个object，也就是说整个Document是一个Object类型的dom元素
}
// 解包
void Package::SetConText(char *jsonStr)
{
	this->json.Parse(jsonStr);

	// 客户端的应用ID，字符串，最长32位
	if (this->json["appId"].IsNull()) {
		this->SetAppId(NULL);
	}
	else {
		if (!this->json["appId"].IsString()) return;
		Value& appId = this->json["appId"];
		this->SetAppId((char*)appId.GetString());
	}

	// 客户端的应用KEY，字符串，最长32位
	if (!this->json.HasMember("appKey")) return;
	if (this->json["appKey"].IsNull()) {
		this->SetAppKey(NULL);
	}
	else {
		if (!this->json["appKey"].IsString()) return;
		Value& appKey = this->json["appKey"];
		this->SetAppKey((char*)appKey.GetString());
	}

	if (!this->json.HasMember("package")) return;
	if (!this->json["package"].IsObject()) return;
	rapidjson::Value& packageObject = this->json["package"];

	// 数据包唯一ID，字符串，最长32位
	if (!packageObject.HasMember("uniqueId")) return;
	if (!packageObject["uniqueId"].IsString()) return;
	Value& uniqueId = packageObject["uniqueId"];
	this->SetUniqueId((char*)uniqueId.GetString());

	// 发送时间，毫秒级时间戳，字符串，13位
	if (!packageObject.HasMember("timestamp")) return;
	if (!packageObject["timestamp"].IsString()) return;
	Value& timestamp = packageObject["timestamp"];
	this->SetTimestamp((char*)timestamp.GetString());

	// 接收指令的目标微信客户端ID，每开一个微信客户端带有一个唯一ID，无目标客户端则为null，字符串或null
	if (!packageObject.HasMember("wechatId")) return;
	if (packageObject["wechatId"].IsNull()) {
		this->SetWechatId(NULL);
	}
	else {
		if (!packageObject["wechatId"].IsString()) return;
		Value& wechatId = packageObject["wechatId"];
		this->SetWechatId((char*)wechatId.GetString());
	}
	
	// 操作指令，16进制数值
	if (!packageObject.HasMember("opCode")) return;
	if (!packageObject["opCode"].IsInt()) return;
	Value& opCode = packageObject["opCode"];
	this->SetOpCode(opCode.GetInt());

	// 业务主体对象
	if (!packageObject.HasMember("body")) return;
	Value& body = packageObject["body"];
	if (packageObject["body"].IsNull()) {
		body.SetNull();
		this->SetBody(body);
	}
	else {
		if (!packageObject["body"].IsObject()) return;
		//rapidjson::Value& bodyObject = packageObject["body"];
		this->SetBody(packageObject["body"]);
	}
}
// 封包
rapidjson::StringBuffer Package::GetConText()
{
	// 客户端的应用ID，字符串，最长32位
	char *appId = this->GetAppId();
	rapidjson::Value appIdString(rapidjson::kStringType);
	if (!appId) {
		string sAppId = ReadConfig("app_id");
		char cAppId[33] = {0};
		sprintf_s(cAppId, sizeof(cAppId), "%s", sAppId.c_str());
		appIdString.SetString(cAppId, 32);
	}
	else {
		appIdString.SetString(appId, 32);
	}
	if (this->json.HasMember("appId")) {
		this->json.RemoveMember("appId");
	}
	this->json.AddMember("appId", appIdString, this->json.GetAllocator());

	// 客户端的应用KEY，字符串，最长32位
	char *appKey = this->GetAppKey();
	rapidjson::Value appKeyString(rapidjson::kStringType);
	if (!appKey) {
		string sAppKey = ReadConfig("app_key");
		char cAppKey[33] = { 0 };
		sprintf_s(cAppKey, sizeof(cAppKey), "%s", sAppKey.c_str());
		appKeyString.SetString(cAppKey, 32);
	}
	else {
		appKeyString.SetString(appKey, 32);
	}
	if (this->json.HasMember("appKey")) {
		this->json.RemoveMember("appKey");
	}
	this->json.AddMember("appKey", appKeyString, this->json.GetAllocator());

	// 初始化包数据
	rapidjson::Value packageObject(rapidjson::kObjectType);

	// 数据包唯一ID，字符串，最长32位
	char *uniqueId = this->GetUniqueId();
	rapidjson::Value uniqueIdString(rapidjson::kStringType);
	if (!uniqueId) {
		string uniqueId = CreateUniqueId();
		char cUniqueId[33] = { 0 };
		sprintf_s(cUniqueId, 33, "%s", uniqueId.c_str());
		uniqueIdString.SetString(cUniqueId, strlen(cUniqueId));
	}
	else {
		uniqueIdString.SetString(uniqueId, 32);
	}
	if (packageObject.HasMember("uniqueId")) {
		packageObject.RemoveMember("uniqueId");
	}
	packageObject.AddMember("uniqueId", uniqueIdString, this->json.GetAllocator());

	// 发送时间，毫秒级时间戳，数值
	char *timestamp = this->GetTimestamp();
	rapidjson::Value timestampString(rapidjson::kStringType);
	if (timestamp == NULL) {
		long long intTimestamp = CreateTimestamp();
		char sTimestamp[64] = { 0 };
		sprintf_s(sTimestamp, sizeof(sTimestamp), "%lld", intTimestamp);
		timestampString.SetString(sTimestamp, strlen(sTimestamp));
	}
	else {
		timestampString.SetString(timestamp, strlen(timestamp));
	}
	if (packageObject.HasMember("timestamp")) {
		packageObject.RemoveMember("timestamp");
	}
	packageObject.AddMember("timestamp", timestampString, this->json.GetAllocator());

	// 接收指令的目标微信客户端ID，每开一个微信客户端带有一个唯一ID，无目标客户端则为null，字符串或null
	char *wechatId = this->GetWechatId();
	if (wechatId == NULL) {
		rapidjson::Value wechatIdString(rapidjson::kNullType);
		wechatIdString.SetNull();
		if (packageObject.HasMember("wechatId")) {
			packageObject.RemoveMember("wechatId");
		}
		packageObject.AddMember("wechatId", wechatIdString, this->json.GetAllocator());
	}
	else {
		rapidjson::Value wechatIdString(rapidjson::kStringType);
		wechatIdString.SetString(wechatId, strlen(wechatId));
		if (packageObject.HasMember("wechatId")) {
			packageObject.RemoveMember("wechatId");
		}
		packageObject.AddMember("wechatId", wechatIdString, this->json.GetAllocator());
	}

	// 操作指令，16进制数值
	int opCode = this->GetOpCode();
	rapidjson::Value opCodeNumber(rapidjson::kNumberType);
	opCodeNumber.SetInt(opCode);
	if (packageObject.HasMember("opCode")) {
		packageObject.RemoveMember("opCode");
	}
	packageObject.AddMember("opCode", opCodeNumber, this->json.GetAllocator());

	// 业务主体对象
	rapidjson::Value *bodyObject = this->GetBody();
	if (packageObject.HasMember("body")) {
		packageObject.RemoveMember("body");
	}
	packageObject.AddMember("body", *bodyObject, this->json.GetAllocator());

	// 赋值包数据
	if (this->json.HasMember("package")) {
		this->json.RemoveMember("package");
	}
	this->json.AddMember("package", packageObject, this->json.GetAllocator());

	// 生成字符串
	rapidjson::StringBuffer buffer;
	Writer<rapidjson::StringBuffer> writer(buffer);
	this->json.Accept(writer);

	return buffer;
}
// 检查包的必传参数
bool Package::Check()
{
	// 检查应用ID
	char *appId = this->GetAppId();
	if (appId == NULL || strlen(appId) <= 0) {
		return FALSE;
	}

	// 检查应用KEY
	char *appKey = this->GetAppKey();
	if (appKey == NULL || strlen(appKey) <= 0) {
		return FALSE;
	}

	// 检查操作指令
	int opCode = this->GetOpCode();
	bool checkOpCodeResult = FALSE;
	for (int i = 0; i < sizeof(allowOpCode); i++) {
		if (opCode == allowOpCode[i]) {
			checkOpCodeResult = TRUE;
			break;
		}
	}
	if (checkOpCodeResult == FALSE) {
		return FALSE;
	}

	return TRUE;
}
/*==============================================================*/
// 获取客户端的应用ID
char *Package::GetAppId()
{
	return this->appId;
}
// 设置客户端的应用ID
void Package::SetAppId(char *appId)
{
	this->appId = appId;
}
// 获取客户端的应用KEY
char *Package::GetAppKey()
{
	return this->appKey;
}
// 设置客户端的应用KEY
void Package::SetAppKey(char *appKey)
{
	this->appKey = appKey;
}
/*==============================================================*/
// 获取数据包唯一ID
char *Package::GetUniqueId()
{
	return this->uniqueId;
}
// 设置数据包唯一
void Package::SetUniqueId(char *uniqueId)
{
	this->uniqueId = uniqueId;
}
/*==============================================================*/
// 获取发送时间
char *Package::GetTimestamp()
{
	return this->timestamp;
}
// 设置发送时间
void Package::SetTimestamp(char *timestamp)
{
	this->timestamp = timestamp;
}
/*==============================================================*/
// 获取接收指令的目标微信客户端ID
char *Package::GetWechatId()
{
	return this->wechatId;
}
// 设置接收指令的目标微信客户端ID
void Package::SetWechatId(char *wechatId)
{
	this->wechatId = wechatId;
}
/*==============================================================*/
// 获取操作指令
int Package::GetOpCode()
{
	return this->opCode;
}
// 设置操作指令
void Package::SetOpCode(int opCode)
{
	this->opCode = opCode;
}
/*==============================================================*/
// 获取业务主体对象
rapidjson::Value *Package::GetBody()
{
	return &body;
}
// 设置业务主体对象
void Package::SetBody(rapidjson::Value &body)
{
	this->body = body;
}