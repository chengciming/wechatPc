#include "stdafx.h"
#include "Common.h"
#include "OpCode.h"
#include "Package.h"
#include "WebSocket.h"
#include "LoginInfo.h"
#include "Message.h"
#include "Send.h"
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <atlstr.h>

// 发送好友列表
void Send::SendFriendList(FriendList *list, Package *package, int page, int pageSize, int total)
{
	// 初始化数据包
	if (!package) {
		package = new Package();
	}
	// 设置操作类型
	package->SetOpCode(OpCode::OPCODE_FRIEND_LIST);

	// 设置body
	rapidjson::Value body(rapidjson::kObjectType); // 创建一个Object类型的元素

	// 初始化一个好友列表字段
	rapidjson::Value friendList(rapidjson::kArrayType);

	FriendList *p = list;
	while (p) {
		if (strlen(p->wxid) > 0) {
			// 初始化一个好友详情对象
			rapidjson::Value info(rapidjson::kObjectType);

			rapidjson::Value wxid(rapidjson::kStringType);
			wxid.SetString(p->wxid, strlen(p->wxid));
			info.AddMember("wxid", wxid, package->json.GetAllocator());

			rapidjson::Value username(rapidjson::kStringType);
			if (!p->username || strcmp(p->username, "(null)") == 0) {
				username.SetString("", strlen(""));
			} else {
				username.SetString(p->username, strlen(p->username));
			}
			info.AddMember("username", username, package->json.GetAllocator());
			
			rapidjson::Value sex(rapidjson::kNumberType);
			if (!p->sex) {
				sex.SetInt(0);
			}
			else {
				sex.SetInt(p->sex);
			}
			info.AddMember("sex", sex, package->json.GetAllocator());

			rapidjson::Value v1(rapidjson::kStringType);
			if (!p->v1 || strcmp(p->v1, "(null)") == 0) {
				v1.SetString("", strlen(""));
			} else {
				v1.SetString(p->v1, strlen(p->v1));
			}
			info.AddMember("v1", v1, package->json.GetAllocator());

			rapidjson::Value v2(rapidjson::kStringType);
			if (!p->v2 || strcmp(p->v2, "(null)") == 0) {
				v2.SetString("", strlen(""));
			} else {
				v2.SetString(p->v2, strlen(p->v2));
			}
			info.AddMember("v2", v2, package->json.GetAllocator());

			rapidjson::Value type(rapidjson::kNumberType);
			if (!p->type) {
				type.SetInt(0);
			} else {
				type.SetInt(p->type);
			}
			info.AddMember("type", type, package->json.GetAllocator());

			rapidjson::Value nickname(rapidjson::kStringType);
			if (!p->nickname || strcmp(p->nickname, "(null)") == 0) {
				nickname.SetString("", strlen(""));
			} else {
				nickname.SetString(p->nickname, strlen(p->nickname));
			}
			info.AddMember("nickname", nickname, package->json.GetAllocator());

			rapidjson::Value roomType(rapidjson::kNumberType);
			if (!p->roomType) {
				roomType.SetInt(0);
			}
			else {
				roomType.SetInt(p->roomType);
			}
			info.AddMember("roomType", roomType, package->json.GetAllocator());

			rapidjson::Value remark(rapidjson::kStringType);
			if (!p->remark || strcmp(p->remark, "(null)") == 0) {
				remark.SetString("", strlen(""));
			} else {
				remark.SetString(p->remark, strlen(p->remark));
			}
			info.AddMember("remark", remark, package->json.GetAllocator());

			rapidjson::Value headUrl(rapidjson::kStringType);
			if (!p->bigHeadImgUrl || strcmp(p->bigHeadImgUrl, "(null)") == 0 || strlen(p->bigHeadImgUrl) <= 0) {
				if (!p->smallHeadImgUrl || strcmp(p->smallHeadImgUrl, "(null)") == 0) {
					headUrl.SetString("", strlen(""));
				} else {
					headUrl.SetString(p->smallHeadImgUrl, strlen(p->smallHeadImgUrl));
				}
			} else {
				headUrl.SetString(p->bigHeadImgUrl, strlen(p->bigHeadImgUrl));
			}
			info.AddMember("headUrl", headUrl, package->json.GetAllocator());

			rapidjson::Value delFlag(rapidjson::kNumberType);
			if (!p->delFlag) {
				delFlag.SetInt(0);
			}
			else {
				delFlag.SetInt(p->delFlag);
			}
			info.AddMember("delFlag", delFlag, package->json.GetAllocator());

			rapidjson::Value verifyFlag(rapidjson::kNumberType);
			if (!p->verifyFlag) {
				verifyFlag.SetInt(0);
			}
			else {
				verifyFlag.SetInt(p->verifyFlag);
			}
			info.AddMember("verifyFlag", verifyFlag, package->json.GetAllocator());

			rapidjson::Value roomWxidList(rapidjson::kStringType);
			if (!p->roomWxidList || strcmp(p->roomWxidList, "(null)") == 0) {
				roomWxidList.SetString("", strlen(""));
			} else {
				roomWxidList.SetString(p->roomWxidList, strlen(p->roomWxidList));
			}
			info.AddMember("roomWxidList", roomWxidList, package->json.GetAllocator());

			rapidjson::Value roomOwner(rapidjson::kNumberType);
			if (!p->roomOwner) {
				roomOwner.SetInt(0);
			}
			else {
				roomOwner.SetInt(p->roomOwner);
			}
			info.AddMember("roomOwner", roomOwner, package->json.GetAllocator());

			rapidjson::Value roomAnnouncement(rapidjson::kStringType);
			if (!p->roomAnnouncement || strcmp(p->roomAnnouncement, "(null)") == 0) {
				roomAnnouncement.SetString("", strlen(""));
			} else {
				roomAnnouncement.SetString(p->roomAnnouncement, strlen(p->roomAnnouncement));
			}
			info.AddMember("roomAnnouncement", roomAnnouncement, package->json.GetAllocator());

			rapidjson::Value roomAnnouncementEditor(rapidjson::kStringType);
			if (!p->roomAnnouncementEditor || strcmp(p->roomAnnouncementEditor, "(null)") == 0) {
				roomAnnouncementEditor.SetString("", strlen(""));
			} else {
				roomAnnouncementEditor.SetString(p->roomAnnouncementEditor, strlen(p->roomAnnouncementEditor));
			}
			info.AddMember("roomAnnouncementEditor", roomAnnouncementEditor, package->json.GetAllocator());

			rapidjson::Value roomAnnouncementPublishTime(rapidjson::kStringType);
			if (!p->roomAnnouncementPublishTime || strcmp(p->roomAnnouncementPublishTime, "(null)") == 0) {
				roomAnnouncementPublishTime.SetString("", strlen(""));
			} else {
				roomAnnouncementPublishTime.SetString(p->roomAnnouncementPublishTime, strlen(p->roomAnnouncementPublishTime));
			}
			info.AddMember("roomAnnouncementPublishTime", roomAnnouncementPublishTime, package->json.GetAllocator());

			rapidjson::Value roomStatus(rapidjson::kNumberType);
			if (!p->roomStatus) {
				roomStatus.SetInt(0);
			}
			else {
				roomStatus.SetInt(p->roomStatus);
			}
			info.AddMember("roomStatus", roomStatus, package->json.GetAllocator());

			rapidjson::Value roomNotify(rapidjson::kNumberType);
			if (!p->roomNotify) {
				roomNotify.SetInt(0);
			}
			else {
				roomNotify.SetInt(p->roomNotify);
			}
			info.AddMember("roomNotify", roomNotify, package->json.GetAllocator());

			rapidjson::Value bizType(rapidjson::kNumberType);
			if (!p->bizType) {
				bizType.SetInt(0);
			}
			else {
				bizType.SetInt(p->bizType);
			}
			info.AddMember("bizType", bizType, package->json.GetAllocator());

			rapidjson::Value bizAcceptType(rapidjson::kNumberType);
			if (!p->bizAcceptType) {
				bizAcceptType.SetInt(0);
			}
			else {
				bizAcceptType.SetInt(p->bizAcceptType);
			}
			info.AddMember("bizAcceptType", bizAcceptType, package->json.GetAllocator());
			
			rapidjson::Value bizBrandList(rapidjson::kStringType);
			if (!p->bizBrandList || strcmp(p->bizBrandList, "(null)") == 0) {
				bizBrandList.SetString("", strlen(""));
			}
			else {
				bizBrandList.SetString(p->bizBrandList, strlen(p->bizBrandList));
			}
			info.AddMember("bizBrandList", bizBrandList, package->json.GetAllocator());

			rapidjson::Value bizBrandFlag(rapidjson::kNumberType);
			if (!p->bizBrandFlag) {
				bizBrandFlag.SetInt(0);
			}
			else {
				bizBrandFlag.SetInt(p->bizBrandFlag);
			}
			info.AddMember("bizBrandFlag", bizBrandFlag, package->json.GetAllocator());

			rapidjson::Value bizBrandIconURL(rapidjson::kStringType);
			if (!p->bizBrandIconURL || strcmp(p->bizBrandIconURL, "(null)") == 0) {
				bizBrandIconURL.SetString("", strlen(""));
			}
			else {
				bizBrandIconURL.SetString(p->bizBrandIconURL, strlen(p->bizBrandIconURL));
			}
			info.AddMember("bizBrandIconURL", bizBrandIconURL, package->json.GetAllocator());

			rapidjson::Value bizExtInfo(rapidjson::kStringType);
			if (!p->bizExtInfo || strcmp(p->bizExtInfo, "(null)") == 0) {
				bizExtInfo.SetString("", strlen(""));
			}
			else {
				bizExtInfo.SetString(p->bizExtInfo, strlen(p->bizExtInfo));
			}
			info.AddMember("bizExtInfo", bizExtInfo, package->json.GetAllocator());
			
			rapidjson::Value extraBuf(rapidjson::kStringType);
			if (!p->extraBuf || strcmp(p->extraBuf, "(null)") == 0) {
				extraBuf.SetString("", strlen(""));
			}
			else {
				extraBuf.SetString(p->extraBuf, strlen(p->extraBuf));
			}
			info.AddMember("extraBuf", extraBuf, package->json.GetAllocator());

			friendList.PushBack(info, package->json.GetAllocator());
		}
		p = p->next;
	}

	// 插入消息列表
	body.AddMember("friendList", friendList, package->json.GetAllocator());
	// 分页参数
	rapidjson::Value pageNumber(rapidjson::kNumberType);
	pageNumber.SetInt(page);
	rapidjson::Value pageSizeNumber(rapidjson::kNumberType);
	pageSizeNumber.SetInt(pageSize);
	rapidjson::Value totalNumber(rapidjson::kNumberType);
	totalNumber.SetInt(total);
	body.AddMember("page", pageNumber, package->json.GetAllocator());
	body.AddMember("pageSize", pageSizeNumber, package->json.GetAllocator());
	body.AddMember("total", totalNumber, package->json.GetAllocator());

	package->SetBody(body);

	// 获取json字符串
	rapidjson::StringBuffer pack = package->GetConText();
	// 发送
	WsClientSend((char*)pack.GetString());
}

void Send::SendWxMessage(WebsocketMessageStruct *message, Package *package)
{
	// 初始化数据包
	Package tmp_package;
	if (!package) {
		package = &tmp_package;
	}
	// 设置操作类型
	package->SetOpCode(OpCode::OPCODE_MESSAGE_RECEIVE);

	// 设置body
	rapidjson::Value body(rapidjson::kObjectType); // 创建一个Object类型的元素

	// 消息ID
	rapidjson::Value msgId(rapidjson::kStringType);
	msgId.SetString(message->msgId, strlen(message->msgId));
	body.AddMember("msgId", msgId, package->json.GetAllocator());
	// 是否自己发的消息：0=否，1=是
	rapidjson::Value isOwner(rapidjson::kNumberType);
	isOwner.SetInt(message->isOwner);
	body.AddMember("isOwner", isOwner, package->json.GetAllocator());
	// 消息类型
	rapidjson::Value msgType(rapidjson::kNumberType);
	msgType.SetInt(message->msgType);
	body.AddMember("msgType", msgType, package->json.GetAllocator());
	// 消息来源：0=好友消息，1=群消息
	rapidjson::Value msgSource(rapidjson::kNumberType);
	msgSource.SetInt(message->msgSource);
	body.AddMember("msgSource", msgSource, package->json.GetAllocator());
	// 消息来源的XML
	rapidjson::Value msgSourceXml(rapidjson::kStringType);
	msgSourceXml.SetString(message->msgSourceXml, strlen(message->msgSourceXml));
	body.AddMember("msgSourceXml", msgSourceXml, package->json.GetAllocator());
	// 发送人微信ID
	rapidjson::Value wxid(rapidjson::kStringType);
	wxid.SetString(message->wxid, strlen(message->wxid));
	body.AddMember("wxid", wxid, package->json.GetAllocator());
	// 群ID
	rapidjson::Value roomId(rapidjson::kStringType);
	roomId.SetString(message->roomId, strlen(message->roomId));
	body.AddMember("roomId", roomId, package->json.GetAllocator());
	// 消息内容
	rapidjson::Value content(rapidjson::kStringType);
	content.SetString(message->content, strlen(message->content));
	body.AddMember("content", content, package->json.GetAllocator());

	package->SetBody(body);

	// 获取json字符串
	rapidjson::StringBuffer pack = package->GetConText();
	// 发送
	WsClientSend((char*)pack.GetString());
}

// 发送登录详情
void Send::SendLoginInfo(LoginInfo *info, Package *package)
{
	// 初始化数据包
	if (!package) {
		package = new Package();
	}
	// 设置操作类型
	package->SetOpCode(OpCode::OPCODE_LOGIN_INFO);

	// 设置body
	rapidjson::Value body(rapidjson::kObjectType); // 创建一个Object类型的元素

	rapidjson::Value wxid(rapidjson::kStringType);
	wxid.SetString(info->wxid, strlen(info->wxid));
	body.AddMember("wxid", wxid, package->json.GetAllocator());

	rapidjson::Value username(rapidjson::kStringType);
	username.SetString(info->username, strlen(info->username));
	body.AddMember("username", username, package->json.GetAllocator());

	rapidjson::Value nickname(rapidjson::kStringType);
	nickname.SetString(info->nickname, strlen(info->nickname));
	body.AddMember("nickname", nickname, package->json.GetAllocator());

	rapidjson::Value headUrl(rapidjson::kStringType);
	headUrl.SetString(info->headUrl, strlen(info->headUrl));
	body.AddMember("headUrl", headUrl, package->json.GetAllocator());

	rapidjson::Value mobile(rapidjson::kStringType);
	mobile.SetString(info->mobile, strlen(info->mobile));
	body.AddMember("mobile", mobile, package->json.GetAllocator());

	rapidjson::Value email(rapidjson::kStringType);
	email.SetString(info->email, strlen(info->email));
	body.AddMember("email", email, package->json.GetAllocator());

	rapidjson::Value sex(rapidjson::kNumberType);
	sex.SetInt(info->sex);
	body.AddMember("sex", sex, package->json.GetAllocator());

	rapidjson::Value nation(rapidjson::kStringType);
	nation.SetString(info->nation, strlen(info->nation));
	body.AddMember("nation", nation, package->json.GetAllocator());

	rapidjson::Value province(rapidjson::kStringType);
	province.SetString(info->province, strlen(info->province));
	body.AddMember("province", province, package->json.GetAllocator());
	
	rapidjson::Value city(rapidjson::kStringType);
	city.SetString(info->city, strlen(info->city));
	body.AddMember("city", city, package->json.GetAllocator());

	// 先不做个性签名，会有中文乱码的情况
	//rapidjson::Value sign(rapidjson::kStringType);
	//sign.SetString(info->sign, strlen(info->sign));
	//body.AddMember("sign", sign, package->json.GetAllocator());

	rapidjson::Value device(rapidjson::kStringType);
	device.SetString(info->device, strlen(info->device));
	body.AddMember("device", device, package->json.GetAllocator());

	package->SetBody(body);

	// 获取json字符串
	rapidjson::StringBuffer pack = package->GetConText();
	// 发送
	WsClientSend((char*)pack.GetString());
}

// 发送登录状态
void Send::SendLoginStatus(int loginStatus, Package *package)
{
	// 初始化数据包
	if (!package) {
		package = new Package();
	}
	// 设置操作类型
	package->SetOpCode(OpCode::OPCODE_WECHAT_GET_LOGIN_STATUS);

	// 设置body
	rapidjson::Value body(rapidjson::kObjectType); // 创建一个Object类型的元素
	rapidjson::Value statusInt(rapidjson::kNumberType);
	statusInt.SetInt(loginStatus);
	body.AddMember("loginStatus", statusInt, package->json.GetAllocator());
	package->SetBody(body);

	// 获取json字符串
	rapidjson::StringBuffer pack = package->GetConText();
	// 发送
	WsClientSend((char*)pack.GetString());
}

// 发送登录二维码
void Send::SendLoginQrcode(int loginStatus, char *qrcode, Package *package)
{
	// 初始化数据包
	if (!package) {
		package = new Package();
	}
	// 设置操作类型
	package->SetOpCode(OpCode::OPCODE_WECHAT_QRCODE);

	// 设置body
	rapidjson::Value body(rapidjson::kObjectType); // 创建一个Object类型的元素
	// 登录状态
	rapidjson::Value loginStatusJson(rapidjson::kNumberType);
	loginStatusJson.SetInt(loginStatus);
	body.AddMember("loginStatus", loginStatusJson, package->json.GetAllocator());
	// 二维码链接
	rapidjson::Value qrcodeJson(rapidjson::kStringType);
	qrcodeJson.SetString(qrcode, strlen(qrcode));
	body.AddMember("loginQrcode", qrcodeJson, package->json.GetAllocator());

	package->SetBody(body);

	// 获取json字符串
	rapidjson::StringBuffer pack = package->GetConText();
	// 发送
	WsClientSend((char*)pack.GetString());
}