<?php
namespace Wechat\App\Service;

use Wechat\App\Service\Listener\Factory;

/**
 * 接收消息
 */
class Listener
{
    /**
     * 进程启动完成
     */
    public function workerStart()
    {
        Factory::WorkerStart()->handle();
    }

    /**
     * 报错
     *
     * @param $connection
     * @param $code
     * @param $msg
     * @return void
     */
    public function onError($connection, $code, $msg)
    {
        Factory::Error($connection, $code, $msg)->handle();
    }

    /**
     * 关闭连接事件
     *
     * @param \Workerman\Connection\TcpConnection $connection
     * @return void
     */
    public function onClose($connection)
    {
        Factory::Close($connection)->handle();
    }

    /**
     * 连接事件
     *
     * @param \Workerman\Connection\TcpConnection $connection
     * @return void
     */
    public function onConnect($connection)
    {
        Factory::Connect($connection)->handle();
    }

    /**
     * 收到消息事件
     *
     * @param \Workerman\Connection\TcpConnection $connection
     * @param $message
     * @return void
     */
    public function onMessage($connection, $message)
    {
        echo 'Recv Data: ' . $message."\n";
        Factory::Message($connection, $message)->handle();
    }
}
