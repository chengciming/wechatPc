<h1 align="left">PC端微信Hook源码</h1>

# PC微信hook源码，仅供学习，请不要用于商业、违法途径，本人不对此源码造成的违法负责！

## 技术学习与交流群
1. QQ群：147694968
2. 说明：此群仅限学习和交流，无其他用处

## 目录说明
1. WechatSetup：微信安装包，也可到官网下载对应版本安装包。
2. WechatRobot：Hook端主程序源码目录，C++程序，编译后放到Bin目录下运行。
3. WechatDll：注入库源码，C++程序，编译后放到Bin目录下运行。
4. ServerPhp：服务端源码，PHP程序，wokerman框架v3.5，相关框架资料可以从官网查看。
5. Web：web端源码，HTML5程序。
6. Bin：Hook端和注入库编译后的存放与运行目录。

## 程序说明
1. Hook原理：exe注入DLL到微信进程，实现对微信的消息拦截、方法调用。
2. 程序语言：exe和DLL库是C++编写，服务端是PHP的workman框架编写，web端是HTML5编写。
3. 限制：此程序仅限PC端微信2.8.0.121版本，需要其他版本则需要更新Hook偏移量。
4. 升级：微信2.8.0.121升级到其他版本，可以借助《微信基址-2.8.0.121.txt》里面的特征码进行OD捕获偏移量。

## 程序语言版本
1. C++是基于VS2017开发，SDK版本：10.0.17763.0，工具集：v141，MFC：标准Window库
2. PHP限制版本看workman官方v3.5版本限制，建议PHP7以上

## 环境
1. 客户端：windows
2. 服务端：windows or linux --> for PHP
3. 通讯机制：WebSocket
4. 通讯流程：WechatDll(注入到微信) <--> [WS] <--> WechatRobot(操作界面) <--> [WS] <--> ServerPhp(服务端) <--> [WS] <--> Web(HTML操作界面)

## 配置

### 服务端配置(ServerPhp)
1. 配置文件：wechatPc/ServerPhp/Config/Config.php
2. debug项为调试开关，开启将打印消息到屏幕
3. listen项为Hook端通讯监听地址和端口，Hook端即为WechatRobot
4. web_listen项为web端通讯监听端口
5. worker项则为workman框架的配置，可以看官网解说
### Hook端配置(WechatRobot --> Bin)
1. 配置文件：wechatPc/Bin/Debug/Config.txt
2. app_id、app_key为认证授权使用，未认证的不可通讯
3. wesocket_server_address和wesocket_server_port为服务端通讯的IP和端口，对应服务端配置listen项，服务端即ServerPhp
### web端配置(Web)
1. 配置文件：
2. 文件最后面找到：ws = new WebSocket("ws://127.0.0.1:5678");
3. 将127.0.0.1:5678改为服务端IP和端口，对应服务端配置web_listen项，服务端即ServerPhp

## 启动

### 服务端启动
```shell
[composer@localhost wechatPc]# cd ServerPhp
[composer@localhost ServerPhp]# composer install    // 注：安装程序扩展包、框架，第一次运行前跑一次即可
[composer@localhost ServerPhp]# php index.php start
----------------------- WORKERMAN -----------------------------
Workerman version:3.5.31          PHP version:7.2.25
------------------------ WORKERS -------------------------------
worker               listen                              processes status
Server:Wechat        websocket://0.0.0.0:8686            4         [ok]
[PID:7940 | 2020-09-01 08:57:44.759] Worker Started.
```
### Hook端启动
```shell
1. 环境：window环境下
2. 操作：点击运行wechatPc/Bin/Debug/WechatRobot.exe文件，点击打开微信将自动开启微信
	WechatRobot将自动加载并且注入WechatDll.dll到对应微信
```
### web端启动
```shell
1. nginx或者其他web服务器指定站点到Web目录即可访问，或尝试直接浏览器打开访问index.html
```
## 大佬，打个赏吧
![微信收款二维码](https://github.com/chengciming/wechatPc/blob/master/images/微信收款二维码.jpg)
![支付宝收款二维码](https://github.com/chengciming/wechatPc/blob/master/images/支付宝收款二维码.jpg)
