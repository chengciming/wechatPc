<?php
namespace Wechat\App\Service\Send\Body;


abstract class AbstractBody
{
    /** @var array 业务主体 */
    protected $body = [];

    /**
     * Listener constructor.
     */
    public function __construct()
    {

    }

    /**
     * 实现自动化(getXxx获取xxx属性  或者  setXxx设置xxx属性)
     * @param $name
     * @param array $arguments
     * @return bool|null
     */
    public function __call($name, $arguments = [])
    {
        if (strlen($name) <= 3) {
            return false;
        }
        // 删除get或者set前缀
        $var = lcfirst(substr($name, 3));
        // 获取参数
        if (substr($name, 0, 3) == 'get') {
            return $this->$var ?? null;
        } else {
            if (isset($this->$var)) {
                $this->$var = $arguments[0] ?? null;
            }
        }
    }

    /**
     * 获取业务主体
     */
    public function getBody():array
    {
        return (array)$this->body;
    }

    /**
     * 设置业务主体
     * @param array $body
     */
    public function setBody(array $body)
    {
        $this->body = $body;
    }
}
