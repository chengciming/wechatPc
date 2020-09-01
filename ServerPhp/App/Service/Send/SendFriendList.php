<?php
namespace Wechat\App\Service\Send;

/**
 * 获取好友列表
 */
class SendFriendList extends AbstractSend
{
    /** @var null 定义body解析类，会自动解析 */
    protected $bodyClass = \Wechat\App\Service\Send\Body\BodyFriendList::class;
}
