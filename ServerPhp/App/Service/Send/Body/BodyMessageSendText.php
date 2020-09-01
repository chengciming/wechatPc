<?php
namespace Wechat\App\Service\Send\Body;

/**
 * 发送文本消息
 */
class BodyMessageSendText extends AbstractBody
{
    /** @var null 接收人微信ID */
    protected $wxid = null;
    /** @var null 内容 */
    protected $content = null;
}
