// JavaScript Document

// websocket句柄
var ws = null;

// 获取登录信息
function getLoginInfo(wechatId)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_LOGIN_INFO);
	json.setBody({});
	send(json.getJson());
}
// 获取登录状态
function getLoginStatus(wechatId)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_WECHAT_GET_LOGIN_STATUS);
	json.setBody({});
	send(json.getJson());
}

// 退出登录
function logout(wechatId)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_WECHAT_QUIT);
	json.setBody({});
	send(json.getJson());
}

// 发送文本消息
function sendText(wechatId, wxid, content)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_MESSAGE_SEND_TEXT);
	json.setBody({
		wxid: wxid,
		content: content
	});
	send(json.getJson());
}
// 发送图片消息
function messageSendImage(wechatId, wxid, imageUrl)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_MESSAGE_SEND_IMAGE);
	json.setBody({
		wxid: wxid,
		imageUrl: imageUrl
	});
	send(json.getJson());
}
// 发送附件消息
function messageSendFile(wechatId, wxid, fileUrl)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_MESSAGE_SEND_FILE);
	json.setBody({
		wxid: wxid,
		fileUrl: fileUrl
	});
	send(json.getJson());
}
// 发送名片消息
function messageSendCard(wechatId, wxid, xml)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_MESSAGE_SEND_CARD);
	json.setBody({
		wxid: wxid,
		xml: xml
	});
	send(json.getJson());
}
// 发送xml消息
function messageSendXml(wechatId, type, wxid, fromWxid, imageUrl, xml)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_MESSAGE_SEND_XML);
	json.setBody({
		type: type,
		wxid: wxid,
		fromWxid: fromWxid,
		imageUrl: imageUrl,
		xml: xml
	});
	send(json.getJson());
}
// wxid加好友
function friendAdd(wechatId, wxid, message)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_FRIEND_ADD);
	json.setBody({
		wxid: wxid,
		message: message
	});
	send(json.getJson());
}
// v1加好友
function friendAddFromV1(wechatId, v1, message)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_FRIEND_ADD_FROM_V1);
	json.setBody({
		v1: v1,
		message: message
	});
	send(json.getJson());
}
// 删除好友
function friendDelete(wechatId, wxid)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_FRIEND_DELETE);
	json.setBody({
		wxid: wxid
	});
	send(json.getJson());
}
// v1+v2同意好友请求
function friendVerify(wechatId, v1, v2)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_FRIEND_VERIFY);
	json.setBody({
		v1: v1,
		v2: v2
	});
	send(json.getJson());
}
// 好友列表
function friendList(wechatId)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_FRIEND_LIST);
	json.setBody({});
	send(json.getJson());
}
// 创建群聊
function roomCreate(wechatId, wxid1, wxid2)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_ROOM_CREATE);
	json.setBody({
		wxid1: wxid1,
		wxid2: wxid2
	});
	send(json.getJson());
}
// 修改好友备注
function setFriendRemark(wechatId, wxid, remark)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_FRIEND_REMARK);
	json.setBody({
		wxid: wxid,
		remark: remark
	});
	send(json.getJson());
}
// 修改群名称
function editRoomName(wechatId, roomId, roomName)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_ROOM_EDIT_NAME);
	json.setBody({
		roomId: roomId,
		roomName: roomName
	});
	send(json.getJson());
}
// 发送群公告
function setRoomAnnouncement(wechatId, roomId, announcement)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_ROOM_ANNOUNCEMENT);
	json.setBody({
		roomId: roomId,
		announcement: announcement
	});
	send(json.getJson());
}
// 获取群成员列表
function roomMemberList(wechatId, roomId)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_ROOM_MEMBER_LIST);
	json.setBody({
		roomId: roomId
	});
	send(json.getJson());
}
// 拉好友入群
function roomAddMember(wechatId, roomId, wxid)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_ROOM_ADD_MEMBER);
	json.setBody({
		roomId: roomId,
		wxid: wxid
	});
	send(json.getJson());
}
// 删除群成员
function roomDeleteMember(wechatId, roomId, wxid)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_ROOM_DELETE_MEMBER);
	json.setBody({
		roomId: roomId,
		wxid: wxid
	});
	send(json.getJson());
}
// 艾特群成员
function roomAtMember(wechatId, roomId, wxid, nickname, message)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_ROOM_AT_MEMBER);
	json.setBody({
		roomId: roomId,
		wxid: wxid,
		nickname: nickname,
		message: message
	});
	send(json.getJson());
}
// 退出群聊
function roomQuit(wechatId, roomId)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_ROOM_QUIT);
	json.setBody({
		roomId: roomId
	});
	send(json.getJson());
}
// 收款
function transferRecv(wechatId, wxid, transferId)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_TRANSFER_RECV);
	json.setBody({
		roomId: roomId,
		transferId: transferId
	});
	send(json.getJson());
}
// 关闭一个微信
function closeWechat(wechatId)
{
	var json = new Package();
	json.setWechatId(wechatId);
	json.setOpCode(OPCODE_WECHAT_QUIT);
	json.setBody({});
	send(json.getJson());
}
// 新开一个微信
function openWechat()
{
	var json = new Package();
	json.setWechatId('5B4729E0FCB1A8B75973D68C31FF7DBC');
	json.setOpCode(OPCODE_WECHAT_OPEN);
	json.setBody({});
	send(json.getJson());
}
// 发送
function send(msg)
{
	if (ws.status == true) {
		ws.send(msg);
	}
}
// 接收消息
function recv(data)
{
	var json = new Package();
	json.setJson(data);
	var wechatId = json.getWechatId();
	var opCode = json.getOpCode();
	var body = json.getBody();
	switch (opCode) {
		// 收到微信消息
		case OPCODE_MESSAGE_RECEIVE:
			recvMessage(wechatId, body);
		break;
		// 好友列表
		case OPCODE_FRIEND_LIST:
			var pageTotal = Math.ceil(body.total / body.pageSize);
			recvFriendList(wechatId, body.friendList, body.page, pageTotal)
		break;
		// 登录状态
		case OPCODE_WECHAT_GET_LOGIN_STATUS:
			// 登录成功
			if (body.loginStatus != 0) {
				hideQrcode();  // 关闭二维码显示
				getLoginInfo(wechatId);  // 获取登录详情
				setTimeout(function(){
					friendList(wechatId);  // 获取好友列表
				}, 1000);
			}
		break;
		// 登录二维码
		case OPCODE_WECHAT_QRCODE:
			if (body.loginQrcode) {
				// 显示二维码
				showQrcode(body.loginQrcode);
			}
		break;
		// 已登录的账号信息
		case OPCODE_LOGIN_INFO:
			// 显示登录账号信息
			loginSuccess(wechatId, body);
		break;
	}
	//{"wechatId":"E15B66F22D6D0178791EB1864BA49375","opCode":18,"body":{"login_status":1}}
//{"wechatId":"E15B66F22D6D0178791EB1864BA49375","opCode":18,"body":{"wxid":"wxid_vju0phxgdhgp22","username":"supper-busy","nickname":"\u6211ddd\u662f\u7279\u6b8a\u7684\u4f60d\u59b9\u59b9\u7684\u5431mdjdjfff","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/rz5dWiaYy1q6pZR3FlSmaI27IYY7FpqPcsne6eOw35f0l1XpTF1s8qZJOkDJAxdlWh9z7qqdSrm7Ur6mtCQYEyzsu20IA6lPib5y4Z6qyOX54\/0","mobile":"13711458538","email":"chengciming@vip.qq.com","sex":1,"nation":"CN","province":"Guangdong","city":"Guangzhou","sign":"\u600e\u4e48\u6837\u7684\u4eba\u5c31\u6709\u600e\u4e48\u6837\u7684\u6545\u4e8b\u3002\u3002\u3002","device":"android"}}
	
}

function connect()
{
	// 打开一个 web socket
	ws = new WebSocket("ws://127.0.0.1:5678");
	ws.status = false;
	ws.onopen = function() {
		ws.status = true;
		$('.network_notice').hide();
	};
	ws.onmessage = function (evt) {
		recv(evt.data);
	};
	ws.onclose = function() {
		ws.status = false;
		$('.network_notice').show();
		setTimeout(connect, 3000);
	};
}
$(function (){
	if ("WebSocket" in window) {
	   //alert("您的浏览器支持 WebSocket!");
	   connect();
	} else {
	   // 浏览器不支持 WebSocket
	   alert("您的浏览器不支持 WebSocket，请使用谷歌浏览器!");
	}
})