#pragma once
#include "OpCode.h"
#include "PJson.h"

using namespace rapidjson;


class Package
{
	// Construction
public:
	Package();	// standard constructor

	rapidjson::Document json;

private:
	char *appId = NULL;  // 客户端的应用ID，字符串，最长32位
	char *appKey = NULL;  // 客户端的应用KEY，字符串，最长32位
	
	char *uniqueId = NULL;  // 数据包唯一ID，字符串，最长32位
	char *timestamp = NULL;  // 发送时间，毫秒级时间戳，字符串,13位
	char *wechatId = NULL;  // 接收指令的目标微信客户端ID，每开一个微信客户端带有一个唯一ID，无目标客户端则为null，字符串或null
	int opCode = 0;  // 操作指令，16进制数值

	rapidjson::Value body;  // 业务主体对象
public:
	/*==============================================================*/
	// 解包
	void SetConText(char *jsonStr);
	// 封包
	rapidjson::StringBuffer GetConText();
	// 检查包的必传参数
	bool Check();
	/*==============================================================*/
	// 获取客户端的应用ID
	char *GetAppId();
	// 设置客户端的应用ID
	void SetAppId(char *appId);
	// 获取客户端的应用KEY
	char *GetAppKey();
	// 设置客户端的应用KEY
	void SetAppKey(char *appKey);
	/*==============================================================*/
	// 获取数据包唯一ID
	char *GetUniqueId();
	// 设置数据包唯一
	void SetUniqueId(char *uniqueId);
	/*==============================================================*/
	// 获取发送时间
	char *GetTimestamp();
	// 设置发送时间
	void SetTimestamp(char *timestamp);
	/*==============================================================*/
	// 获取接收指令的目标微信客户端ID
	char *GetWechatId();
	// 设置接收指令的目标微信客户端ID
	void SetWechatId(char *wechatId);
	/*==============================================================*/
	// 获取操作指令
	int GetOpCode();
	// 设置操作指令
	void SetOpCode(int opCode);
	/*==============================================================*/
	// 获取业务主体对象
	rapidjson::Value *GetBody();
	// 设置业务主体对象
	void SetBody(rapidjson::Value &body);
	/*==============================================================*/
};