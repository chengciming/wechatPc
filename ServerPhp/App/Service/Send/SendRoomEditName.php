<?php
namespace Wechat\App\Service\Send;

/**
 * 编辑群名称
 */
class SendRoomEditName extends AbstractSend
{
    /** @var null 定义body解析类，会自动解析 */
    protected $bodyClass = \Wechat\App\Service\Send\Body\BodyRoomEditName::class;
}
