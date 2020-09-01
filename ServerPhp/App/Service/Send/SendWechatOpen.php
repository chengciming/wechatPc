<?php
namespace Wechat\App\Service\Send;

/**
 * 新开启一个微信客户端
 */
class SendWechatOpen extends AbstractSend
{
    /** @var null 定义body解析类，会自动解析 */
    protected $bodyClass = \Wechat\App\Service\Send\Body\BodyWechatOpen::class;
}
