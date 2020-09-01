<?php
namespace Wechat\App\Service;

use Wechat\App\Enums\OpCode;
use Wechat\App\Service\Send\AbstractSend;
use Wechat\App\Service\Send\SendFriendAdd;
use Wechat\App\Service\Send\SendFriendAddFromV1;
use Wechat\App\Service\Send\SendFriendDelete;
use Wechat\App\Service\Send\SendFriendList;
use Wechat\App\Service\Send\SendFriendRemark;
use Wechat\App\Service\Send\SendFriendVerify;
use Wechat\App\Service\Send\SendLoginInfo;
use Wechat\App\Service\Send\SendMessageSendCard;
use Wechat\App\Service\Send\SendMessageSendFile;
use Wechat\App\Service\Send\SendMessageSendImage;
use Wechat\App\Service\Send\SendMessageSendText;
use Wechat\App\Service\Send\SendMessageSendXml;
use Wechat\App\Service\Send\SendRoomAddMember;
use Wechat\App\Service\Send\SendRoomAnnouncement;
use Wechat\App\Service\Send\SendRoomAtMember;
use Wechat\App\Service\Send\SendRoomCreate;
use Wechat\App\Service\Send\SendRoomDeleteMember;
use Wechat\App\Service\Send\SendRoomEditName;
use Wechat\App\Service\Send\SendRoomMemberList;
use Wechat\App\Service\Send\SendRoomQuit;
use Wechat\App\Service\Send\SendTransferRecv;
use Wechat\App\Service\Send\SendWechatGetLoginStatus;
use Wechat\App\Service\Send\SendWechatOpen;
use Wechat\App\Service\Send\SendWechatQuit;

/**
 * 接收消息
 */
class Send
{
    /** @var array 操作对象列表 */
    public $objectList = [
        /** @var int 业务指令 */
        OpCode::OPCODE_WECHAT_OPEN => SendWechatOpen::class,  // 新开一个微信客户端
        OpCode::OPCODE_WECHAT_QUIT => SendWechatQuit::class,  // 退出一个微信
        OpCode::OPCODE_WECHAT_GET_LOGIN_STATUS => SendWechatGetLoginStatus::class,  // 获取登录状态
        OpCode::OPCODE_LOGIN_INFO => SendLoginInfo::class,  // 获取登录详情
        OpCode::OPCODE_MESSAGE_SEND_TEXT => SendMessageSendText::class,  // 发送文本消息
        OpCode::OPCODE_MESSAGE_SEND_IMAGE => SendMessageSendImage::class,  // 发送图片消息
        OpCode::OPCODE_MESSAGE_SEND_FILE => SendMessageSendFile::class,  // 发送附件消息
        OpCode::OPCODE_MESSAGE_SEND_CARD => SendMessageSendCard::class,  // 发送名片消息
        OpCode::OPCODE_MESSAGE_SEND_XML => SendMessageSendXml::class,  // 发送xml消息
        OpCode::OPCODE_FRIEND_LIST => SendFriendList::class,  // 获取好友列表
        OpCode::OPCODE_FRIEND_REMARK => SendFriendRemark::class,  // 修改备注
        OpCode::OPCODE_FRIEND_ADD => SendFriendAdd::class,  // wxid加好友
        OpCode::OPCODE_FRIEND_ADD_FROM_V1 => SendFriendAddFromV1::class,  // v1加好友
        OpCode::OPCODE_FRIEND_DELETE => SendFriendDelete::class,  // 删除好友
        OpCode::OPCODE_FRIEND_VERIFY => SendFriendVerify::class,  // v1+v2同意好友请求
        OpCode::OPCODE_ROOM_CREATE => SendRoomCreate::class,  // 创建群聊
        OpCode::OPCODE_ROOM_EDIT_NAME => SendRoomEditName::class,  // 修改群名称
        OpCode::OPCODE_ROOM_ANNOUNCEMENT => SendRoomAnnouncement::class,  // 发送群公告
        OpCode::OPCODE_ROOM_MEMBER_LIST => SendRoomMemberList::class,  // 获取群成员列表
        OpCode::OPCODE_ROOM_ADD_MEMBER => SendRoomAddMember::class,  // 拉好友入群
        OpCode::OPCODE_ROOM_DELETE_MEMBER => SendRoomDeleteMember::class,  // 删除群成员
        OpCode::OPCODE_ROOM_AT_MEMBER => SendRoomAtMember::class,  // 艾特群成员
        OpCode::OPCODE_ROOM_QUIT => SendRoomQuit::class,  // 退出群聊
        OpCode::OPCODE_TRANSFER_RECV => SendTransferRecv::class,  // 收款
    ];

    /** @var array 对象储存 */
    public $object = [];

    /** @var object 单例模式 */
    private static $instance;

    /**
     * 获取发送对象
     * @param $opCode
     * @return bool|AbstractSend
     */
    public static function getSender($opCode)
    {
        if (!isset(self::$instance)) {
            self::$instance = new static();
        }
        if (!isset(self::$instance->objectList[$opCode])) {
            return false;
        }
        if (!isset(self::$instance->object[$opCode])) {
            self::$instance->object[$opCode] = new self::$instance->objectList[$opCode]();
        }
        self::$instance->object[$opCode]->setOpCode($opCode);
        return self::$instance->object[$opCode];
    }
}
