<?php
namespace Wechat\App\Service;

use Wechat\App\Enums\OpCode;
use Wechat\App\Library\ConnectionPool;
use Wechat\App\Library\ConnectionRelationPool;
use Wechat\App\Library\Package;
use Wechat\App\Library\Tools;
use Workerman\Connection\TcpConnection;

/**
 * 消息中转: <wechat> <<===>> <Transit> <<===>> <web>
 */
class Transit
{
    /** @var null 浏览器端配置 */
    public static $webListenAddress = null;
    public static $webListenPort = null;
    /** @var string 绑定关系的前缀 */
    protected static $webRelationSuffix = 'web_';

    /** @var null 微信端配置 */
    public static $wechatListenAddress = null;
    public static $wechatListenPort = null;
    /** @var string 绑定关系的前缀 */
    protected static $wechatRelationSuffix = 'wechat_';
    /** @var int 微信端开启个数，每个连接开启的微信个数 */
    protected static $wechatOpenNumber = [];
    /** @var array 微信客户端的APPID与APPKEY */
    protected static $wechatAppId = [];
    protected static $wechatAppKey = [];

    /**
     * 微信器端消息事件
     * @param Package $package
     * @return bool
     */
    public static function wechatMessage(Package $package)
    {
        // 保存微信端的信息
        $opCode = $package->getOpCode();
        switch ($opCode) {
            // ws客户端准备完毕
            case OpCode::OPCODE_READY:
                // 保存APPID和APPKEY
                self::$wechatAppId[$package->getConnection()->id] = $package->getAppId();
                self::$wechatAppKey[$package->getConnection()->id] = $package->getAppKey();
                // 获取微信客户端列表
                $body = $package->getBody();
                if (isset($body['wechatIdList']) && !empty($body['wechatIdList'])) {
                    $wechatIdList = explode(',', $body['wechatIdList']);
                    $loginStatusPackage = clone($package);
                    $connection = $package->getConnection();
                    // 绑定关系
                    foreach ($wechatIdList ?? [] as $wechatId) {
                        // TODO: 不存在浏览器，取第一个链接，暂时为了调试用的
                        $webConnection = ConnectionPool::getRand(self::$webListenPort);
                        echo $wechatId."\n\n\n";
                        // 绑定关系
                        self::bindWechatConnection($webConnection, $wechatId, $connection);
                        // 获取登录状态
                        $loginStatusPackage->setOpCode(OpCode::OPCODE_WECHAT_GET_LOGIN_STATUS);
                        $loginStatusPackage->setWechatId($wechatId);
                        $loginStatusPackage->setBody([]);
                        $loginStatusPackage->send();
                    }
                    // 初始化个数
                    self::$wechatOpenNumber[$package->getConnection()->id] = count($wechatIdList);
                } else {
                    // 初始化个数
                    self::$wechatOpenNumber[$package->getConnection()->id] = 0;
                }
                break;
        }
        $wechatId = $package->getWechatId();
        // 如果有微信ID，返回给浏览器端
        if (!empty($wechatId)) {
            // 构造返回给浏览器端的数据
            $data = [
                'wechatId' => $wechatId,
                'opCode' => $package->getOpCode(),
                'body' => $package->getBody(),
            ];
            $json = json_encode($data);
            // 查找浏览器端的连接
            $webConnectId = ConnectionRelationPool::getGroupId(self::$webRelationSuffix . $wechatId);
            if ($webConnectId) {
                $webConnectId = str_replace(self::$webRelationSuffix, '', $webConnectId);
                $webConnection = ConnectionPool::get($webConnectId, self::$webListenPort);
                // 转发数据
                if ($webConnection) {
                    $webConnection->send($json);
                } else {
                    Tools::log('Transit Wechat Message Error: Not Find Web Client' . 'ConnectId=' . $package->getConnection()->id . ', opCode=' . $package->getOpCode());
                    return false;
                }
            }
        }

        Tools::log('Transit Wechat Message: ' . 'ConnectId=' . $package->getConnection()->id . ', opCode=' . $package->getOpCode());
        return true;
    }

    /**
     * 微信器端连接事件
     * @param TcpConnection $connection
     */
    public static function wechatConnect($connection)
    {
        // 保存连接对象
        ConnectionPool::add($connection, self::$wechatListenPort);
        Tools::log('Transit Wechat Connect: ' . 'ConnectId=' . $connection->id);
    }

    /**
     * 微信端断开连接事件
     * @param TcpConnection $connection
     */
    public static function wechatClose($connection)
    {
        // 删除初始化个数
        if (isset(self::$wechatOpenNumber[$connection->id])) {
            unset(self::$wechatOpenNumber[$connection->id]);
        }
        // 删除APPID和APPKEY
        if (isset(self::$wechatAppId[$connection->id])) {
            unset(self::$wechatAppId[$connection->id]);
        }
        if (isset(self::$wechatAppKey[$connection->id])) {
            unset(self::$wechatAppKey[$connection->id]);
        }
        // 解绑微信端与微信ID关系
        ConnectionRelationPool::removeGroup(self::$wechatRelationSuffix . $connection->id);
        // 删除连接对象
        ConnectionPool::remove($connection, self::$wechatListenPort);
        Tools::log('Transit Wechat Close: ' . 'ConnectId=' . $connection->id);
    }

    /** =============================================================================================================================================== */

    /**
     * 浏览器端消息事件
     * @param Package $package
     * @return bool
     * @throws \ErrorException
     */
    public static function webMessage($package)
    {
        // 绑定关系与解绑关系
        $opCode = $package->getOpCode();
        switch ($opCode) {
            // 新开一个微信
            case OpCode::OPCODE_WECHAT_OPEN:
                if (empty(self::$wechatOpenNumber)) {
                    Tools::log('Transit Web Relation Not Wechat Online: ' . 'ConnectId=' . $package->getConnection()->id);
                    return false;
                }
                // 自动生成一个微信客户端ID
                $wechatId = strtoupper(md5(rand(100000, 999999) . Tools::timestamp() . rand(100000, 999999)));
                $package->setWechatId($wechatId);
                // 绑定关系
                self::bindWechatConnection($package->getConnection(), $wechatId);
                break;
        }
        $wechatId = $package->getWechatId();
        // 转发消息
        $wechatConnectId = ConnectionRelationPool::getGroupId(self::$wechatRelationSuffix . $wechatId);
        if (!$wechatConnectId) {
            Tools::log('Transit Web Not WechatConectionId: ' . 'ConnectId=' . $package->getConnection()->id);
            return false;
        }
        // 获取微信端连接对象
        $wechatConnectId = str_replace(self::$wechatRelationSuffix, '', $wechatConnectId);
        $wechatConnection = ConnectionPool::get($wechatConnectId, self::$wechatListenPort);
        if (!$wechatConnection) {
            Tools::log('Transit Web Not WechatConection: ' . 'ConnectId=' . $package->getConnection()->id);
            return false;
        }
        // 发送消息
        $sender = Send::getSender($package->getOpCode());
        if ($sender) {
            $sender->setAppId(self::$wechatAppId[$wechatConnectId] ?? null);
            $sender->setAppKey(self::$wechatAppKey[$wechatConnectId] ?? null);
            $sender->setConnection($wechatConnection);
            $sender->setWechatId($package->getWechatId());
            $sender->setOpCode($package->getOpCode());
            $sender->setBody($package->getBody());
            $sender->send();
            Tools::log('Transit Web Message: ' . 'ConnectId=' . $package->getConnection()->id);
            return true;
        }
        Tools::log('Transit Web Message: ' . 'ConnectId=' . $package->getConnection()->id . ', Invalid Send OpCode.');
        return false;
    }

    /**
     * 浏览器端连接事件
     * @param TcpConnection $connection
     */
    public static function webConnect($connection)
    {
        ConnectionPool::add($connection, self::$webListenPort);
        Tools::log('Transit Web Connect: ' . 'ConnectId=' . $connection->id);
    }

    /**
     * 浏览器端断开连接事件
     * @param TcpConnection $connection
     */
    public static function webClose($connection)
    {
        // 解绑浏览器端与微信ID关系
        ConnectionRelationPool::removeGroup(self::$webRelationSuffix . $connection->id);
        // 删除连接对象
        ConnectionPool::remove($connection, self::$webListenPort);
        Tools::log('Transit Web Close: ' . 'ConnectId=' . $connection->id);
    }

    /**
     * 绑定微信ID与终端关系
     * @param $webConnection
     * @param $wechatId
     * @param TcpConnection|null $wechatConnection
     */
    protected static function bindWechatConnection($webConnection, $wechatId, $wechatConnection = null)
    {
        // 绑定浏览器端与微信ID的关系
        ConnectionRelationPool::add(self::$webRelationSuffix . $wechatId, self::$webRelationSuffix . $webConnection->id);
        // 绑定微信端与微信ID的关系
        $wechatConnectId = !is_null($wechatConnection) ? $wechatConnection->id : Tools::getArrayKeyByMinValue(self::$wechatOpenNumber);
        ConnectionRelationPool::add(self::$wechatRelationSuffix . $wechatId, self::$wechatRelationSuffix . $wechatConnectId);
    }
}
