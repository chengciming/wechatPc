<?php
namespace Wechat\App\Service\Send;

/**
 * 返回群成员列表
 */
class SendRoomMemberList extends AbstractSend
{
    /** @var null 定义body解析类，会自动解析 */
    protected $bodyClass = \Wechat\App\Service\Send\Body\BodyRoomMemberList::class;
}
