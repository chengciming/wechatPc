
var wechatList = [];
var friendList = [];
var messageList = [];
// 收到好友列表消息
function recvFriendList(wechatId, fiendsList, page, pageTotal)
{
	if (page == 1) {
		friendList[wechatId] = {wechatId: wechatId, friendList: []};
	}
	for (var i = 0;i < fiendsList.length;i++) {
		friendList[wechatId].friendList.push(fiendsList[i]);
	};
	if (page == pageTotal) {
		showFriendList(wechatId);
	}
}
// 保存消息
function saveMessage(wechatId, message, isMy)
{
	isMy = isMy ? isMy : false;
	var chatId = message.wxid;
	if (!messageList.hasOwnProperty(wechatId)) {
		messageList[wechatId] = [];
	}
	if (!messageList[wechatId].hasOwnProperty(chatId)) {
		messageList[wechatId][chatId] = [];
	}
	// 保存
	messageList[wechatId][chatId].push({isMy: isMy, message: message});
}
// 收到消息 - 显示消息
function recvMessage(wechatId, message)
{
	// 保存信息
	saveMessage(wechatId, message, false);
	// 展示信息
	var chatId = message.wxid;
	var currWxid = getCurrChatWxid();
	if (currWxid == chatId) {
		var info = getFriendInfo(wechatId, chatId);
		var headUrl = info.headUrl;
		var nickname = info.nickname;
		if (message.roomId != '') {
			info = getFriendInfo(wechatId, message.wxid);
			headUrl = info.headUrl;
			nickname = info.nickname;
		}
		showMessage(message.content, false, headUrl, nickname);
	} else {
		addMessageNumber(chatId, 1);  // 增加未读数量
	}
	// 排序到第一
	$('.bcen_block[data-wxid="'+chatId+'"]')
	
	var obj = $('.bcen_block[data-wxid="'+chatId+'"]');
	obj.insertBefore(obj.parent('div').children('.bcen_block:first'));
	
}
// 获取微信好友详情
function getFriendInfo(wechatId, wxid)
{
	var list = friendList.hasOwnProperty(wechatId) ? friendList[wechatId] : null;
	if (!list) {
		return null;
	}
	list = list.friendList;
	for (var i = 0;i < list.length;i++) {
		if (list[i].wxid == wxid) {
			return list[i];
		}
	}
	return null;
}

// 显示好友列表
function showFriendList(wechatId, list)
{
	if (!list) {
		list = friendList.hasOwnProperty(wechatId) ? friendList[wechatId].friendList : [];
	}
	// 当前显示的不是此微信
	if (getCurrWechatId() != wechatId) {
		return false;
	}
	
	var friendHtml = '';
	var addFriendHtml = '';
	var roomHtml = '';
	var publicHtml = '';
	
	for (var i = 0;i < list.length;i++) {
		if ($('.bcen_block[data-wxid="'+list[i].wxid+'"]').length > 0) {
			continue;
		}
		var type = getWxType(list[i].wxid, list[i].bizBrandFlag);
		// 没有v1的说明没加好友，群聊除外，或可能是已经删除好友的
		if (list[i].type <= 0 || list[i].type > 3) {
			continue;
		}
		if (type == 1 && list[i].nickname == '') {
			list[i].nickname = getTmpRoomName(wechatId, list[i].roomWxidList);
		}
		if (type == 1) {
			roomHtml += '<div class="bcen_block" data-wechat-id="'+wechatId+'" data-wxid="'+list[i].wxid+'" data-username="'+list[i].username+'" data-biz-brand-flag="'+list[i].bizBrandFlag+'">';
				roomHtml += '<div class="bcen_block_face fl relative">';
					roomHtml += '<div class="iconnum">0</div>';
					roomHtml += '<img src="'+checkDefaultHeadUrl(list[i].headUrl)+'"/>';
				roomHtml += '</div>';
				roomHtml += '<div class="bcen_block_font fl">';
					roomHtml += '<div class="bcen_block_title" style="line-height:40px;">'+list[i].nickname+'</div>';
					//roomHtml += '<div class="bcen_block_fonts over">昵称：劳拉·克劳馥</div>';
				roomHtml += '</div>';
			roomHtml += '</div>';
		} else if (type == 2) {
			publicHtml += '<div class="bcen_block" data-wechat-id="'+wechatId+'" data-wxid="'+list[i].wxid+'" data-username="'+list[i].username+'">';
				publicHtml += '<div class="bcen_block_face fl relative">';
					publicHtml += '<div class="iconnum">0</div>';
					publicHtml += '<img src="'+checkDefaultHeadUrl(list[i].headUrl)+'"/>';
				publicHtml += '</div>';
				publicHtml += '<div class="bcen_block_font fl">';
					publicHtml += '<div class="bcen_block_title" style="line-height:40px;">'+list[i].nickname+'</div>';
					//publicHtml += '<div class="bcen_block_fonts over">昵称：劳拉·克劳馥</div>';
				publicHtml += '</div>';
			publicHtml += '</div>';
		} else {
			// 添加好友进群列表
			addFriendHtml += '<div class="ba_leftf_block" data-wechat-id="'+wechatId+'" data-wxid="'+list[i].wxid+'" data-username="'+list[i].username+'">';
				addFriendHtml += '<div class="ba_lfb_face fl">';
					addFriendHtml += '<img src="'+checkDefaultHeadUrl(list[i].headUrl)+'"/>';
				addFriendHtml += '</div>';
				//addFriendHtml += '<div class="ba_lfb_font fl">A.呵呵呵</div>';
				
				if (list[i].remark != '') {
					addFriendHtml += '<div class="ba_lfb_font fl">'+list[i].remark+'</div>';
					//addFriendHtml += '<div class="bcen_block_fonts over">昵称：'+list[i].nickname+'</div>';
				} else {
					addFriendHtml += '<div class="ba_lfb_font fl">'+list[i].nickname+'</div>';
				}
				addFriendHtml += '<div class="ba_lfb_circle fr"><img src="common/img/circle010.png"/></div>';
			addFriendHtml += '</div>';
			// 好友列表
			friendHtml += '<div class="bcen_block" data-wechat-id="'+wechatId+'" data-wxid="'+list[i].wxid+'" data-username="'+list[i].username+'">';
				friendHtml += '<div class="bcen_block_face fl relative">';
					friendHtml += '<div class="iconnum">0</div>';
					friendHtml += '<img src="'+checkDefaultHeadUrl(list[i].headUrl)+'"/>';
				friendHtml += '</div>';
				friendHtml += '<div class="bcen_block_font fl">';
					if (list[i].remark != '') {
						friendHtml += '<div class="bcen_block_title">'+list[i].remark+'</div>';
						friendHtml += '<div class="bcen_block_fonts over">昵称：'+list[i].nickname+'</div>';
					} else {
						friendHtml += '<div class="bcen_block_title" style="line-height:40px;">'+list[i].nickname+'</div>';
					}
				friendHtml += '</div>';
			friendHtml += '</div>';
		}
	}
	if (friendHtml != '') {
		friendHtml = '<div class="bsea_first">好友</div><div>' + friendHtml + '</div>';
	}
	if (addFriendHtml != '') {
		$('.ba_leftf_box').html(addFriendHtml);
		$(".ba_leftf_block").unbind('click').click(function (){
			
			var wechatId = $(this).attr('data-wechat-id');
			var wxid = $(this).attr('data-wxid');
			var wechatId = $(this).attr('data-wechat-id');
			
			$(this).addClass("ba_leftf_back");
			$(this).siblings(".ba_leftf_block").removeClass("ba_leftf_back");
						
			selectFriend(wxid);
		});
	}
	if (roomHtml != '') {
		roomHtml = '<div class="bsea_first">群聊</div><div>' + roomHtml + '</div>';
	}
	if (publicHtml != '') {
		publicHtml = '<div class="bsea_first">公众号</div><div>' + publicHtml + '</div>';
	}
	
	// 加载好友页面
	$('.friend_list').html(roomHtml + friendHtml + publicHtml);
	
	// 绑定点击事件 - 切换聊天窗口
	$('.bcen_block').click(function(){
		var wxid = $(this).attr('data-wxid');
		var bizBrandFlag = $(this).attr('data-biz-brand-flag');
		switchChat($(this).attr('data-wechat-id'), wxid, bizBrandFlag);
		// 未读计数清零
		subMessageNumber(wxid, 0);
		$('.bcen_block').removeClass('bcen_menu_block_hover');
		$(this).addClass('bcen_menu_block_hover');
	});
	
	// 自动默认选择第一个人
	$('.bcen_block:first').click();
	
	$('.bsea_first').click(function(){
		$(this).toggleClass('bsea_first_up');
		$(this).next().toggle('show');
	});
}
// 选择好友的效果
function selectFriend(wxid)
{
	var number = $('.buju_add').attr('data-number');
	number = number ? number - 0 : 0;
	
	var selectObj = $('.ba_leftf_block[data-wxid="'+wxid+'"]');
	var selectListObj = $('.ba_rif_box div[data-wxid="'+wxid+'"]');
	
	if(selectListObj.length <= 0) {
		// 判断选择数量的限制
		var limit = $(".buju_add").attr('data-limit');
		limit = limit ? limit - 0 : 0;
		if (limit > 0 && number >= limit) {
			return false;
		}
		selectObj.find(".ba_lfb_circle").find("img").attr("src","common/img/circle011.png");		
		number++;
		$(".ba_right_top").html('已选择了'+number+'个联系人');
		// 添加选择列表
		var headUrl = selectObj.find(".ba_lfb_face").find("img").attr("src");
		var nickname = selectObj.find(".ba_lfb_font").html();
		var selectFriendHtml = '<div class="ba_leftf_block" data-wxid="'+wxid+'"><div class="ba_lfb_face fl"><img src="'+headUrl+'"/></div><div class="ba_lfb_font fl">'+nickname+'</div><div class="ba_rfb_close fr" onclick="selectFriend(\''+wxid+'\');"><img src="common/img/close.png"/></div></div>';
		$(".ba_rif_box").children("div").append(selectFriendHtml);
	} else {
		selectListObj.remove();
		selectObj.find(".ba_lfb_circle").find("img").attr("src","common/img/circle010.png");
		number--;
		if(number > 0){
			$(".ba_right_top").html("已选择了" + number + "个联系人"); 	
		} else {
			$(".ba_right_top").html("请勾选需要添加的联系人");
		}
	}
	
	$('.buju_add').attr('data-number', number);
	
	$(".ba_ri_but").removeClass("ba_ri_buts");
	if(number > 0) {
		$(".ba_ri_but").addClass("ba_ri_buts");
	}
}
// 增加好友列表未读消息的数量
function addMessageNumber(wxid, number)
{
	var obj = $('div[data-wxid="'+wxid+'"] .iconnum');
	var oldNumber = obj.html();
	oldNumber = parseInt(oldNumber) + number;
	obj.html(oldNumber);
	obj.show();
}
// 减少好友列表未读消息的数量
function subMessageNumber(wxid, number)
{
	var obj = $('div[data-wxid="'+wxid+'"] .iconnum');
	if (number == 0) {  // 清零
		var oldNumber = number;
	} else {
		var oldNumber = obj.html();
		oldNumber -= number;
		oldNumber = oldNumber < 0 ? 0 : oldNumber;
	}
	obj.html(oldNumber);
	if (oldNumber <= 0) {
		obj.hide();
	}
}
// 获取当前聊天窗口微信ID
function getCurrChatWxid()
{
	var wxid = $('.bri_top').attr('data-wxid');
	return wxid ? wxid : null;
}
// 切换当前聊天窗口
function switchChat(wechatId, chatId, bizBrandFlag)
{
	bizBrandFlag = bizBrandFlag ? bizBrandFlag : 0;
	var wechat = wechatList.hasOwnProperty(wechatId) ? wechatList[wechatId] : null;
	var info = getFriendInfo(wechatId, chatId);
	// 切换聊天窗口
	$('.bri_top').attr('data-wechat-id', wechatId);
	$('.bri_top').attr('data-wxid', chatId);
	$('.bri_top').attr('data-biz-brand-flag', bizBrandFlag);
	$('.bri_top .bri_top_font').html(info.nickname);
	$('.brce_block_box').html('');
	// 显示聊天记录
	var chatMessage = messageList.hasOwnProperty(wechatId) ? messageList[wechatId] : null;
	if (chatMessage) {
		chatMessage = chatMessage.hasOwnProperty(chatId) ? chatMessage[chatId] : null;
		if (chatMessage) {
			for (var msgId in chatMessage) {
				var info = getFriendInfo(wechatId, chatId);
				var headUrl = info.headUrl;
				var nickname = info.nickname;
				if (chatMessage[msgId].message.roomId != '') {
					info = getFriendInfo(wechatId, chatMessage[msgId].message.wxid);
					headUrl = info.headUrl;
					nickname = info.nickname;
				}
				
				showMessage(chatMessage[msgId].message.content, chatMessage[msgId].isMy, chatMessage[msgId].isMy ? wechat.headUrl : headUrl, chatMessage[msgId].isMy ? '' : nickname);
			}
		}
	}
	// 更多详情操作事件绑定
	$(".bri_top_more").unbind('click').click(function (){
		var wechatId = $('.bri_top').attr('data-wechat-id');
		var wxid = $('.bri_top').attr('data-wxid');
		var info = getFriendInfo(wechatId, wxid);
		var type = getWxType(wxid);
		
		$('.friend_chat,.room_chat').hide();
		if (type == 1) {  // 群聊
			$('.bin_five_text').html(info.nickname);
			$('.bin_five_name').val(info.nickname);
			$('.room_chat').show();
		} else {  // 公众号 || 私聊
			$('.bin_one_face img').attr('src', checkDefaultHeadUrl(info.headUrl));
			$('.bin_one_name').html(info.remark != '' ? info.remark : info.nickname);
			$('.friend_chat').show();
			$('.friend_chat').find('.friend_chat_add').attr('data-wxid', wxid).unbind('click').click(function(){
				var wxid = $(this).attr('data-wxid');
				addfriend(2, [wxid])
			});
			
			// 头像点击事件
			$('.bin_one_face').unbind('click').click(function(){
				var wechatId = $('.bri_top').attr('data-wechat-id');
				var wxid = $('.bri_top').attr('data-wxid');
				var info = getFriendInfo(wechatId, wxid);
				showInfo('.bin_one_face', info.username != '' ? info.username : info.wxid, info.nickname, info.sex ? info.sex : null, info.headUrl, info.province, info.city, info.remark);
			});
		}
		// 显示或者隐藏
		$(".buju_info").toggle('show');
		// 关闭事件
		$(document).unbind("click").bind("click",function(e){
			if($(e.target).closest(".buju_info").length == 0 && $(e.target).closest(".bri_top_more").length == 0){
				$(".buju_info").hide();
			}
		});
	});
}

// 显示二维码
function showQrcode(text)
{
	$('.login_qrcode_content_img').each(function(){
		$(this).html('');
		new QRCode(this, {
			text: text,
			width: 100,
			height: 100,
			colorDark : "#000000",
			colorLight : "#ffffff",
		});
	});
	$('.login_qrcode_body,.login_qrcode_content').show();
}
// 隐藏二维码
function hideQrcode()
{
	$('.login_qrcode_content_img').html('');
	$('.login_qrcode_body,.login_qrcode_content').hide();
}
// 切换当前展示的微信
function switchWechat(wechatId, chatId)
{
	// 显示当前用户信息
	var wechat = wechatList.hasOwnProperty(wechatId) ? wechatList[wechatId] : null;
	if (wechat != null) {
		$('.login_face_img').attr('src', checkDefaultHeadUrl(wechat.info.headUrl));
		$('body').attr('data-wechat-id', wechatId);
		// 显示好友列表
		showFriendList(wechatId);
	}
}
// 登录成功操作
function loginSuccess(wechatId, bodyJson)
{
	// 临时储存账号详情
	wechatList[wechatId] = {wechatId: wechatId, info: bodyJson};
	// 添加到登陆列表
	addLoginList(wechatId, bodyJson.wxid, bodyJson.nickname, bodyJson.headUrl);
	// 如果当前没有已经展示的微信账号，自动切换到新账号
	if (!getCurrWechatId()) {
		switchWechat(wechatId);
	}
}
// 添加新登陆账号到登陆列表
function addLoginList(wechatId, wxid, nickname, headUrl)
{
	if ($('.account_one[data-wechat-id="'+wechatId+'"]').length > 0) {
		$('.account_one[data-wechat-id="'+wechatId+'"]').remove();
	}
	var html = '<div class="account_one"'+(' data-wechat-id='+wechatId)+' data-wxid="'+wxid+'">';
		html += '<div class="account_one_face fl">';
			html += '<img src="'+headUrl+'" width="40" height="40" />';
		html += '</div>';
		html += '<div class="account_one_font fl" title="'+nickname+'">'+nickname+'</div>';
	html += '</div>';
	$('.account_list_content').append(html);
	$('.account_list_content .account_one[data-wechat-id="'+wechatId+'"]').click(function(){
		switchWechat($(this).attr('data-wechat-id'), $(this).attr('data-wxid'));
	});
}
// 获取当前已经展示的微信ID
function getCurrWechatId()
{
	var wechatId = $('body').attr('data-wechat-id');
	return wechatId&&wechatId!=undefined ? wechatId : false;
}
// 没头衔时候设置默认头像
function checkDefaultHeadUrl(headUrl)
{
	if (!headUrl || headUrl == '') {
		headUrl = 'common/img/user-192.png';
	}
	return headUrl;
}
// 识别微信好友类型
function getWxType(wxid, bizBrandFlag)
{
	bizBrandFlag = bizBrandFlag ? bizBrandFlag : 0;
	var type = 0;  // 好友
	if (wxid.indexOf('@chatroom') >= 0) {
		type = 1;  // 群
	} else if (wxid.indexOf('gh_') >= 0 || bizBrandFlag > 0) {
		type = 2;  // 公众号
	}
	return type;
}
// 切割群成员微信ID
function splitRoomMemberWxid(roomWxidList)
{
	if (!roomWxidList || roomWxidList == '') {
		return [];
	}
	roomWxidList = roomWxidList.substr(2, roomWxidList.length - 4);
	if (roomWxidList.indexOf('^G') < 0) {
		return [roomWxidList];
	}
	var roomList = roomWxidList.split('^G');
	return roomList;
}
// 获取临时群名称
function getTmpRoomName(wechatId, roomWxidList)
{
	var nickname = [];
	var roomMmberWxidList = splitRoomMemberWxid(roomWxidList);
	if (roomMmberWxidList.length > 0) {
		var info1 = getFriendInfo(wechatId, roomMmberWxidList[0]);
		if (info1) {
			nickname.push(info1.nickname);
		}
	}
	if (roomMmberWxidList.length > 1) {
		var info2 = getFriendInfo(wechatId, roomMmberWxidList[1]);
		if (info2) {
			nickname.push(info2.nickname);
		}
	}
	if (roomMmberWxidList.length > 2) {
		var info3 = getFriendInfo(wechatId, roomMmberWxidList[2]);
		if (info3) {
			nickname.push(info3.nickname);
		}
	}
	return nickname.join('、');
}