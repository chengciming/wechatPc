<?php
namespace Wechat\App\Service\Send;

/**
 * 使用v1添加好友
 */
class SendFriendAddFromV1 extends AbstractSend
{
    /** @var null 定义body解析类，会自动解析 */
    protected $bodyClass = \Wechat\App\Service\Send\Body\BodyFriendAddFromV1::class;
}
