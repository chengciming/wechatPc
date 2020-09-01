<?php
namespace Wechat\App\Service;

use Wechat\App\Enums\OpCode;
use Wechat\App\Library\Package;
use Wechat\App\Service\Receive\AbstractReceive;
use Wechat\App\Service\Receive\ReceiveFailure;
use Wechat\App\Service\Receive\ReceiveFriendList;
use Wechat\App\Service\Receive\ReceiveLoginInfo;
use Wechat\App\Service\Receive\ReceiveMessageReceive;
use Wechat\App\Service\Receive\ReceiveReady;
use Wechat\App\Service\Receive\ReceiveRoomMemberList;
use Wechat\App\Service\Receive\ReceiveSuccess;
use Wechat\App\Service\Receive\ReceiveWechatGetLoginStatus;
use Wechat\App\Service\Receive\ReceiveWechatOpen;
use Wechat\App\Service\Receive\ReceiveWechatQrcode;
use Wechat\App\Service\Receive\ReceiveWechatQuit;

/**
 * 接收消息
 */
class Receive
{
    /** @var array 操作对象实例 */
    protected $object = [];

    /** @var array 操作对象列表 */
    protected $objectList = [
        /** @var int 响应指令 */
        OpCode::OPCODE_READY => ReceiveReady::class,  // ws客户端准备完毕
        OpCode::OPCODE_SUCCESS => ReceiveSuccess::class,  // 操作正常
        OpCode::OPCODE_FAILURE => ReceiveFailure::class,  // 操作失败
        /** @var int 业务指令 */
        OpCode::OPCODE_WECHAT_OPEN => ReceiveWechatOpen::class,  // 新开一个微信客户端
        OpCode::OPCODE_WECHAT_QUIT => ReceiveWechatQuit::class,  // 退出一个微信
        OpCode::OPCODE_WECHAT_GET_LOGIN_STATUS => ReceiveWechatGetLoginStatus::class,  // 获取登录状态
        OpCode::OPCODE_FRIEND_LIST=> ReceiveFriendList::class,  // 好友列表
        OpCode::OPCODE_ROOM_MEMBER_LIST => ReceiveRoomMemberList::class,  // 获取群成员列表
        OpCode::OPCODE_WECHAT_QRCODE => ReceiveWechatQrcode::class,  // 返回登录二维码地址
        OpCode::OPCODE_LOGIN_INFO => ReceiveLoginInfo::class,  // 返回当前登录的微信号信息
        OpCode::OPCODE_MESSAGE_RECEIVE => ReceiveMessageReceive::class,  // 返回接收的微信消息
    ];

    /**
     * 设置数据包
     * @param Package $package
     * @return bool|AbstractReceive
     */
    public function setPackage(Package $package)
    {
        return $this->initObject($package);
    }

    /**
     * 初始化对象
     * @param Package $package
     * @return bool|AbstractReceive
     */
    protected function initObject(Package $package)
    {
        $opCode = $package->getOpCode();
        if (!isset($this->objectList[$opCode])) {
            return false;
        }
        // TODO: 这里可能需要做appid的准入限制
        //$appId = $package->getAppId();
        //$appKey = $package->getAppKey();

        if (!isset($this->object[$opCode])) {
            $this->object[$opCode] = new $this->objectList[$opCode]();
        }
        $this->object[$opCode]->setPackage($package);
        return $this->object[$opCode];
    }
}
