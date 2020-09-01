<?php
namespace Wechat\App\Service\Send;

/**
 * 发送文本消息
 */
class SendMessageSendText extends AbstractSend
{
    /** @var null 定义body解析类，会自动解析 */
    protected $bodyClass = \Wechat\App\Service\Send\Body\BodyMessageSendText::class;
}
