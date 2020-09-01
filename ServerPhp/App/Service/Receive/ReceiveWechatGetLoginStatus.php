<?php
namespace Wechat\App\Service\Receive;

use Wechat\App\Service\Transit;

/**
 * 获取微信客户端登录状态
 */
class ReceiveWechatGetLoginStatus extends AbstractReceive
{
    /** @var string 定义body业务主体类，自动解析 */
    protected $bodyClass = \Wechat\App\Service\Receive\Body\BodyWechatGetLoginStatus::class;

    /**
     * 处理消息
     */
    public function handle()
    {
        // 触发中转站
        Transit::wechatMessage($this->package);
    }
}
