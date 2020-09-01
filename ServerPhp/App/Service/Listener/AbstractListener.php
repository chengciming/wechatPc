<?php
namespace Wechat\App\Service\Listener;

abstract class AbstractListener
{
    /** 处理事件 */
    abstract public function handle();

    /**
     * Listener constructor.
     */
    public function __construct()
    {

    }

    /**
     * 储存参数
     * @param array $arguments
     */
    public function arguments($arguments = [])
    {
        $allParams = get_class_vars(get_class($this));
        $i = 0;
        foreach ($allParams ?? [] as $var=>$defaultValue) {
            if (!isset($arguments[$i])) {
                break;
            }
            $this->$var = $arguments[$i];
            $i++;
        }
    }
}
