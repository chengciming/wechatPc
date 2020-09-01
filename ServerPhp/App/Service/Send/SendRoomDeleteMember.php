<?php
namespace Wechat\App\Service\Send;

/**
 * 删除群成员
 */
class SendRoomDeleteMember extends AbstractSend
{
    /** @var null 定义body解析类，会自动解析 */
    protected $bodyClass = \Wechat\App\Service\Send\Body\BodyRoomDeleteMember::class;
}
