<?php
namespace Wechat\App\Service\Receive;

use Wechat\App\Library\Package;
use Wechat\App\Service\Receive\Body\AbstractBody;

abstract class AbstractReceive
{
    /** @var Package|null 数据包对象 */
    protected $package = null;

    /** @var AbstractBody|null body对象 */
    protected $body = null;
    /** @var null 定义body解析类 */
    protected $bodyClass = null;

    /** 处理事件 */
    abstract public function handle();

    /**
     * Listener constructor.
     */
    public function __construct()
    {

    }

    /**
     * 设置数据包
     * @param Package $package
     */
    public function setPackage(Package $package)
    {
        $this->package = $package;
        // 解析body业务参数
        if (!is_null($this->bodyClass)) {
            $this->body = new $this->bodyClass();
            $this->body->setBody((array)$this->package->getBody());
        }
    }
}
