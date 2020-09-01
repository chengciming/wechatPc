function addfriend(limit, selectWxid)
{
	limit = limit ? limit : 0;
	selectWxid = selectWxid ? selectWxid : [];
	$('.buju_add').attr('data-number', '0');
	$(".buju_add").show();
	$(".buju_add").attr('data-limit', limit);
	// 默认选中
	for (var i = 0;i < selectWxid.length;i++) {
		selectFriend(selectWxid[i]);
	}
}
function addclose()
{
	$(".buju_add").hide();	
	$(".ba_ri_but").removeClass("ba_ri_buts");
	$(".ba_leftf_box").find(".ba_leftf_block").removeClass("ba_leftf_back");
	$(".ba_leftf_box").find(".ba_lfb_circle").find("img").attr("src","common/img/circle010.png");
	$(".ba_rif_box").children("div").html("");
	$(".ba_right_top").html("请勾选需要添加的联系人");
}
function showMessage(content, isMy, headUrl, nickname)
{
	nickname = nickname ? nickname : '';
	headUrl = headUrl ? headUrl : '';
	isMy = isMy ? isMy : false;
	content = content.replace(/\r/g, '');
	content = content.replace(/\n/g, '<br />');
	content = content.replace(/\t/g, '&nbsp;&nbsp;&nbsp;&nbsp;');
	var chatHtml = '';
	chatHtml += '<div class="'+(isMy ? 'brce_right' : 'brce_left')+'">';
		chatHtml += '<div class="brce_left_face '+(isMy ? 'fr' : 'fl')+'">';
			chatHtml += '<img src="'+checkDefaultHeadUrl(headUrl)+'"/>';
		chatHtml += '</div>';
		chatHtml += '<div class="brce_left_talk '+(isMy ? 'fr' : 'fl')+'">';
			if (nickname != '') {
				chatHtml += '<div class="brce_lt_name">'+nickname+'</div>';
			}
			chatHtml += '<div class="brce_'+(isMy ? 'rt' : 'lt')+'_jiao '+(isMy ? 'fr' : 'fl')+'"></div>';
			chatHtml += '<div class="brce_'+(isMy ? 'rt' : 'lt')+'_block '+(isMy ? 'fr' : 'fl')+'">';
				chatHtml += '<div>'+content+'</div>';
			chatHtml += '</div>';
		chatHtml += '</div>';
		chatHtml += '<div class="clear"></div>';
	chatHtml += '</div>';
	$(".brce_block_box").append(chatHtml);
	// 滚动到底部
	$('.brce_block_box').animate({scrollTop:$('.brce_block_box')[0].scrollHeight},'slow');
}
// 显示详情弹窗
function showInfo(notCloseClassName, username, nickname, sex, headUrl, province, city, remark)
{
	if (username || nickname) {
		sex = sex != undefined ? sex : null;
		headUrl = headUrl != undefined ? headUrl : null;
		province = province != undefined ? province : null;
		city = city != undefined ? city : null;
		
		$('.wx_info_top_left_nickname_text').html(nickname);
		$('.wx_info_top_left_nickname_text').attr('title', nickname);
		if (sex == null || sex == false) {
			$('.wx_info_top_left_sex_img').hide();
		} else {
			$('.wx_info_top_left_sex_img').attr('src', (sex==1 ? 'common/img/man.jpg' : 'common/img/woman.jpg')).show();
		}
		$('.wx_info_top_left_wxid_text').html(username);
		$('.wx_info_top_left_wxid_text').attr('title', username);
		$('.wx_info_top_right_head_url').attr('src', checkDefaultHeadUrl(headUrl));
		if ((province == null && city == null) || (province == '' && city == '')) {
			$('.wx_info_top_left_area').hide();
		} else {
			$('.wx_info_top_left_area_text').html(province + '&nbsp;' + city);
			$('.wx_info_top_left_area').show();
		}
		if (remark == undefined) {
			$('.wx_info_top_left_remark').hide();
		} else if (remark != '') {
			$('.wx_info_top_left_remark_text').html(remark);
			$('.wx_info_top_left_remark').show();
			$('.wx_info_top_left_input').val(remark);
		} else {
			$('.wx_info_top_left_remark_text').html('[点击设置备注]');
			$('.wx_info_top_left_remark').show();
			$('.wx_info_top_left_input').val('');
		}
		// 修改备注
		$(".wx_info_top_left_remark").hover(function (){
			$('.wx_info_top_left_img').show();
		}, function (){
			$('.wx_info_top_left_img').hide();
		}).click(function (){
			$(this).next("input").attr("type","text");
			$('.wx_info_top_left_remark_text').css("display","none");
			$(this).next("input").focus();
		}).next("input").blur(function (){
			var remark = $(this).val();
			if(remark != ""){
				$(".wx_info_top_left_remark_text").html(remark);
				// 发送
				var wechatId = $('.bri_top').attr('data-wechat-id');
				var wxid = $('.bri_top').attr('data-wxid');
				setFriendRemark(wechatId, wxid, remark);
			}
			$(".wx_info_top_left_remark_text").show();
			$(this).attr("type","hidden");
		});
	}
	
	var y = event.pageY;
	var x = event.pageX;
	var winHeight = $(window).height();
	var winWidth = $(window).width();
	if (x > winWidth - $('.wx_info_content').innerWidth()) {
		x = x - $('.wx_info_content').innerWidth();
	}
	if (y > winHeight - $('.wx_info_content').innerHeight()) {
		y = y - $('.wx_info_content').innerHeight();
	}
	
	$('.wx_info_content')
	.css('top', y + 'px')
	.css('left', x + 'px')
	.show();
	
	// 点击关闭事件
	$(document).unbind("click").bind("click",function(e){
		if($(e.target).closest(notCloseClassName).length == 0 && $(e.target).closest(".wx_info_content").length == 0){
			$(".wx_info_content").hide();
		}
	});
}

function res()
{
	var winds = $(window).height();
	var windw = $(window).width();
	winds = winds * 0.9;
	windw = windw * 0.9;
	
	$('.buju').css('margin-top', (winds * 0.05) + 'px');
	
	$(".buju_left,.buju_center,.buju_right").height(winds);
	$(".buju_info,.bcen_box").height(winds - 61);
	$(".buju_right,.bri_bottom,.brce_block_box").width(windw - 310);
	$(".buju_right textarea").width(windw - 360);
	$(".bri_center,.brce_block_box").height(winds - 205);
	
	$(".buju_info").css('top', (winds * 0.05 + 61)+'px');
	$(".buju_info").css('right', (windw * 0.055)+'px');
	
	$("#more .buju_right").width(windw - 61);
}

$(function (){
	res();
	$(window).resize(function(){
		res();
	});
	// 登录头像点击事件
	$('.login_face').click(function(e){
		var wechatId = $('body').attr('data-wechat-id');
		var wechat = wechatList.hasOwnProperty(wechatId) ? wechatList[wechatId] : null;
		if (wechat != null) {
			// 显示当前登录的用户信息
			showInfo('.login_face', wechat.info.username, wechat.info.nickname, wechat.info.sex, wechat.info.headUrl, wechat.info.province, wechat.info.city);
		}
	});
	
	$('.bleft_iconone').click(function(){
		$('.bleft_iconone').each(function(){
			$('.content').hide();
			$(this).children('img').attr('src', $(this).attr('data-not-active'));
		});
		$(this).children('img').attr('src', $(this).attr('data-active'));
		var divId = $(this).attr('data-div-id');
		$('#' + divId).show();
	});
	
	$('.account_ico').click(function(){
		$('.account_list').toggle('show');
	});
	
	// 聊天窗口开始
	$(".bcen_block").click(function (){
		$(this).addClass("bcen_background");
		$(".bcen_block").not(this).removeClass("bcen_background");
	});
	// 聊天输入框
	$(".bri_bottom textarea").focus(function (){
		$(".bri_bottom").css("background-color","#fff");
	}).blur(function (){
		$(".bri_bottom").css("background-color","#f5f5f5");
	});

	$(".bri_bottom_face").click(function (){
		$(".bri_bottom_look").show();
		// 关闭事件
		$(document).unbind("click").bind("click",function(e){
			if($(e.target).closest(".bri_bottom_look").length == 0 && $(e.target).closest(".bri_bottom_face").length == 0){
				$(".bri_bottom_look").hide();
			}
		});
	})

	$(".bri_bottom_file").click(function (){
		$(".bri_bottom_files").click();
	})
	
	// 退出群聊
	$('.bin_six').click(function(){
		var wechatId = $('.bri_top').attr('data-wechat-id');
		var roomId = $('.bri_top').attr('data-wxid');
		showMessage('[您已退出群聊]', true);
		// 发送
		roomQuit(wechatId, roomId);
	});

	// 发送消息
	$(".bri_bottom_but").click(function (){
		var content = $(".bri_bottom textarea").val();
		if(content != ""){
			var wechatId = $('.bri_top').attr('data-wechat-id');
			var wxid = $('.bri_top').attr('data-wxid');
			saveMessage(wechatId, {wxid: wxid, roomId: '', content: content}, true);
			showMessage(content, true);
			$(".bri_bottom textarea").val("");
			// 发送
			sendText(wechatId, wxid, content);
		}
	})

	$(".bin_two_but").click(function (){
		var imgs = $(this).find("img").attr("src");
		imgs = imgs.replace("common/img/","");
		imgs = imgs.replace(".png","");
		if(imgs == "butclose"){
			$(this).find("img").attr("src","common/img/butopen.png");
		}else if(imgs == "butopen"){
			$(this).find("img").attr("src","common/img/butclose.png");
		}
	})

	$(".bin_three input").focus(function (){
		$(".bin_three").css("background-color","#fff");
	})

	$(".bin_three input").keyup(function (){
		var val = $(this).val();
		if(val == ""){
			$(".bin_three_close").hide();
			$(".bin_three_seach").show();
		}else if(val != ""){
			$(".bin_three_close").show();
			$(".bin_three_seach").hide();
		}
	})

	$(".bin_three_close").click(function (){
		$(".bin_three input").val("");
		$(".bin_three_close").hide();
		$(".bin_three_seach").show();
		$(".bin_three").css("background-color","#f5f5f5");
	})
	// 群更多成员按钮
	$(".bin_four_more").click(function (){
		var hts = $(this).find("label").html();
		if(hts == "查看更多群成员"){
			$(this).find("label").html("收起群成员");
			$(this).find("img").attr("src","common/img/up001.png");
		}else if(hts == "收起群成员"){
			$(this).find("label").html("查看更多群成员");
			$(this).find("img").attr("src","common/img/down001.png");
		}
	})
	
	// 修改群名称
	$(".bin_five_text").hover(function (){
		$('.bin_five_text_img').show();
	}, function (){
		$('.bin_five_text_img').hide();
	}).click(function (){
		$(this).next("input").attr("type","text");
		$(this).css("display","none");
		$(this).next("input").focus();
	}).next("input").blur(function (){
		var name = $(this).val();
		if(name != ""){
			$(this).prev(".bin_five_text").html(name);
			$(".bri_top_font").html(name);
			// 发送
			var wechatId = $('.bri_top').attr('data-wechat-id');
			var wxid = $('.bri_top').attr('data-wxid');
			editRoomName(wechatId, wxid, name);
		}
		$(this).prev(".bin_five_text").show();
		$(this).attr("type","hidden");
	});
	
	// 发送群公告
	$(".bin_five_note").hover(function (){
		$('.bin_five_note_img').show();
	}, function (){
		$('.bin_five_note_img').hide();
	}).click(function (){
		$(this).next("input").attr("type","text");
		$(this).css("display","none");
		$(this).next("input").focus();
	}).next("input").blur(function (){
		var content = $(this).val();
		if(content != ""){
			$(this).prev(".bin_five_note").html(content);
			// 发送
			var wechatId = $('.bri_top').attr('data-wechat-id');
			var wxid = $('.bri_top').attr('data-wxid');
			setRoomAnnouncement(wechatId, wxid, content);
		}
		$(this).prev(".bin_five_note").show();
		$(this).attr("type","hidden");
	});
	
	// 退出登录
	$('.bir_block_exit').click(function(){
		var wechatId = $('.bri_top').attr('data-wechat-id');
		logout(wechatId);
	});
	
	/*
	// 聊天窗口结束
	var wechatId = '097358B401A1D41FAA1B83D13C86AE97';
	var body = {"wxid":"wxid_vju0phxgdhgp23","username":"supper-busy3","nickname":"3我ddd是特殊的你d妹妹的吱mdjdjfff","headUrl":"http://wx.qlogo.cn/mmhead/ver_1/f2TNkO1UK3mg3H79RXRvXyzWNolNMPvC6OT5BaouiafKJP26BRer340djrqXgicoicSwBHqRx4zYu2YGJbJFibXWnDwq5pS8AmV8WibZfFtLroibs/132","mobile":"13711458538","email":"chengciming@vip.qq.com","sex":1,"nation":"CN","province":"Guangdong","city":"Guangzhou","device":"android"};
	loginSuccess(wechatId, body);
	
	var friend = {"wechatId":"99FE491DD2ACCA87F8694F68DD977F70","opCode":52,"body":{"friendList":[{"wxid":"23840874437@chatroom","username":"","v1":"","v2":"","type":"2","nickname":"","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmcrhead\/ZvE4OUWibbYmSeKNrmHcPicibxiaWbiaymWWMGic6GcWnBOUMUtlCtjhib0fK9icGyK5gZcqrCPeGZibLjKVs5PGeaGF8I55asjPFoHdT\/0","roomWxidList":"^Gwxid_vju0phxgdhgp22^G","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"0","roomStatus":"0"},{"wxid":"21584771086@chatroom","username":"","v1":"","v2":"","type":"2","nickname":"123456789","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmcrhead\/A8nnflTeeQ6l98dsq5NLIpx7DIm8g1CyeZ8BWJQyKp8NB6NnZZUCXT8MQe5JhL4lMaWISGq4je74ovjWKqqJuODQEnnV6cgR\/0","roomWxidList":"^Gwxid_vju0phxgdhgp22^G","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"0","roomStatus":"0"},{"wxid":"23146771271@chatroom","username":"","v1":"","v2":"","type":"2","nickname":"","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmcrhead\/YOVxuavf2t4M2NHPnxmztj6Q3RXQgjuqZFEnNGBOG1aR3VJ267qT0Zp5hnJ7BnicBMG3jETDD4qZ7xQytnFGvngO9dVz1kPmh\/0","roomWxidList":"^Gwxid_vju0phxgdhgp22^G","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"0","roomStatus":"0"},{"wxid":"22945281113@chatroom","username":"","v1":"","v2":"","type":"2","nickname":"","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmcrhead\/q9BPaHSPKGqFWiaa5P7V7A6mEvDgb10xQONzBWnXuVAD8sZD3gkafwcP14dQPGFHDWlFsqUTcFcl2cahEXPibbVicNJraC5mp68\/0","roomWxidList":"^Gwxid_vju0phxgdhgp22^G","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"0","roomStatus":"0"},{"wxid":"24494138792@chatroom","username":"","v1":"","v2":"","type":"2","nickname":"","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmcrhead\/QNqEdbuPaneyAX31Fz0mqoj9HkVWX6x2u4UmNxk7E6sXLnAjchTablVxibWCWwGyFxlQLJv4dFvEA8RqLlibYeBicGpej57MDyO\/0","roomWxidList":"^Gwxid_vju0phxgdhgp22^G","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"0","roomStatus":"0"},{"wxid":"23800075662@chatroom","username":"","v1":"","v2":"","type":"2","nickname":"","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmcrhead\/IveMbneItA4lT5JpUxuwhwbnCTo3cgNBkJjicuxUXVMEY9Xhiaz7DxTGXImwW8jqoHtC9eU46ZHujeLP7KRm4cSmBnPzJKwsjQ\/0","roomWxidList":"^Gwxid_vju0phxgdhgp22^G","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"0","roomStatus":"0"},{"wxid":"21987171159@chatroom","username":"","v1":"","v2":"","type":"2","nickname":"","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmcrhead\/7vAArmYXayrMy6UdU3lzQCESu0zrX3FyUqcA7Mtxzzcibpg27cxXjslR4t5cNqhTNicFicia9QaeIyEeFXqeZ4mGbq65r9YaJ6KW\/0","roomWxidList":"^Gwxid_vju0phxgdhgp22^G","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"0","roomStatus":"0"},{"wxid":"22750971244@chatroom","username":"","v1":"","v2":"","type":"2","nickname":"","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmcrhead\/TI9DgL0uI5QQ2BnmEN4UsrWH7yBsIKiadldN8HZPvffTBibGxwicfgpicK4PFxKGGbJhKg568OhW7weZBsRzqOhVibniadT95boYaW\/0","roomWxidList":"^Gwxid_vju0phxgdhgp22^G","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"0","roomStatus":"0"},{"wxid":"22972880856@chatroom","username":"","v1":"","v2":"","type":"2","nickname":"","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmcrhead\/NRTtdJ1JxkWiaEdtmiaK9cYsM1ONKiasasZ686OjiaBk7sx1WpftosfHMpoCQtQmStk5cTJMQVtKlw1cekUiaNaOYbqyzNJTVFkaJ\/0","roomWxidList":"^Gwxid_vju0phxgdhgp22^G","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"0","roomStatus":"0"},{"wxid":"asdasdasdasd","username":"","v1":"","v2":"","type":"0","nickname":"","roomType":"0","remark":"","headUrl":"(null)","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"Lemonice-cheng","username":"","v1":"v1_63a5c79810b2f39abe44093512a11d03de8b383ae9f83a539d559fe68e1b1544@stranger","v2":"v2_679c6c2dd446a8f2f8d78a8447887527eaaecbe6ef4d0995fb72bbec95e6cb04d1aef830e7109854a3ce04f1e12c98b8@stranger","type":"3","nickname":"Lemonice","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/lMsfZxdz7SfYgOnKR8CcF9d4v4icj14HtRn58JzZWlfCZN1cQT4iaXRCYGTX4H302sYOgqelFCMWt0BiapmuyfGXoEib5Eibr2xaBIalP8j5ZzEE\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"v1_3b1ca81b52d27be3dc3d4935fee4095a22a443a233b30861c8943abfef3ae4c3@stranger","username":"","v1":"","v2":"v2_f8d39be3c584aeded82770efa995f6c9f68d4e1a3523e0125abf881342b6e0ebbce65851ffce01593d16bc1fb9dd6bbb5b652dfd2ae9f99ebb3d2fc4d38e5e6afaef99e4f95088248a5a9bad46bebb3bc7fb550f5199afe0a0ec443f47f6565c883fe73e69a070e0ee765b9b90e93f1deaaadd6908b221f71744ecf2c86defc711c759b99be60cb2af328c2914a3c75caf8af9bd9dd156e37f5afbbaa0b4c654558bb4867b60e10ce4190689f4606702@stranger","type":"0","nickname":"\u963f\u5fb7","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/Yibia9ibKbyFVia6cXIboGicsWeQd1OPf1lwhqI26NIuCzdxW3VWrbp1qOEZVzgo1YuyCfhicJiaQJicOUmOQxPb1Z130YbZ0qPS8XPoMibRiaf1NKxW4\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"22905167168@chatroom","username":"","v1":"v1_17eba9f63dc6ae6b619d25b2d6d629fb121633daeb308197b3041ace70449e8c8fdbffba7a1f51984b1369cd1914a89d@stranger","v2":"","type":"3","nickname":"\u8fd9\u4e2a\u662f\u6d4b\u8bd5\u7684\u5fae\u4fe1\u7fa4","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmcrhead\/Ep81DRIzKK3PCm6VnGUDmZWnUUcfChxzI7QxSwsyL3icKyF0oQhPpRcgMoFO93Twp2zcE1N5Sw0AGLIMOnng6eKicItr5DJyet\/0","roomWxidList":"^Gwxid_vju0phxgdhgp22^GLemonice-cheng^G","roomOwner":"0","roomAnnouncement":"\u963f\u745f\u5927\u65f6\u4ee3","roomAnnouncementEditor":"wxid_vju0phxgdhgp22","roomAnnouncementPublishTime":"1582239217","roomStatus":"0"},{"wxid":"23096367433@chatroom","username":"","v1":"v1_a3b2f08c6f98646753d8f307e74054c0e445394811b796dd08c2c42c070f0275b047c1d8bdd446c515cc5d4b2c42468c@stranger","v2":"","type":"2","nickname":"\u5fae\u4fe1\u6d4b\u8bd5","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmcrhead\/qGusKyb0IEdhia3mBj49AdKlApmw0Bk9rOE31b0j75lYGAVHVhugTthNibia9s0wNTxw9TRQybrLenB6YRXHDW9wQl2B6sBl69F\/0","roomWxidList":"^GLemonice-cheng^Gwxid_vju0phxgdhgp22^G","roomOwner":"0","roomAnnouncement":"123456","roomAnnouncementEditor":"Lemonice-cheng","roomAnnouncementPublishTime":"1579618948","roomStatus":"0"},{"wxid":"wxid_z08ms09obu9721","username":"","v1":"","v2":"","type":"4","nickname":"\u55f3\u672c\u516e\uffe1\u55f3\u751f\u6d3b","roomType":"0","remark":"","headUrl":"(null)","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"wxid_yp06wf8lurde22","username":"","v1":"","v2":"","type":"4","nickname":"\u51a0\u519b\u9a91\u58eb","roomType":"0","remark":"","headUrl":"(null)","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"xy_2005","username":"","v1":"","v2":"","type":"4","nickname":"\u65e0\u75d5\u5c45\u58eb","roomType":"0","remark":"","headUrl":"(null)","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"wxid_7xp3gnomuvci21","username":"","v1":"","v2":"","type":"4","nickname":"\u7f16\u8f91","roomType":"0","remark":"","headUrl":"(null)","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"gzgchan","username":"","v1":"","v2":"","type":"4","nickname":"\u9648\u51ac\u660e","roomType":"0","remark":"","headUrl":"(null)","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"wxid_3pkodd1tgqv522","username":"","v1":"","v2":"","type":"4","nickname":"\u4e5d\u5dde\uff5ePC\u7248\u673a\u5668\u4eba~","roomType":"0","remark":"","headUrl":"(null)","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"filehelper","username":"","v1":"v1_0ab58fe8e8e11e97ce734957f749c03901b6d46dcec58f3d253de744bb85190c@stranger","v2":"","type":"2","nickname":"\u6587\u4ef6\u4f20\u8f93\u52a9\u624b","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/5qD3shjFSJtgLvgbcABm02qdmBRTJ10oyrckaZpUbGRhDA1Y0SULS8HwnfGQa1VJltzj4CqgxqBWfWehPPpvVZQXdMDl6dCdiaW5IGr53iaOM\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"9199093107@chatroom","username":"","v1":"v1_ce9e44d4aee7f4b5fa3a48d81036e426f5ce32c1031d6284a4ac1519aa0485ac4b91ca344061a4f708fa7e1f56521a0f@stranger","v2":"","type":"2","nickname":"\u6d4b\u8bd5\u7fa4\u540d\u79f0","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmcrhead\/xxib5HEohdRiaO26jEOkqqEqPgvUHYaFdibfA1ia4ogIGnlCSc1FSvv630hUicTQh4lGbABukxeptmKeZhjvLia7rFD1nFT1Qe4EJB\/0","roomWxidList":"^Ggzgchan^Gwxid_3pkodd1tgqv522^GLemonice-cheng^Gwxid_vju0phxgdhgp22^Gwxid_7xp3gnomuvci21^Gxy_2005^Gwxid_yp06wf8lurde22^G","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"0","roomStatus":"0"},{"wxid":"12807061681@chatroom","username":"","v1":"v1_440ac9b4a029e8fa91f71282b03c26afbd48e558f6955739b93ded1826cf300b1699acbc67adf5e268b49bb48d325b3f@stranger","v2":"","type":"2","nickname":"\u6d4b\u8bd5\u5fae\u4fe1\u673a\u5668\u4eba\u4e13\u7528\u7fa4","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmcrhead\/RhONPFh9Sb8MSAetQAWHLCtzv50SHssdak1U8dNdRgLBsHCJwfpGgsDLybzzvRRn6iaDvEWefgoicicOquibSz43bbicgL9zTia5FK\/0","roomWxidList":"^GLemonice-cheng^Gwxid_z08ms09obu9721^Gwxid_7xp3gnomuvci21^Gxy_2005^Gwxid_vju0phxgdhgp22^G","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"0","roomStatus":"0"},{"wxid":"wxid_vju0phxgdhgp22","username":"","v1":"","v2":"","type":"0","nickname":"\u6211ddd\u662f\u7279\u6b8a\u7684\u4f60d\u59b9\u59b9\u7684\u5431mdjdjfff","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/TPeRIWUHjqVkF9Z7WyMgbCibv53OHyuxPc708JaxJR1TQ7ricok34C4qkjeXicyevrffvQY04ic613jqibV2ibLMvC3Ud5hKWRDF7DwCJfAElfmWA\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"wxid_8du0u27rttry22","username":"abclh1122","v1":"v1_f53a48c6550d289999e0943afe4c6765865ca989a815f402ba680482147fed740ed43f259fa146e2d9928fcef557dd88@stranger","v2":"","type":"1","nickname":"A\u80d6\u4ed9\u5973\ue003\uff08\u5c0f\u9a6c\uff09","roomType":"1","remark":"\u5c0f\u9a6c\ue110","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/WonSemcZOHLh2z2dVeFCaicGh1iaXyK2cDdGTWRRqRIoZuEG7KEJYXJq39ibOQGcvCLjXtlYCUdS8v8iawKibRFVzkJWFa6bUicdPYw7z6Kh0IKAY\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"wxid_jq8o82gaeos421","username":"sssloveh520","v1":"v1_054db123de94e4b9b680ac5393b5599229ee1fa084e013748af65173a0228582dd579061b26f50c0f7b1a71bb4b6bc42@stranger","v2":"","type":"1","nickname":"\u7d20\u7d20","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/1wLHib42PIrhW26SUsyCX3KXcTNIQKX9FPVpMfEeHuFh6via8R0ScYjiaoQh7fhIY0JgTYByNTQS4pQNhdllICj20WnDjTC3pE5PJYqfG6EnjQ\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"shx_ok","username":"wawa867961565","v1":"","v2":"","type":"65545","nickname":"\u77f3\u6d77\u971e\u00b9\u00b3\u2078\u00b2\u2076\u2074\u00b9\u2077\u2077\u2074\u2075","roomType":"0","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/iaasHWia3osmq1LqZZrGSSMyaf7AcZEAmgDOjBIniajKHPEvWRSJjwAnH3NrDyLXEEU1ftfAq8AjAH9Yt6tpoicXXvQXFkm0CSn4ZUwiaxkdn2ME\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"wxid_sbrnzc86ibft22","username":"s18924200025","v1":"v1_2fa19913639b9dde54c783cbc4511c75a5957e157ebd2164683240918d59be169b5cef8d0a626c2febe0ecf2addaec2c@stranger","v2":"","type":"1","nickname":"AOE","roomType":"1","remark":"\u6c88\u80fd\u5f3a","headUrl":"","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"gh_3dfda90e39d6","username":"wxzhifu","v1":"v1_b91353590974fd62828618e3eedb4a8d0d7d5bdc53be3bd8f1b1579641acf625@stranger","v2":"","type":"1","nickname":"\u5fae\u4fe1\u652f\u4ed8","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/M9xTDTq2pqE7ZibBf1TTOusFonNL695RzZySkEG9tUICwCRnUAzdrpOqYCToLCeQNGMicaKxNNAqPTE59SAMABHHtoLWcoGX27ibGpW954Poia4\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"gh_267292b8f733","username":"","v1":"v1_9531f8bc67d85107a269b738b3bcfa90f672dbb13da21f8bc978eee475f280a2@stranger","v2":"","type":"1","nickname":"\u8d5e\u64ad\u667a\u5e97","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/IpjlgxKcL1EBKLelMrXCKrLVerV82UoApCZ8zBJiaEpkOWtXjTU9m2fCzeUts9gTicAUwFIojPHJsqpNVaknLORNe2xfpbAd5mEuO2zia846mw\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"gh_c57d89fdfe84","username":"","v1":"v1_1a35eb4cc8ff36c003a90f7d8a38c0abf64ecc03b7ad2a92cd2ec604a0e5bb71@stranger","v2":"","type":"1","nickname":"\u5e7f\u5dde\u8d5e\u64ad","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/W2VIJyd11ed1kmtYBgyA59oSJ7dlD7llgcFkk3NPAM0GNy7BKjz7YCW2flyiccpia0cQWKyr9Hzt8iaflw70U3f9HFF2gicLRouAa7Ge6ey7fDE\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"wxid_ex8vs2ew6u5j12","username":"yfsx5201314","v1":"v1_d0bebd57db3ead25fdc4c9632853fe0a6a11f06cc4c3b2906bd0f4e2e484f6b348ed932e77c98e558421d02e2cf234a7@stranger","v2":"","type":"1","nickname":"\u96e8\u96ea\u7eb7\u98de","roomType":"1","remark":"\u74f7\u808c-\u68ee\u8d24","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/iawpq15hKibHLoeeARDH3bf2OKrBXDelnibYosAZMMnGRXzibw1pd9I82oibDkmrnDCibfaL60mpCM0ibFwdTS5rNChibLVFEGFvqcVCXXsbwfL3kXY\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"gh_82933d6ccba8","username":"sjdspxy","v1":"v1_15b43abd78ad478a23b25a510fd0b6337cead20cfc6f822ed4c4b7e869b5d321@stranger","v2":"","type":"1","nickname":"\u62fc\u62fc\u4fa0\u6279\u53d1\u5546\u57ce","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/YhUPqZjKuhov8rYnzkGibjI85s2PuwatMm49K2C9ST6YyFeiamu2tR1gKEekWmTNiaBEBmuhKPAQasYv4vwad4KWTyesPBJBtCXaia1pgZ3eKDw\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"wxid_rk4yybsdrjju22","username":"wj15915865278","v1":"v1_e37ea36b912c4c04e11dc1067bec1431945f53c701dca4f431a4360ae50ec3a628165923f98424976eb38bc43307ba44@stranger","v2":"","type":"1","nickname":"\u738b\u5065","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/t2BHTfsDAoPJJEOOq2Z8EZamiaxrl4vejrdgianReH7Jiaw3zdX4qNccyPQWjyPuR8TaWYo6iaYbichU3IicMX4sTuibN3Q5Zewicvmas4iaC33mBAAo\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"wxid_anvdmk35f7kq22","username":"wf7kq22","v1":"","v2":"","type":"11","nickname":"\u4e03\u70ab","roomType":"0","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/dZVPO8lFbZzQKIb06czQEG5qzyCgKqls1jreGZByEsBzAE9b4CyCQn5eSU31VFNicKg35oqDy7uLib7NSq9FwCiarsTicWgpLsbvgExqrW0Qxwc\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"wxid_4jfhuk0twqz322","username":"jkjijjhj","v1":"","v2":"","type":"11","nickname":"\u5c0f\u4e39\uff5e*","roomType":"0","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/lXUOUicaqZCP7AjEE0iaCZn3hCueqFs8CQWhvS2sR4w1Sy1w6VkV2OjBtwSiblgyHM1RVGISwZl39AmTqD1YeXcaW1RAiciaQialXtwUMia7jLpXia4\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"gh_7aac992b0363","username":"","v1":"v1_36eb3731f112cf060814663622ef127d7aa2136e11abf89c957fdd774f3a8de5@stranger","v2":"","type":"1","nickname":"\u5fae\u4fe1\u5361\u5305","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/7iaMjlk40EjIj4fXXJWia6icv2P2q5ccytZXSzGJGwicvDUjyVeZnoXzPialARic2wyrfsOGUVViap4703yiaLibOsQl9MOPXN27aKIicQjicXT77NuvLk\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"wxid_3zisxl1q943i22","username":"babb72","v1":"","v2":"","type":"11","nickname":"\u6b27\u9633\u6155\u96ea","roomType":"0","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/6ibEOibtjDEnr208w4ic1f6pCxC7vQKar7FPWDpaNWRM2St4sjSgnsE0akSext1dtwIzcAAK6MiajauGesKRibkpA3G5C0ZtKr0kqBeZf6I3U3L4\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"wxid_r4j7n8uildx122","username":"w13730979996","v1":"","v2":"","type":"11","nickname":"\u66fe\u7ecf\u7684\u5c11\u5e74\u68a6","roomType":"0","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/rlE1dwFLDwD2ib6FPyvRTJ7SnVTicBozJCicnHCh1PVdDQQUibXgYxVDQ6zG8ByWD57TGqhNS4dTChzEVHhBoMQib31myPEYlIJrb1vP78MP5AJc\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"wxid_ffn7hhp6x1j422","username":"zketpr","v1":"","v2":"","type":"11","nickname":"\u82cf\u96ef\ud83c\udf38","roomType":"0","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/icE2eYibEF95sXH8VZiboqeibKSG8Tremu8gsEnn4AbiaKdFrz3V1rMiclaKZB5HGo9DCLrkDKtZ3ayyhIdoSK4LFibrT7toLluziankE3gTHQ47DgI\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"qqmail","username":"","v1":"","v2":"","type":"33","nickname":"QQ\u90ae\u7bb1\u63d0\u9192","roomType":"0","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/XQrLN6uScF1IO20oqVqEyQas5DMIy8Dm9NlRTRWEiaO84vUqAM6HaMo8jbmxicMkIKawROuibhdI0RicUezWaEb2VQ\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"qmessage","username":"","v1":"","v2":"","type":"1","nickname":"QQ\u79bb\u7ebf\u6d88\u606f","roomType":"0","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/gaPHOribY3okmeIwiarhibw8vmI9cnkY5FGQHBwIY91595AAqwgXXzsVd4n8Qpwj5ALOic0Rgab4ndiaHvAAdCl4fVQ\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"floatbottle","username":"","v1":"","v2":"","type":"1","nickname":"\u6f02\u6d41\u74f6","roomType":"0","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/f1GRX7BxGW5gAkvxVITVjUxbE0lSicwYBhHiaic5cYNPMyr06wFibZakOpTOpdw7akicSREqJknjvtGZZOVnw8rQHeA\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"medianote","username":"","v1":"v1_222a4651d3631fca4a0ac3aae7d7a3a0ece364d082f2fbe6a4b2b8ca13c0a3a0@stranger","v2":"","type":"35","nickname":"\u8bed\u97f3\u8bb0\u4e8b\u672c","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/Q121TGOmhmlm8TlzZArYiax5lonWAS5F8r8E3rdluUsuSl8XZeibNsl2OibCb2t9Hd9GiauRFn7RmfDQVngQ5kxo4Q\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"},{"wxid":"fmessage","username":"","v1":"v1_1db322fb5f62d32608c2d4e089013f354714746bb0eecceae28b7a807a30b9ce@stranger","v2":"","type":"33","nickname":"\u670b\u53cb\u63a8\u8350\u6d88\u606f","roomType":"1","remark":"","headUrl":"http:\/\/wx.qlogo.cn\/mmhead\/ver_1\/TNhoUBbrJLac5lramgqgZdIzUgL0nJOakSZf5G71mqw6qYCeGxS8jkicZgyjv0oQjXsSQLgkFrvyB3DTiagwAhrg\/0","roomWxidList":"","roomOwner":"0","roomAnnouncement":"","roomAnnouncementEditor":"","roomAnnouncementPublishTime":"","roomStatus":"0"}]}};
	showFriendList(wechatId, friend.body.friendList);
	
	//var roomMsg = {"msg_id":"311f060af239d7ad2c058c62b60aa319","msg_type":1,"msg_source":1,"wxid":"Lemonice-cheng","roomId":"22905167168@chatroom","room_xml":"<msgsource>\n\t<silence>0</silence>\n\t<membercount>2</membercount>\n</msgsource>\n","content":"还是宿舍"};
	var roomMsg = {"msg_id":"a0b66a5f54e677dc103fc121c3adf6c2","msg_type":1,"msg_source":0,"wxid":"Lemonice-cheng","room_id":"","room_xml":"","content":"#接龙\n后端组1：2月21日个人身体数据开始统计，个人必须接龙。（\n例：姓名+体温+无症状）接龙如下：\n\n1. 王海+36.4+无症状\n2. 黄锦冰 36.5 无症状"};
	recvMessage(wechatId, roomMsg);
	*/
});