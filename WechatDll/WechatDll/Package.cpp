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

Package::Package()
{
	json.SetObject(); // 将当前的Document设置为一个object，也就是说整个Document是一个Object类型的dom元素
}
// 解包
void Package::SetConText(char *jsonStr)
{
	json.Parse(jsonStr);

	// 客户端的应用ID，字符串，最长32位
	if (json["appId"].IsNull()) {
		this->SetAppId(NULL);
	}
	else {
		if (!json["appId"].IsString()) return;
		Value& appId = json["appId"];
		this->SetAppId((char*)appId.GetString());
	}

	// 客户端的应用KEY，字符串，最长32位
	if (!json.HasMember("appKey")) return;
	if (json["appKey"].IsNull()) {
		this->SetAppKey(NULL);
	}
	else {
		if (!json["appKey"].IsString()) return;
		Value& appKey = json["appKey"];
		this->SetAppKey((char*)appKey.GetString());
	}

	if (!json.HasMember("package")) return;
	if (!json["package"].IsObject()) return;
	rapidjson::Value& packageObject = json["package"];

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
	if (!appId) {
		rapidjson::Value appIdString(rapidjson::kNullType);
		appIdString.SetNull();
		if (this->json.HasMember("appId")) {
			this->json.RemoveMember("appId");
		}
		this->json.AddMember("appId", appIdString, this->json.GetAllocator());
	}
	else {
		rapidjson::Value appIdString(rapidjson::kStringType);
		appIdString.SetString(appId, strlen(appId));
		if (this->json.HasMember("appId")) {
			this->json.RemoveMember("appId");
		}
		this->json.AddMember("appId", appIdString, this->json.GetAllocator());
	}

	// 客户端的应用KEY，字符串，最长32位
	char *appKey = this->GetAppKey();
	if (!appKey) {
		rapidjson::Value appKeyString(rapidjson::kNullType);
		appKeyString.SetNull();
		if (this->json.HasMember("appKey")) {
			this->json.RemoveMember("appKey");
		}
		this->json.AddMember("appKey", appKeyString, this->json.GetAllocator());
	}
	else {
		rapidjson::Value appKeyString(rapidjson::kStringType);
		appKeyString.SetString(appKey, strlen(appKey));
		if (this->json.HasMember("appKey")) {
			this->json.RemoveMember("appKey");
		}
		this->json.AddMember("appKey", appKeyString, this->json.GetAllocator());
	}

	// 初始化包数据
	rapidjson::Value packageObject(rapidjson::kObjectType);

	// 数据包唯一ID，字符串，最长32位
	char *uniqueId = this->GetUniqueId();
	rapidjson::Value uniqueIdString(rapidjson::kStringType);
	if (uniqueId == NULL) {
		string uniqueId = CreateUniqueId();
		char cUniqueId[33] = { 0 };
		sprintf_s(cUniqueId, 33, "%s", uniqueId.c_str());
		uniqueIdString.SetString(cUniqueId, strlen(cUniqueId));
	}
	else {
		uniqueIdString.SetString(uniqueId, strlen(uniqueId));
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
	// 检查操作指令
	int opCode = this->GetOpCode();
	if (opCode <= 0) {
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