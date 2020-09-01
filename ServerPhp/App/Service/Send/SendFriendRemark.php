<?php
namespace Wechat\App\Service\Send;

/**
 * 设置好友备注
 */
class SendFriendRemark extends AbstractSend
{
    /** @var null 定义body解析类，会自动解析 */
    protected $bodyClass = \Wechat\App\Service\Send\Body\BodyFriendRemark::class;
}
