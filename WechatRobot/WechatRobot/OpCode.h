#pragma once

class OpCode
{
public:
	/** @var int 响应指令 */
	static const int OPCODE_READY = 0x0;  // ws客户端准备完毕
	static const int OPCODE_SUCCESS = 0x1;  // 操作成功
	static const int OPCODE_FAILURE = 0x2;  // 操作失败
	/** @var int 业务指令 */
	static const int OPCODE_WECHAT_OPEN = 0x10;  // 新开一个微信客户端
	static const int OPCODE_WECHAT_QUIT = 0x11;  // 退出一个微信
	static const int OPCODE_WECHAT_GET_LOGIN_STATUS = 0x12;  // 获取登录状态
	static const int OPCODE_MESSAGE_SEND_TEXT = 0x20;  // 发送文本消息
	static const int OPCODE_MESSAGE_SEND_IMAGE = 0x21;  // 发送图片消息
	static const int OPCODE_MESSAGE_SEND_FILE = 0x22;  // 发送附件消息
	static const int OPCODE_MESSAGE_SEND_CARD = 0x23;  // 发送名片消息
	static const int OPCODE_MESSAGE_SEND_XML = 0x24;  // 发送xml消息
	static const int OPCODE_FRIEND_ADD = 0x30;  // wxid加好友
	static const int OPCODE_FRIEND_ADD_FROM_V1 = 0x31;  // v1加好友
	static const int OPCODE_FRIEND_DELETE = 0x32;  // 删除好友
	static const int OPCODE_FRIEND_VERIFY = 0x33;  // v1+v2同意好友请求
	static const int OPCODE_FRIEND_LIST = 0x34;  // 好友列表
	static const int OPCODE_FRIEND_REMARK = 0x35;  // 修改备注
	static const int OPCODE_ROOM_CREATE = 0x40;  // 创建群聊
	static const int OPCODE_ROOM_EDIT_NAME = 0x41;  // 修改群名称
	static const int OPCODE_ROOM_ANNOUNCEMENT = 0x42;  // 发送群公告
	static const int OPCODE_ROOM_MEMBER_LIST = 0x43;  // 获取群成员列表
	static const int OPCODE_ROOM_ADD_MEMBER = 0x44;  // 拉好友入群
	static const int OPCODE_ROOM_DELETE_MEMBER = 0x45;  // 删除群成员
	static const int OPCODE_ROOM_AT_MEMBER = 0x46;  // 艾特群成员
	static const int OPCODE_ROOM_QUIT = 0x47;  // 退出群聊
	static const int OPCODE_TRANSFER_RECV = 0x50;  // 收款
	static const int OPCODE_WECHAT_QRCODE = 0x90;  // 返回登录二维码地址
	static const int OPCODE_LOGIN_INFO = 0x91;  // 返回当前登录的微信号信息
	static const int OPCODE_MESSAGE_RECEIVE = 0x92;  // 返回接收的微信消息
};