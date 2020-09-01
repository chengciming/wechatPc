<?php
namespace Wechat\App\Service\Listener;

use Wechat\App\Library\Tools;

/**
 * 报错
 */
class Error extends AbstractListener
{
    /** ==========================================顶部是参数区，自动设置参数=================================================*/
    /** @var null|\Workerman\Connection\TcpConnection 连接对象 */
    protected $connection = null;
    /** @var null 错误码 */
    protected $code = null;
    /** @var null 错误信息 */
    protected $msg = null;
    /** ==========================================顶部是参数区，自动设置参数=================================================*/

    /**
     * 处理事件
     */
    public function handle()
    {
        // 打印日记
        Tools::log('Error: ' . 'ConnectId=' . $this->connection->id . ', Message=' . $this->code . ' - ' . $this->msg);
    }
}
