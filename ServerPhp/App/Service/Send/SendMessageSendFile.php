<?php
namespace Wechat\App\Service\Send;

/**
 * 发送附件消息
 */
class SendMessageSendFile extends AbstractSend
{
    /** @var null 定义body解析类，会自动解析 */
    protected $bodyClass = \Wechat\App\Service\Send\Body\BodyMessageSendFile::class;
}
