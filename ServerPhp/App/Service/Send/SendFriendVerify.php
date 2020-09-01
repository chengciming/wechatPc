<?php
namespace Wechat\App\Service\Send;

/**
 * 同意加好友请求
 */
class SendFriendVerify extends AbstractSend
{
    /** @var null 定义body解析类，会自动解析 */
    protected $bodyClass = \Wechat\App\Service\Send\Body\BodyFriendVerify::class;
}
