<?php
namespace Wechat\App\Service\Receive;

use Wechat\App\Service\Transit;

/**
 * ws客户端准备完毕
 */
class ReceiveReady extends AbstractReceive
{
    /** @var string 定义body业务主体类，自动解析 */
    protected $bodyClass = \Wechat\App\Service\Receive\Body\BodyReady::class;

    /**
     * 处理消息
     */
    public function handle()
    {
        // 触发中转站
        Transit::wechatMessage($this->package);
    }
}
