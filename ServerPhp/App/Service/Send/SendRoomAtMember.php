<?php
namespace Wechat\App\Service\Send;

/**
 * 发送@群成员
 */
class SendRoomAtMember extends AbstractSend
{
    /** @var null 定义body解析类，会自动解析 */
    protected $bodyClass = \Wechat\App\Service\Send\Body\BodyRoomAtMember::class;
}
