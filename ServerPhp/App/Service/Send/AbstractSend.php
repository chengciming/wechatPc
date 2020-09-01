<?php
namespace Wechat\App\Service\Send;

use Wechat\App\Library\Package;
use Wechat\App\Service\Send\Body\AbstractBody;

abstract class AbstractSend extends Package
{
    /** @var AbstractBody|null body对象 */
    protected $body = null;
    /** @var null 定义body解析类 */
    protected $bodyClass = null;

    /**
     * 发送数据给客户端
     * @return bool|null
     * @throws \ErrorException
     */
    public function send()
    {
        // 将包里面的业务参数与body对象的业务参数合并
        $packBody = $this->getBody();
        $body = $this->body->getBody();
        if (!empty($body)) {
            $packBody = array_merge($body, $packBody);
        }
        // 设置业务主体
        $this->setBody($packBody);
        // 发送
        return parent::send();
    }

    /**
     * AbstractSend constructor.
     */
    public function __construct()
    {
        // 初始化业务主体参数类
        if ($this->bodyClass) {
            $this->body = new $this->bodyClass();
        }
    }

    /**
     * 实现body业务主体参数的自动化（getXxx获取xxx属性  或者  setXxx设置xxx属性）
     * @param $name
     * @param array $arguments
     * @return bool|null
     */
    public function __call($name, $arguments = [])
    {
        echo $name."\n\n";
        if (strlen($name) <= 3) {
            return false;
        }
        return $this->body->$name(...$arguments);
    }
}
