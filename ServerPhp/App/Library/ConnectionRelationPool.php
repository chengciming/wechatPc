<?php
namespace Wechat\App\Library;

/**
 * 连接关系池
 * Class ConnectionRelationPool
 * @package Mapping\Application\Library
 */
class ConnectionRelationPool
{
    /** @var array 一对多绑定关系，关系组储存 */
    protected static $relationPoolGroup = array();
    /** @var array 一对一绑定关系，关系池 */
    protected static $relationPool = array();

    /**
     * 随机获取一个目标ID
     * @param string $groupId 组ID
     * @return mixed|null
     */
    public static function getRandId($groupId)
    {
        if (!isset(self::$relationPoolGroup[$groupId]) || empty(self::$relationPoolGroup[$groupId])) {
            return null;
        }
        return array_rand(self::$relationPoolGroup[$groupId]);
    }

    /**
     * 添加到组
     * @param string $id 目标ID
     * @param string $groupId 绑定到组ID
     * @return bool
     */
    public static function add($id, $groupId)
    {
        if (!isset(self::$relationPoolGroup[$groupId])) {
            self::$relationPoolGroup[$groupId] = array();
        }
        self::$relationPoolGroup[$groupId][$id] = $id;  // 一对多
        self::$relationPool[$id] = $groupId;  // 一对一
        return true;
    }

    /**
     * 从组里面删除
     * @param string $id 目标ID
     * @param string $groupId 组ID
     * @return bool
     */
    public static function remove($id, $groupId = null)
    {
        if (!isset(self::$relationPool[$id])) {
            return false;  // 未绑定过
        }
        $groupId = $groupId ? $groupId : self::$relationPool[$id];
        if (isset(self::$relationPoolGroup[$groupId][$id])) {
            unset(self::$relationPoolGroup[$groupId][$id]);  // 解绑一对多，从组里面删除
            if (empty(self::$relationPoolGroup[$groupId])) {
                unset(self::$relationPoolGroup[$groupId]);
            }
        }
        if (isset(self::$relationPool[$id])) {
            unset(self::$relationPool[$id]);  // 解绑一对一
        }
        return true;
    }

    /**
     * 获取目标ID对应绑定的组ID
     * @param string $id 目标ID
     * @return string|null
     */
    public static function getGroupId($id)
    {
        if (isset(self::$relationPool[$id])) {
            return self::$relationPool[$id];
        }
        return null;
    }

    /**
     * 随机获取一个分组ID
     * @return mixed|null
     */
    public static function getRandGroupId()
    {
        if (empty(self::$relationPool)) {
            return null;
        }
        return array_rand(self::$relationPool);
    }

    /**
     * 获取整个组下面的ID
     * @param $groupId
     * @return array|null
     */
    public static function getGroup($groupId)
    {
        if (!isset(self::$relationPoolGroup[$groupId])) {
            return null;
        }
        return self::$relationPoolGroup[$groupId];
    }

    /**
     * 删除整个组关系
     * @param string $groupId 组ID
     * @return bool
     */
    public static function removeGroup($groupId)
    {
        if (!isset(self::$relationPoolGroup[$groupId])) {
            return false;  // 组不存在
        }
        // 组存在，解除组下面所有的ID
        if (!empty(self::$relationPoolGroup[$groupId])) {
            foreach (self::$relationPoolGroup[$groupId] as $id) {
                // 删除绑定关系
                if (isset(self::$relationPool[$id])) {
                    unset(self::$relationPool[$id]);
                }
                // 删除组下面对应的ID
                unset(self::$relationPoolGroup[$groupId][$id]);
            }
        }
        if (empty(self::$relationPoolGroup[$groupId])) {
            unset(self::$relationPoolGroup[$groupId]);
        }
        return true;
    }
}
