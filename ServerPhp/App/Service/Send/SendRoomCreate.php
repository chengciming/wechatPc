<?php
namespace Wechat\App\Service\Send;

/**
 * 创建群聊
 */
class SendRoomCreate extends AbstractSend
{
    /** @var null 定义body解析类，会自动解析 */
    protected $bodyClass = \Wechat\App\Service\Send\Body\BodyRoomCreate::class;
}
