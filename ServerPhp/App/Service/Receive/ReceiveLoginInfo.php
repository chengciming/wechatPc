<?php
namespace Wechat\App\Service\Receive;

use Wechat\App\Service\Transit;

/**
 * 返回当前登录的微信号信息
 */
class ReceiveLoginInfo extends AbstractReceive
{
    /** @var string 定义body业务主体类，自动解析 */
    protected $bodyClass = \Wechat\App\Service\Receive\Body\BodyLoginInfo::class;

    /**
     * 处理消息
     */
    public function handle()
    {
        // 触发中转站
        Transit::wechatMessage($this->package);
    }
}
