<?php
namespace Wechat\App\Enums;

/**
 * 操作指令
 */
class OpCode
{
    /** @var int 响应指令 */
    const OPCODE_READY = 0x0;  // ws客户端准备完毕
    const OPCODE_SUCCESS = 0x1;  // 操作成功
    const OPCODE_FAILURE = 0x2;  // 操作失败
    /** @var int 业务指令 */
    const OPCODE_WECHAT_OPEN = 0x10;  // 新开一个微信客户端
    const OPCODE_WECHAT_QUIT = 0x11;  // 退出一个微信
    const OPCODE_WECHAT_GET_LOGIN_STATUS = 0x12;  // 获取登录状态
    const OPCODE_MESSAGE_SEND_TEXT = 0x20;  // 发送文本消息
    const OPCODE_MESSAGE_SEND_IMAGE = 0x21;  // 发送图片消息
    const OPCODE_MESSAGE_SEND_FILE = 0x22;  // 发送附件消息
    const OPCODE_MESSAGE_SEND_CARD = 0x23;  // 发送名片消息
    const OPCODE_MESSAGE_SEND_XML = 0x24;  // 发送xml消息
    const OPCODE_FRIEND_ADD = 0x30;  // wxid加好友
    const OPCODE_FRIEND_ADD_FROM_V1 = 0x31;  // v1加好友
    const OPCODE_FRIEND_DELETE = 0x32;  // 删除好友
    const OPCODE_FRIEND_VERIFY = 0x33;  // v1+v2同意好友请求
    const OPCODE_FRIEND_LIST = 0x34;  // 好友列表
    const OPCODE_FRIEND_REMARK = 0x35;  // 修改备注
    const OPCODE_ROOM_CREATE = 0x40;  // 创建群聊
    const OPCODE_ROOM_EDIT_NAME = 0x41;  // 修改群名称
    const OPCODE_ROOM_ANNOUNCEMENT = 0x42;  // 发送群公告
    const OPCODE_ROOM_MEMBER_LIST = 0x43;  // 获取群成员列表
    const OPCODE_ROOM_ADD_MEMBER = 0x44;  // 拉好友入群
    const OPCODE_ROOM_DELETE_MEMBER = 0x45;  // 删除群成员
    const OPCODE_ROOM_AT_MEMBER = 0x46;  // 艾特群成员
    const OPCODE_ROOM_QUIT = 0x47;  // 退出群聊
    const OPCODE_TRANSFER_RECV = 0x50;  // 收款
    const OPCODE_WECHAT_QRCODE = 0x90;  // 返回登录二维码地址
    const OPCODE_LOGIN_INFO = 0x91;  // 返回当前登录的微信号信息
    const OPCODE_MESSAGE_RECEIVE = 0x92;  // 返回接收的微信消息

    /** @var array 允许的操作指令 */
    public static $allowOpCode = [
        /** @var int 响应指令 */
        self::OPCODE_READY,  // ws客户端准备完毕
        self::OPCODE_SUCCESS,  // 操作正常
        self::OPCODE_FAILURE,  // 操作失败
        /** @var int 业务指令 */
        self::OPCODE_WECHAT_OPEN,  // 新开一个微信客户端
        self::OPCODE_WECHAT_QUIT,  // 退出一个微信
        self::OPCODE_WECHAT_GET_LOGIN_STATUS,  // 获取登录状态
        self::OPCODE_MESSAGE_SEND_TEXT,  // 发送文本消息
        self::OPCODE_MESSAGE_SEND_IMAGE,  // 发送图片消息
        self::OPCODE_MESSAGE_SEND_FILE,  // 发送附件消息
        self::OPCODE_MESSAGE_SEND_CARD,  // 发送名片消息
        self::OPCODE_MESSAGE_SEND_XML,  // 发送xml消息
        self::OPCODE_FRIEND_ADD,  // wxid加好友
        self::OPCODE_FRIEND_ADD_FROM_V1,  // v1加好友
        self::OPCODE_FRIEND_DELETE,  // 删除好友
        self::OPCODE_FRIEND_VERIFY,  // v1+v2同意好友请求
        self::OPCODE_FRIEND_LIST,  // 好友列表
        self::OPCODE_FRIEND_REMARK,  // 修改备注
        self::OPCODE_ROOM_CREATE,  // 创建群聊
        self::OPCODE_ROOM_EDIT_NAME,  // 修改群名称
        self::OPCODE_ROOM_ANNOUNCEMENT,  // 发送群公告
        self::OPCODE_ROOM_MEMBER_LIST,  // 获取群成员列表
        self::OPCODE_ROOM_ADD_MEMBER,  // 拉好友入群
        self::OPCODE_ROOM_DELETE_MEMBER,  // 删除群成员
        self::OPCODE_ROOM_AT_MEMBER,  // 艾特群成员
        self::OPCODE_ROOM_QUIT,  // 退出群聊
        self::OPCODE_TRANSFER_RECV,  // 收款
        self::OPCODE_WECHAT_QRCODE,  // 返回登录二维码地址
        self::OPCODE_LOGIN_INFO,  // 返回当前登录的微信号信息
        self::OPCODE_MESSAGE_RECEIVE,  // 返回接收的微信消息
    ];
}
