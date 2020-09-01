<?php
namespace Wechat\App\Service\Listener;

use Wechat\App\Library\Tools;
use Wechat\App\Service\Transit;

/**
 * ws客户端断开链接
 */
class Close extends AbstractListener
{
    /** ==========================================顶部是参数区，自动设置参数=================================================*/
    /** @var null|\Workerman\Connection\TcpConnection 连接对象 */
    protected $connection = null;
    /** ==========================================顶部是参数区，自动设置参数=================================================*/

    /**
     * 处理事件
     */
    public function handle()
    {
        // 触发中转站的处理
        Transit::wechatClose($this->connection);
    }
}
