<?php
/**
 * 服务端应用类
 */
namespace Wechat\App;

use Wechat\App\Library\Tools;
use Wechat\App\Service\Listener;
use Wechat\App\Service\Transit;
use Workerman\Worker;

class Server
{
    /** @var Worker|null 工作进程 */
    protected $worker = null;

    /**
     * 初始化
     */
    public function handle()
    {
        // 加载配置
        $listenConfig = Tools::config('listen');
        $workerConfig = Tools::config('worker');

        // 设置中转站配置
        Transit::$webListenAddress = $listenConfig['ipaddress'] ?? null;
        Transit::$webListenPort = $listenConfig['port'] ?? null;

        $listener = new Listener();

        //设置监听端口
        $this->worker = new Worker("websocket://".$listenConfig['ipaddress'].":" . $listenConfig['port']);
        $this->worker->reusePort = true;  // 允许不同进程共同监听
        $this->worker->count = $workerConfig['worker_num'];
        $this->worker->name = 'Server:' . $workerConfig['name'];
        Worker::$pidFile = $workerConfig['pid_file'];
        Worker::$logFile = $workerConfig['log_file'];
        Worker::$daemonize = $workerConfig['daemonize'];
        if(isset($workerConfig['stdout_file']) && !empty($workerConfig['stdout_file'])){
            Worker::$stdoutFile = $workerConfig['stdout_file'];
        }
        $this->worker->onWorkerStart = array($listener, 'workerStart');
        $this->worker->onMessage = array($listener, 'onMessage') ;
        $this->worker->onClose = array($listener, 'onClose');
        $this->worker->onConnect = array($listener, 'onConnect');
        $this->worker->onError = array($listener, 'onError');

        // 启动进程
        Worker::runAll();
    }

    /**
     * 初始化
     *
     * @return void
     */
    public static function run()
    {
        $object = new self();
        $object->handle();
    }
}