<?php
namespace Wechat\App\Library;

use Wechat\App\Enums\OpCode;
use Workerman\Connection\TcpConnection;

/**
 * 数据包
 */
class Package
{
    /** @var null|TcpConnection 链接 */
    protected $connection = null;

    /** @var array 数据包内容 */
    protected $package = [
        'appId' => null,  // 客户端的应用ID，字符串，最长32位
        'appKey' => null,  // 客户端的应用KEY，字符串，最长32位
        'package' => [  // 数据包信息
            'uniqueId' => null,  // 数据包唯一ID，字符串，最长32位
            'timestamp' => null,  // 发送时间，毫秒级时间戳，字符串。13位
            'wechatId' => null,  // 接收指令的目标微信客户端ID，每开一个微信客户端带有一个唯一ID，无目标客户端则为null，字符串或null
            'opCode' => null,  // 操作指令，16进制数值
            'body' => null,  // 业务主体对象
        ],
    ];

    /**
     * Package constructor.
     */
    public function __construct()
    {

    }

    /**
     * 发送数据给客户端
     * @return bool|null
     * @throws \ErrorException
     */
    public function send()
    {
        // 检查必传参数
        $result = $this->check();
        if (!$result) {
            throw new \ErrorException('Package->send: 缺少必传参数！');
        }
        // 检查是否设置连接对象
        if (!$this->connection) {
            throw new \ErrorException('Package->send: 未设置连接对象！');
        }
        // 获取context
        $context = $this->getContext();
        return $this->connection->send($context);
    }

    /**
     * 设置客户端连接对象
     * @param TcpConnection $connection
     */
    public function setConnection(TcpConnection $connection)
    {
        $this->connection = $connection;
    }

    /**
     * 获取客户端连接对象
     * @return null|TcpConnection
     */
    public function getConnection(): TcpConnection
    {
        return $this->connection;
    }

    /**
     * 解包
     * @param $json
     */
    public function setConText($json)
    {
        try {
            $package = json_decode($json, true);

            $this->setAppId($package['appId'] ?? null);  // // 客户端的应用ID，字符串，最长32位
            $this->setAppKey($package['appKey'] ?? null);  // // 客户端的应用KEY，字符串，最长32位
            $this->setUniqueId($package['package']['uniqueId'] ?? null);  // 数据包唯一ID，字符串，最长32位
            $this->setTimestamp($package['package']['timestamp'] ?? null);  // 发送时间，毫秒级时间戳，数值
            $this->setWechatId($package['package']['wechatId'] ?? null);  // 接收指令的目标微信客户端ID，每开一个微信客户端带有一个唯一ID，无目标客户端则为null，字符串或null
            $this->setOpCode($package['package']['opCode'] ?? null);  // 操作指令，16进制数值
            $this->setBody($package['package']['body'] ?? null);  // 业务主体对象
        } catch (\ErrorException $exception) {
            Tools::log('Error： function setConText. ['.$exception->getCode().'] ' . $exception->getMessage());
        }
    }

    /**
     * 封包
     * @return string
     */
    public function getContext()
    {
        // 如果没设置时间戳，自动设置
        $this->createTimestamp();
        // 如果没设置消息ID，自动设置
        $this->createUniqueId();
        // 返回json数据
        return json_encode($this->package);
    }

    /**
     * 检查包的必传参数
     * @return bool
     */
    public function check()
    {
        // 检查应用ID
        $appId = $this->getAppId();
        if (is_null($appId)) {
            return false;
        }
        // 检查应用KEY
        $appKey = $this->getAppKey();
        if (is_null($appKey)) {
            return false;
        }
        // 检查操作指令
        $opCode = $this->getOpCode();
        if (is_null($opCode) || !in_array($opCode, OpCode::$allowOpCode)) {
            return false;
        }
        return true;
    }

    /**
     * 生成时间戳 - 毫秒级
     * @param bool $autoSet
     * @return string
     */
    public function createTimestamp($autoSet = true)
    {
        $timestamp = Tools::timestamp();
        if ($autoSet) {
            $this->setTimestamp($timestamp);
        }
        return (string)$timestamp;
    }

    /**
     * 生成包ID
     * @param bool $autoSet
     * @return string
     */
    public function createUniqueId($autoSet = true)
    {
        $uniqueId = strtoupper(md5(Tools::timestamp() . rand(100000, 999999)));
        if ($autoSet) {
            $this->setUniqueId($uniqueId);
        }
        return $uniqueId;
    }

    /**
     * @return string
     */
    public function getUniqueId()
    {
        return $this->package['package']['uniqueId'];
    }

    /**
     * @param string $uniqueId
     */
    public function setUniqueId($uniqueId = null)
    {
        $this->package['package']['uniqueId'] = $uniqueId;
    }

    /**
     * @return string
     */
    public function getTimestamp()
    {
        return (string)$this->package['package']['timestamp'];
    }

    /**
     * @param string $timestamp
     */
    public function setTimestamp($timestamp): void
    {
        $this->package['package']['timestamp'] = (string)$timestamp;
    }

    /**
     * @return array
     */
    public function getBody()
    {
        $body = $this->package['package']['body'];
        $body = empty($body) ? [] : $body;
        return $body;
    }

    /**
     * @param array $body
     */
    public function setBody($body)
    {
        $body = empty($body) ? (object)[] : $body;
        $this->package['package']['body'] = $body;
    }

    /**
     * @return int
     */
    public function getOpCode()
    {
        return $this->package['package']['opCode'];
    }

    /**
     * @param int $opCode
     */
    public function setOpCode($opCode)
    {
        $this->package['package']['opCode'] = $opCode;
    }

    /**
     * @return string
     */
    public function getWechatId()
    {
        return $this->package['package']['wechatId'];
    }

    /**
     * @param string $wechatId
     */
    public function setWechatId($wechatId)
    {
        $wechatId = $wechatId ? $wechatId : null;
        $this->package['package']['wechatId'] = $wechatId;
    }

    /**
     * @return null
     */
    public function getAppId()
    {
        return $this->package['appId'];
    }

    /**
     * @param string $appId
     */
    public function setAppId($appId)
    {
        $this->package['appId'] = $appId;
    }

    /**
     * @return string
     */
    public function getAppKey()
    {
        return $this->package['appKey'];
    }

    /**
     * @param string $appKey
     */
    public function setAppKey($appKey)
    {
        $this->package['appKey'] = $appKey;
    }
}
