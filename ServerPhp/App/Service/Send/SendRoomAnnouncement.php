<?php
namespace Wechat\App\Service\Send;

/**
 * 发送群公告
 */
class SendRoomAnnouncement extends AbstractSend
{
    /** @var null 定义body解析类，会自动解析 */
    protected $bodyClass = \Wechat\App\Service\Send\Body\BodyRoomAnnouncement::class;
}
