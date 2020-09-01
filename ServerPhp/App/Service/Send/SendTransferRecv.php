<?php
namespace Wechat\App\Service\Send;

/**
 * 收款
 */
class SendTransferRecv extends AbstractSend
{
    /** @var null 定义body解析类，会自动解析 */
    protected $bodyClass = \Wechat\App\Service\Send\Body\BodyTransferRecv::class;
}
