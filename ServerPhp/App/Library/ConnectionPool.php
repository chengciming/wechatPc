<?php
namespace Wechat\App\Library;

/**
 * TCP连接对象池
 */
class ConnectionPool
{
    /** @var array 链接储存 */
    public static $connections = array();

    /**
     * 添加连接对象
     *
     * @param \Workerman\Connection\TcpConnection $connection
     * @param string $port
     * @return bool
     */
    public static function add($connection, $port = '0')
    {
        if (!$connection) {
            return false;
        }
        if (!isset(self::$connections[$port])) {
            self::$connections[$port] = array();
        }
        // 获取连接ID
        $id = self::getId($connection);
        // 储存连接对象
        self::$connections[$port][$id] = $connection;
        return true;
    }

    /**
     * 删除连接对象
     *
     * @param \Workerman\Connection\TcpConnection $connection
     * @param string $port
     * @return bool
     */
    public static function remove($connection, $port = '0')
    {
        if (!$connection || !isset(self::$connections[$port])) {
            return false;
        }
        // 获取连接ID
        $id = self::getId($connection);
        // 连接对象不存在
        if (!isset(self::$connections[$port][$id])) {
            return false;
        }
        unset(self::$connections[$port][$id]);
        if (empty(self::$connections[$port])) {
            unset(self::$connections[$port]);
        }
        return true;
    }

    /**
     * 清除某个端口的所有连接
     *
     * @param string $port
     * @return bool
     */
    public static function removeAll($port = '0')
    {
        if (!isset(self::$connections[$port])) {
            return false;
        }
        unset(self::$connections[$port]);
        return true;
    }

    /**
     * 清除所有连接
     *
     * @return bool
     */
    public static function clear()
    {
        self::$connections = array();
        return true;
    }

    /**
     * 获取连接对象
     *
     * @param int $id
     * @param string $port
     * @return mixed|null
     */
    public static function get($id = 0, $port = '0')
    {
        if (!isset(self::$connections[$port])) {
            self::$connections[$port] = array();
        }
        // 连接对象不存在
        if (!isset(self::$connections[$port][$id])) {
            return null;
        }
        return self::$connections[$port][$id];
    }

    /**
     * 获取所有连接对象
     *
     * @param string $port
     * @return array
     */
    public static function getAll($port = '0')
    {
        return isset(self::$connections[$port]) ? self::$connections[$port] : array();
    }

    /**
     * 随机获取一个
     * @param string $port
     * @return mixed|null
     */
    public static function getRand($port = '0')
    {
        if (empty(self::$connections[$port])) {
            return null;
        }
        $id = array_rand(self::$connections[$port]);
        return self::$connections[$port][$id] ? self::$connections[$port][$id] : null;
    }

    /**
     * 组装连接ID
     *
     * @param $connection
     * @return mixed
     */
    protected static function getId($connection)
    {
        return $connection->id;
    }
}
