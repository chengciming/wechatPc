#pragma once
#include "Pjson.h"
#include "Package.h"

class Receive
{
public:
	// 发送已退出消息
	static bool SendLogout(Package *package = NULL);

protected:
	// 方法
	// 获取登录详情
	bool GetLoginInfo(Package *package);
	// 获取登录状态
	bool GetLoginStatus(Package *package);
	// 获取登录状态
	bool Logout(Package *package);
	// 发送文本消息
	bool MessageSendText(Package *package);
	// 发送图片消息
	bool MessageSendImage(Package *package);
	// 发送附件消息
	bool MessageSendFile(Package *package);
	// 发送名片消息
	bool MessageSendCard(Package *package);
	// 发送xml消息
	bool MessageSendXml(Package *package);
	// wxid加好友
	bool FriendAdd(Package *package);
	// v1加好友
	bool FriendAddFromV1(Package *package);
	// 删除好友
	bool FriendDelete(Package *package);
	// v1+v2同意好友请求
	bool FriendVerify(Package *package);
	// 好友列表
	bool FriendList(Package *package);
	// 设置好友备注
	bool FriendRemark(Package *package);
	// 创建群聊
	bool RoomCreate(Package *package);
	// 修改群名称
	bool RoomEditName(Package *package);
	// 发送群公告
	bool RoomAnnouncement(Package *package);
	// 获取群成员列表
	bool RoomMemberList(Package *package);
	// 拉好友入群
	bool RoomAddMember(Package *package);
	// 删除群成员
	bool RoomDeleteMember(Package *package);
	// 艾特群成员
	bool RoomAtMember(Package *package);
	// 退出群聊
	bool RoomQuit(Package *package);
	// 收款
	bool TransferRecv(Package *package);
public:
	int Handle(Package *package);
};