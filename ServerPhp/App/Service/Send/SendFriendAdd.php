<?php
namespace Wechat\App\Service\Send;

/**
 * 添加好友
 */
class SendFriendAdd extends AbstractSend
{
    /** @var null 定义body解析类，会自动解析 */
    protected $bodyClass = \Wechat\App\Service\Send\Body\BodyFriendAdd::class;
}
