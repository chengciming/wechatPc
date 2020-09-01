<?php
namespace Wechat\App\Service\Listener;

use Wechat\App\Library\Package;
use Wechat\App\Library\Tools;
use Wechat\App\Service\Receive;

/**
 * 接收消息
 */
class Message extends AbstractListener
{
    /** ==========================================顶部是参数区，自动设置参数=================================================*/
    /** @var null|\Workerman\Connection\TcpConnection 连接对象 */
    protected $connection = null;
    /** @var null 消息 */
    protected $message = null;
    /** ==========================================顶部是参数区，自动设置参数=================================================*/

    /** @var Receive|null 接收数据处理类 */
    private $receive = null;

    /**
     * Message constructor.
     */
    public function __construct()
    {
        $this->receive = new Receive();
    }

    /**
     * 处理事件
     */
    public function handle()
    {
        // 初始化包对象
        $package = new Package();
        // 解包
        $package->setConText($this->message);
        // 设置连接对象
        $package->setConnection($this->connection);
        // 检查包
        $result = $package->check();
        if (!$result) {
            Tools::log('Recv Error Package: ConnectId=' . $this->connection->id . ', Data=' . $this->message);
            return false;
        }
        // 打印日记
        Tools::log('Recv Data: ' . $this->connection->id . ', Data: ' . $this->message);

        // 处理
        $object = $this->receive->setPackage($package);
        if ($object) {
            $object->handle();
        }
    }
}
