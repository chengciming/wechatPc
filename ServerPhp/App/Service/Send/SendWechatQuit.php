<?php
namespace Wechat\App\Service\Send;

/**
 * 退出微信客户端
 */
class SendWechatQuit extends AbstractSend
{
    /** @var null 定义body解析类，会自动解析 */
    protected $bodyClass = \Wechat\App\Service\Send\Body\BodyWechatQuit::class;
}
