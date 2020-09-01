<?php
namespace Wechat\App\Service\Receive;

use Wechat\App\Service\Transit;

/**
 * 好友列表
 */
class ReceiveFriendList extends AbstractReceive
{
    /** @var string 定义body业务主体类，自动解析 */
    protected $bodyClass = \Wechat\App\Service\Receive\Body\BodyFriendList::class;

    /**
     * 处理消息
     */
    public function handle()
    {
        // 触发中转站
        Transit::wechatMessage($this->package);
    }
}
