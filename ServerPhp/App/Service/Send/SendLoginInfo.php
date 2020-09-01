<?php
namespace Wechat\App\Service\Send;


/**
 * 返回当前登录的微信号信息
 */
class SendLoginInfo extends AbstractSend
{
    /** @var null 定义body解析类，会自动解析 */
    protected $bodyClass = \Wechat\App\Service\Send\Body\BodyLoginInfo::class;
}
