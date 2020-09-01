// JavaScript Document

/** @var int 响应指令 */
var OPCODE_READY = 0x0;  // ws客户端准备完毕
var OPCODE_SUCCESS = 0x1;  // 操作成功
var OPCODE_FAILURE = 0x2;  // 操作失败
/** @var int 业务指令 */
var OPCODE_WECHAT_OPEN = 0x10;  // 新开一个微信客户端
var OPCODE_WECHAT_QUIT = 0x11;  // 退出一个微信
var OPCODE_WECHAT_GET_LOGIN_STATUS = 0x12;  // 获取登录状态
var OPCODE_MESSAGE_SEND_TEXT = 0x20;  // 发送文本消息
var OPCODE_MESSAGE_SEND_IMAGE = 0x21;  // 发送图片消息
var OPCODE_MESSAGE_SEND_FILE = 0x22;  // 发送附件消息
var OPCODE_MESSAGE_SEND_CARD = 0x23;  // 发送名片消息
var OPCODE_MESSAGE_SEND_XML = 0x24;  // 发送xml消息
var OPCODE_FRIEND_ADD = 0x30;  // wxid加好友
var OPCODE_FRIEND_ADD_FROM_V1 = 0x31;  // v1加好友
var OPCODE_FRIEND_DELETE = 0x32;  // 删除好友
var OPCODE_FRIEND_VERIFY = 0x33;  // v1+v2同意好友请求
var OPCODE_FRIEND_LIST = 0x34;  // 好友列表
var OPCODE_FRIEND_REMARK = 0x35;  // 修改备注
var OPCODE_ROOM_CREATE = 0x40;  // 创建群聊
var OPCODE_ROOM_EDIT_NAME = 0x41;  // 修改群名称
var OPCODE_ROOM_ANNOUNCEMENT = 0x42;  // 发送群公告
var OPCODE_ROOM_MEMBER_LIST = 0x43;  // 获取群成员列表
var OPCODE_ROOM_ADD_MEMBER = 0x44;  // 拉好友入群
var OPCODE_ROOM_DELETE_MEMBER = 0x45;  // 删除群成员
var OPCODE_ROOM_AT_MEMBER = 0x46;  // 艾特群成员
var OPCODE_ROOM_QUIT = 0x47;  // 退出群聊
var OPCODE_TRANSFER_RECV = 0x50;  // 收款
var OPCODE_WECHAT_QRCODE = 0x90;  // 返回登录二维码地址
var OPCODE_LOGIN_INFO = 0x91;  // 返回当前登录的微信号信息
var OPCODE_MESSAGE_RECEIVE = 0x92;  // 返回接收的微信消息

// 数据包
class Package {
    constructor() {
        this.wechatId = null;
		this.opCode = null;
		this.body = null;
    }
    setWechatId (wechatId){
		this.wechatId = wechatId;
	}
	getWechatId (){
		return this.wechatId;
	}
    setOpCode (opCode){
		this.opCode = opCode;
	}
	getOpCode (){
		return this.opCode;
	}
	setBody (body){
		this.body = body ? body : new Object();
	}
	getBody (){
		return this.body ? this.body : new Object();
	}
	getJson (){
		var json = new Object();
		json['wechatId'] = this.wechatId;
		json['opCode'] = this.opCode;
		json['body'] = this.body;
		return JSON.stringify(json);
	}
	setJson (data){
		var json = eval('('+data+')');
		this.setWechatId(json.wechatId);
		this.setOpCode(json.opCode);
		this.setBody(json.body);
	}
	free (){
		this.wechatId = null;
		this.opCode = null;
		this.body = null;
		delete this.wechatId, this.opCode, this.body;
		delete this;
	}
}
