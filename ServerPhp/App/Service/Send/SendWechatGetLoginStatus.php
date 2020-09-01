<?php
namespace Wechat\App\Service\Send;

/**
 * 获取微信客户端登录状态
 */
class SendWechatGetLoginStatus extends AbstractSend
{
    /** @var null 定义body解析类，会自动解析 */
    protected $bodyClass = \Wechat\App\Service\Send\Body\BodyWechatGetLoginStatus::class;
}
