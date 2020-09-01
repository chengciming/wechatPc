<?php

namespace Wechat\App\Service\Listener;

/**
 * Class Factory.
 *
 * @method static WorkerStart WorkerStart()
 * @method static Error Error($connection, $code, $msg)
 * @method static Connect Connect($connection)
 * @method static Close Close($connection)
 * @method static Message Message($connection, $message)
 */
class Factory
{
    /** @var 单例模式 */
    private static $instance;

    /** @var array 事件类单例模式 */
    private $object = [];

    /**
     * Dynamically pass methods to the application.
     *
     * @param string $name
     * @param array $arguments
     *
     * @return mixed
     */
    public static function __callStatic($name, $arguments)
    {
        $obj = self::getInstance ();
        return $obj->make ($name, $arguments);
    }

    /**
     * 单例获取当前对象
     * @return static
     */
    protected static function getInstance()
    {
        if (!isset(self::$instance)) {
            self::$instance = new static();
        }
        return self::$instance;
    }

    /**
     * @param $name
     * @param array $arguments
     * @return mixed
     */
    public function make($name, array $arguments = [])
    {
        if (!isset($this->object[$name])) {
            $objectPath = get_class($this);
            $objectPath = explode('\\', $objectPath);
            unset($objectPath[count($objectPath)-1]);
            $objectPath = '\\' . implode('\\', $objectPath) . '\\' . $name;
            $this->object[$name] = new $objectPath();
        }
        $this->object[$name]->arguments($arguments);
        return $this->object[$name];
    }
}
