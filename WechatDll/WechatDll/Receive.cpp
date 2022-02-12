#include "stdafx.h"
#include "Common.h"
#include "OpCode.h"
#include "Package.h"
#include "WebSocket.h"
#include "Login.h"
#include "LoginInfo.h"
#include "Message.h"
#include "Friends.h"
#include "Transfer.h"
#include "Room.h"
#include "Send.h"
#include "Receive.h"
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <atlstr.h>

// 发送已退出消息
bool Receive::SendLogout(Package *package)
{
	// 初始化数据包
	Package tmp_package;
	if (!package) {
		package = &tmp_package;
	}
	// 设置操作类型
	package->SetOpCode(OpCode::OPCODE_WECHAT_QUIT);
	// 设置body
	rapidjson::Value newBody(rapidjson::kObjectType); // 创建一个Object类型的元素
	// 进程ID
	rapidjson::Value processId(rapidjson::kNumberType);
	processId.SetInt(GetCurrentProcessId());
	newBody.AddMember("processId", processId, package->json.GetAllocator());
	// 登录状态
	rapidjson::Value loginStatus(rapidjson::kNumberType);
	loginStatus.SetInt(CheckLoginStatus());
	newBody.AddMember("loginStatus", loginStatus, package->json.GetAllocator());
	package->SetBody(newBody);
	rapidjson::StringBuffer pack = package->GetConText();
	WsClientSend((char*)pack.GetString());
	return TRUE;
}

// 发送登录信息
bool Receive::GetLoginInfo(Package *package)
{
	// 发送详情
	SendWechatUser(package);

	return TRUE;
}

// 获取登录状态
bool Receive::GetLoginStatus(Package *package)
{
	// 设置body
	rapidjson::Value newBody(rapidjson::kObjectType); // 创建一个Object类型的元素
	// 登录状态
	rapidjson::Value loginStatus(rapidjson::kNumberType);
	loginStatus.SetInt(CheckLoginStatus());
	newBody.AddMember("loginStatus", loginStatus, package->json.GetAllocator());
	package->SetBody(newBody);
	rapidjson::StringBuffer pack = package->GetConText();
	WsClientSend((char*)pack.GetString());
	return TRUE;
}
// 退出登录
bool Receive::Logout(Package *package)
{
	// 退出登录操作
	DoLogout();
	// 发送已退出消息
	Receive::SendLogout(package);
	
	return TRUE;
}
// 发送文本消息
bool Receive::MessageSendText(Package *package)
{
	Value *pBody = package->GetBody();
	Value &body = *pBody;
	if (!body.HasMember("wxid") || !body.HasMember("content")) {
		return FALSE;
	}
	Value& wxid = body["wxid"];
	Value& content = body["content"];

	// 发送
	SendTextMessage(UTF8ToUnicode((char*)wxid.GetString()), UTF8ToUnicode((char*)content.GetString()));
	
	return TRUE;
}
// 发送图片消息
bool Receive::MessageSendImage(Package *package)
{
	Value *pBody = package->GetBody();
	Value &body = *pBody;
	if (!body.HasMember("wxid") || !body.HasMember("imageUrl")) {
		return FALSE;
	}
	Value& wxid = body["wxid"];
	Value& imageUrl = body["imageUrl"];

	// 发送
	SendImageMessage(UTF8ToUnicode((char*)wxid.GetString()), UTF8ToUnicode((char*)imageUrl.GetString()));

	return TRUE;
}
// 发送附件消息
bool Receive::MessageSendFile(Package *package)
{
	Value *pBody = package->GetBody();
	Value &body = *pBody;
	if (!body.HasMember("wxid") || !body.HasMember("fileUrl")) {
		return FALSE;
	}
	Value& wxid = body["wxid"];
	Value& fileUrl = body["fileUrl"];

	// 发送
	SendFileMessage(UTF8ToUnicode((char*)wxid.GetString()), UTF8ToUnicode((char*)fileUrl.GetString()));

	return TRUE;
}
// 发送名片消息
bool Receive::MessageSendCard(Package *package)
{
	Value *pBody = package->GetBody();
	Value &body = *pBody;
	if (!body.HasMember("wxid") || !body.HasMember("xml")) {
		return FALSE;
	}
	Value& wxid = body["wxid"];
	Value& xml = body["xml"];

	// 发送
	SendXmlCardMessage(UTF8ToUnicode((char*)wxid.GetString()), UTF8ToUnicode((char*)xml.GetString()));

	return TRUE;
}
// 发送xml消息
bool Receive::MessageSendXml(Package *package)
{
	Value *pBody = package->GetBody();
	Value &body = *pBody;
	if (!body.HasMember("type") || !body.HasMember("wxid") || !body.HasMember("fromWxid") || !body.HasMember("imageUrl") || !body.HasMember("xml")) {
		return FALSE;
	}
	Value& type = body["type"];
	Value& wxid = body["wxid"];
	Value& fromWxid = body["fromWxid"];
	Value& imageUrl = body["imageUrl"];
	Value& xml = body["xml"];

	// 发送
	SendXmlMessage(type.GetInt(), UTF8ToUnicode((char*)wxid.GetString()), UTF8ToUnicode((char*)fromWxid.GetString()), UTF8ToUnicode((char*)imageUrl.GetString()), UTF8ToUnicode((char*)xml.GetString()));

	return TRUE;
}
// wxid加好友
bool Receive::FriendAdd(Package *package)
{
	Value *pBody = package->GetBody();
	Value &body = *pBody;
	if (!body.HasMember("wxid") || !body.HasMember("message")) {
		return FALSE;
	}
	Value& wxid = body["wxid"];
	Value& message = body["message"];

	// 发送
	AddFriend(UTF8ToUnicode((char*)wxid.GetString()), UTF8ToUnicode((char*)message.GetString()));

	return TRUE;
}
// v1加好友
bool Receive::FriendAddFromV1(Package *package)
{
	Value *pBody = package->GetBody();
	Value &body = *pBody;
	if (!body.HasMember("v1") || !body.HasMember("message")) {
		return FALSE;
	}
	Value& v1 = body["v1"];
	Value& message = body["message"];

	// 发送
	AddUserFromCard(UTF8ToUnicode((char*)v1.GetString()), UTF8ToUnicode((char*)message.GetString()));

	return TRUE;
}
// 删除好友
bool Receive::FriendDelete(Package *package)
{
	Value *pBody = package->GetBody();
	Value &body = *pBody;
	if (!body.HasMember("wxid")) {
		return FALSE;
	}
	Value& wxid = body["wxid"];

	// 发送
	DeleteUser(UTF8ToUnicode((char*)wxid.GetString()));

	return TRUE;
}
// v1+v2同意好友请求
bool Receive::FriendVerify(Package *package)
{
	Value *pBody = package->GetBody();
	Value &body = *pBody;
	if (!body.HasMember("v1") || !body.HasMember("v2")) {
		return FALSE;
	}
	Value& v1 = body["v1"];
	Value& v2 = body["v2"];

	// 发送
	VerifyFriend(UTF8ToUnicode((char*)v1.GetString()), UTF8ToUnicode((char*)v2.GetString()));

	return TRUE;
}
// 好友列表
bool Receive::FriendList(Package *package)
{
	// 发送好友列表
	selectFriendList();
	return TRUE;
}
// 设置好友备注
bool Receive::FriendRemark(Package *package)
{
	Value *pBody = package->GetBody();
	Value &body = *pBody;
	if (!body.HasMember("wxid") || !body.HasMember("remark")) {
		return FALSE;
	}
	Value& wxid = body["wxid"];
	Value& remark = body["remark"];
	
	SetRemark(UTF8ToUnicode((char*)wxid.GetString()), UTF8ToUnicode((char*)remark.GetString()));
	return TRUE;
}
// 创建群聊
bool Receive::RoomCreate(Package *package)
{
	Value *pBody = package->GetBody();
	Value &body = *pBody;
	if (!body.HasMember("wxid1") || !body.HasMember("wxid2")) {
		return FALSE;
	}
	Value& wxid1 = body["wxid1"];
	Value& wxid2 = body["wxid2"];

	// 发送
	CreateRoom(UTF8ToUnicode((char*)wxid1.GetString()), UTF8ToUnicode((char*)wxid2.GetString()));

	return TRUE;
}
// 修改群名称
bool Receive::RoomEditName(Package *package)
{
	Value *pBody = package->GetBody();
	Value &body = *pBody;
	if (!body.HasMember("roomId") || !body.HasMember("roomName")) {
		return FALSE;
	}
	Value& roomId = body["roomId"];
	Value& roomName = body["roomName"];

	// 发送
	SetRoomName(UTF8ToUnicode((char*)roomId.GetString()), UTF8ToUnicode((char*)roomName.GetString()));

	return TRUE;
}
// 发送群公告
bool Receive::RoomAnnouncement(Package *package)
{
	Value *pBody = package->GetBody();
	Value &body = *pBody;
	if (!body.HasMember("roomId") || !body.HasMember("announcement")) {
		return FALSE;
	}
	Value& roomId = body["roomId"];
	Value& announcement = body["announcement"];

	// 发送
	SetRoomAnnouncement(UTF8ToUnicode((char*)roomId.GetString()), UTF8ToUnicode((char*)announcement.GetString()));

	return TRUE;
}
// 获取群成员列表
bool Receive::RoomMemberList(Package *package)
{
	Value *pBody = package->GetBody();
	Value &body = *pBody;
	if (!body.HasMember("roomId")) {
		return FALSE;
	}
	Value& roomId = body["roomId"];

	// 发送
	GetRoomUserList(UTF8ToUnicode((char*)roomId.GetString()));

	return TRUE;
}
// 拉好友入群
bool Receive::RoomAddMember(Package *package)
{
	Value *pBody = package->GetBody();
	Value &body = *pBody;
	if (!body.HasMember("roomId") || !body.HasMember("wxid")) {
		return FALSE;
	}
	Value& roomId = body["roomId"];
	Value& wxid = body["wxid"];

	// 发送
	AddRoomUser(UTF8ToUnicode((char*)roomId.GetString()), UTF8ToUnicode((char*)wxid.GetString()));

	return TRUE;
}
// 删除群成员
bool Receive::RoomDeleteMember(Package *package)
{
	Value *pBody = package->GetBody();
	Value &body = *pBody;
	if (!body.HasMember("roomId") || !body.HasMember("wxid")) {
		return FALSE;
	}
	Value& roomId = body["roomId"];
	Value& wxid = body["wxid"];

	// 发送
	DelRoomUser(UTF8ToUnicode((char*)roomId.GetString()), UTF8ToUnicode((char*)wxid.GetString()));

	return TRUE;
}
// 艾特群成员
bool Receive::RoomAtMember(Package *package)
{
	Value *pBody = package->GetBody();
	Value &body = *pBody;
	if (!body.HasMember("roomId") || !body.HasMember("wxid") || !body.HasMember("nickname") || !body.HasMember("message")) {
		return FALSE;
	}
	Value& roomId = body["roomId"];
	Value& wxid = body["wxid"];
	Value& nickname = body["nickname"];
	Value& message = body["message"];

	// 发送
	SendRoomAtMsg(UTF8ToUnicode((char*)roomId.GetString()), UTF8ToUnicode((char*)wxid.GetString()), UTF8ToUnicode((char*)nickname.GetString()), UTF8ToUnicode((char*)message.GetString()));

	return TRUE;
}
// 退出群聊
bool Receive::RoomQuit(Package *package)
{
	Value *pBody = package->GetBody();
	Value &body = *pBody;
	if (!body.HasMember("roomId")) {
		return FALSE;
	}
	Value& roomId = body["roomId"];

	// 发送
	QuitRoom(UTF8ToUnicode((char*)roomId.GetString()));

	return TRUE;
}
// 收款
bool Receive::TransferRecv(Package *package)
{
	Value *pBody = package->GetBody();
	Value &body = *pBody;
	if (!body.HasMember("wxid") || !body.HasMember("transferId")) {
		return FALSE;
	}
	Value& wxid = body["wxid"];
	Value& transferId = body["transferId"];

	// 发送
	CollectMoney(UTF8ToUnicode((char*)wxid.GetString()), UTF8ToUnicode((char*)transferId.GetString()));

	return TRUE;
}

// 数据包处理
int Receive::Handle(Package *package)
{
	int opCode = package->GetOpCode();

	int result = 0;  // 0=正常,1=不支持的指令,2=异常
	bool doResult = FALSE;  // 执行结果

	// 获取登录详情
	if (opCode == OpCode::OPCODE_LOGIN_INFO) {
		doResult = this->GetLoginInfo(package);
	}
	// 获取登录状态
	else if (opCode == OpCode::OPCODE_WECHAT_GET_LOGIN_STATUS) {
		doResult = this->GetLoginStatus(package);
	}
	// 退出登录
	else if (opCode == OpCode::OPCODE_WECHAT_QUIT) {
		doResult = this->Logout(package);
	}
	// 发送文本消息
	else if (opCode == OpCode::OPCODE_MESSAGE_SEND_TEXT) {
		doResult = this->MessageSendText(package);
	}
	// 发送图片消息
	else if (opCode == OpCode::OPCODE_MESSAGE_SEND_IMAGE) {
		doResult = this->MessageSendImage(package);
	}
	// 发送附件消息
	else if (opCode == OpCode::OPCODE_MESSAGE_SEND_FILE) {
		doResult = this->MessageSendFile(package);
	}
	// 发送名片消息
	else if (opCode == OpCode::OPCODE_MESSAGE_SEND_CARD) {
		doResult = this->MessageSendCard(package);
	}
	// 发送xml消息
	else if (opCode == OpCode::OPCODE_MESSAGE_SEND_XML) {
		doResult = this->MessageSendXml(package);
	}
	// 获取好友列表
	else if (opCode == OpCode::OPCODE_FRIEND_LIST) {
		doResult = this->FriendList(package);
	}
	// 获取好友列表
	else if (opCode == OpCode::OPCODE_FRIEND_REMARK) {
		doResult = this->FriendRemark(package);
	}
	// wxid加好友
	else if (opCode == OpCode::OPCODE_FRIEND_ADD) {
		doResult = this->FriendAdd(package);
	}
	// v1加好友
	else if (opCode == OpCode::OPCODE_FRIEND_ADD_FROM_V1) {
		doResult = this->FriendAddFromV1(package);
	}
	// 删除好友
	else if (opCode == OpCode::OPCODE_FRIEND_DELETE) {
		doResult = this->FriendDelete(package);
	}
	// v1+v2同意好友请求
	else if (opCode == OpCode::OPCODE_FRIEND_VERIFY) {
		doResult = this->FriendVerify(package);
	}
	// 创建群聊
	else if (opCode == OpCode::OPCODE_ROOM_CREATE) {
		doResult = this->RoomCreate(package);
	}
	// 修改群名称
	else if (opCode == OpCode::OPCODE_ROOM_EDIT_NAME) {
		doResult = this->RoomEditName(package);
	}
	// 发送群公告
	else if (opCode == OpCode::OPCODE_ROOM_ANNOUNCEMENT) {
		doResult = this->RoomAnnouncement(package);
	}
	// 获取群成员列表
	else if (opCode == OpCode::OPCODE_ROOM_MEMBER_LIST) {
		doResult = this->RoomMemberList(package);
	}
	// 拉好友入群
	else if (opCode == OpCode::OPCODE_ROOM_ADD_MEMBER) {
		doResult = this->RoomAddMember(package);
	}
	// 删除群成员
	else if (opCode == OpCode::OPCODE_ROOM_DELETE_MEMBER) {
		doResult = this->RoomDeleteMember(package);
	}
	// 艾特群成员
	else if (opCode == OpCode::OPCODE_ROOM_AT_MEMBER) {
		doResult = this->RoomAtMember(package);
	}
	// 退出群聊
	else if (opCode == OpCode::OPCODE_ROOM_QUIT) {
		doResult = this->RoomQuit(package);
	}
	// 收款
	else if (opCode == OpCode::OPCODE_TRANSFER_RECV) {
		doResult = this->TransferRecv(package);
	}
	else {
		result = 1;   // 0=正常,1=不支持的指令,2=异常
	}

	// 如果是正常的就直接返回
	if (doResult == TRUE) {
		return result;
	}

	// 操作失败的
	if (result == 0) {  // doResult == FALSE && result == 0
		result = 2;  // 0=正常,1=不支持的指令,2=异常
	}

	// 异常指令
	package->SetOpCode(OpCode::OPCODE_FAILURE);
	// 设置body
	rapidjson::Value newBody(rapidjson::kObjectType); // 创建一个Object类型的元素
	// 源操作指令
	rapidjson::Value oldOpCode(rapidjson::kNumberType);
	oldOpCode.SetInt(opCode);
	newBody.AddMember("opCode", opCode, package->json.GetAllocator());
	// 错误码
	rapidjson::Value errorCode(rapidjson::kNumberType);
	errorCode.SetInt(result);
	newBody.AddMember("errorCode", errorCode, package->json.GetAllocator());
	// 错误信息
	rapidjson::Value errorMessage(rapidjson::kStringType);
	if (result == 1) {
		char str[] = "不支持的指令";
		errorMessage.SetString(GB2312ToUTF8(str), strlen(GB2312ToUTF8(str)));
		newBody.AddMember("errorMessage", errorMessage, package->json.GetAllocator());
	}
	else if (result == 2) {
		char str[] = "执行异常，请检查业务参数！";
		errorMessage.SetString(GB2312ToUTF8(str), strlen(GB2312ToUTF8(str)));
		newBody.AddMember("errorMessage", errorMessage, package->json.GetAllocator());
	}
	else {
		char str[] = "未知的错误";
		errorMessage.SetString(GB2312ToUTF8(str), strlen(GB2312ToUTF8(str)));
		newBody.AddMember("errorMessage", errorMessage, package->json.GetAllocator());
	}
	package->SetBody(newBody);
	rapidjson::StringBuffer pack = package->GetConText();
	WsClientSend((char*)pack.GetString());

	return result;
}