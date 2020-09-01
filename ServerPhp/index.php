<?php
/**
 * 服务管控脚本
 */
// Display errors.
ini_set('display_errors', 'on');
// Reporting all.
error_reporting(E_ALL);

// 如果没定义根目录，则自动设置
if(!defined('ROOT_PATH')){
    $backtrace                = debug_backtrace();
    $dir = isset($backtrace[0]['file']) ? dirname($backtrace[0]['file']) : '';
    if (!file_exists($dir)) {
        $dir = str_replace(array('/'.basename(__FILE__), '\\'.basename(__FILE__)), '', __FILE__);
    }
    define('ROOT_PATH', $dir);
}

include_once(ROOT_PATH . '/vendor/autoload.php');

// 启动
\Wechat\App\Server::run();

