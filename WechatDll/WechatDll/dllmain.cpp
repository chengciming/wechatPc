// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "Config.h"
#include "resource.h"
#include "dllmain.h"
#include "LoginInfo.h"
#include "WebSocket.h"
#include "Login.h"
#include "Message.h"
#include "Friends.h"
#include "Room.h"
#include "Common.h"
#include "Send.h"
#include "Sql.h"
#include "Transfer.h"
#include "Download.h"
#include "Receive.h"


BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	 
    switch (ul_reason_for_call)
    {
		case DLL_PROCESS_ATTACH:
		{
			// 开新线程处理
			/*
			// 此处是调试界面，存在不完善情况
			HANDLE lThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ShowDialog, hModule, NULL, 0);
			if (lThread != 0) {
				CloseHandle(lThread);
			}
			*/
			// 开始监控微信
			
			HANDLE mThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Monitor, hModule, NULL, 0);
			if (mThread != 0) {
				CloseHandle(mThread);
			}

			// 开一个线程开始连接服务端WebSocket
			HANDLE cThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WebSocketClientMonitor, hModule, NULL, 0);
			if (cThread != 0) {
				CloseHandle(cThread);
			}

			break;
		}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}
/*
监控登录信息 - 需要等待登录结果
*/
DWORD WINAPI LoginMonitor(HMODULE hModule)
{
	// 跳转到二维码界面
	GotoQrCode();
	//修改内存版本号
	WriteData();
	// 定时监听登录状态
	int number = 0;
	bool sendQrcode = FALSE;
	while (TRUE) {
		// 10秒发一次二维码
		if (!(number % 10)) {
			sendQrcode = TRUE;
		}
		bool loginStatus = SendLoginStatus(sendQrcode);
		if (loginStatus == TRUE) {
			break;
		}
		sendQrcode = FALSE;
		number++;
		// 90秒换一次二维码
		if (number == 90) {
			// 跳转到二维码界面
			GotoQrCode();
			number = 0;
		}
		Sleep(1000);
	}
	// 发送登录下登录账号的详情回去
	//SendWechatUser();

	// 检查是否退出登录
	while (TRUE) {
		// 已经退出登录了
		if (CheckLoginStatus() == 0) {
			Receive::SendLogout();
			break;
		}
		Sleep(5000);  // 每隔5秒检查一次
	}

	return TRUE;
}
/*
监控WebSocket
*/
DWORD WINAPI WebSocketClientMonitor(HMODULE hModule)
{
	WsClientInit();  // 初始化

	// 开一个线程获取登录信息 - 需要等待登录结果
	HANDLE cThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LoginMonitor, hModule, NULL, 0);
	if (cThread != 0) {
		CloseHandle(cThread);
	}
	return TRUE;
}
/*
监控微信
*/
DWORD WINAPI Monitor(HMODULE hModule)
{
	// 注入数据库句柄监听
	ListenDbHandle();
	// 注入好友列表更新监听 - 暂时不监听，发现微信刷新很频繁
	//ListFriendsList();
	// 注入消息监听
	ListenMessage();

	return TRUE;
}

/*
显示界面
*/
DWORD WINAPI ShowDialog(HMODULE hModule)
{	
	// 显示界面
	DialogBox(hModule, MAKEINTRESOURCE(ID_WECHAT), NULL, &Dlgproc);
	return TRUE;
}

/*
界面事件触发 - 回调
*/
INT_PTR CALLBACK Dlgproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		// 初始化
		case WM_INITDIALOG:
			SetGlobalHwnd(hwndDlg);
			break;
		case WM_COMMAND:
			// 刷新数据
			if (wParam == BT_ID_READ_MEMORY) {
				ReadWechatUser();
			}
			// 同意加好友请求
			if (wParam == ID_BT_FRIENDS_VERIFY) {
				// 接收信息
				wchar_t v1[0x200] = { 0 };
				wchar_t v2[0x200] = { 0 };
				GetDlgItemText(hwndDlg, ID_FRIENDS_V1, v1, sizeof(v1));
				GetDlgItemText(hwndDlg, ID_FRIENDS_V2, v2, sizeof(v2));

				VerifyFriend(v1, v2);
			}
			// 设置好友备注
			if (wParam == ID_BT_SET_REMARK) {
				// 接收信息
				wchar_t wxid[0x200] = { 0 };
				wchar_t remark[0x200] = { 0 };
				GetDlgItemText(hwndDlg, ID_SET_REMARK_WXID, wxid, sizeof(wxid));
				GetDlgItemText(hwndDlg, ID_SET_REMARK_TEXT, remark, sizeof(remark));

				SetRemark(wxid, remark);
			}
			// 加好友
			if (wParam == ID_BT_ADD_USER) {
				// 接收信息
				wchar_t wxid[0x100] = { 0 };
				wchar_t message[0x100] = { 0 };
				GetDlgItemText(hwndDlg, ID_ADD_USER_WXID, wxid, sizeof(wxid));
				GetDlgItemText(hwndDlg, ID_ADD_USER_MSG, message, sizeof(message));

				AddFriend(wxid, message);
			}
			// 名片加好友
			if (wParam == ID_BT_ADD_USER_CARD) {
				// 接收信息
				wchar_t v1[0x100] = { 0 };
				wchar_t message[0x100] = { 0 };
				GetDlgItemText(hwndDlg, ID_ADD_USER_CARD_V1, v1, sizeof(v1));
				GetDlgItemText(hwndDlg, ID_ADD_USER_CARD_VERTIFY, message, sizeof(message));

				AddUserFromCard(v1, message);
			}
			// 获取群成员列表
			if (wParam == ID_BT_GET_ROOM_USER) {
				// 接收信息
				wchar_t roomId[0x100] = { 0 };
				GetDlgItemText(hwndDlg, ID_GET_ROOM_USER, roomId, sizeof(roomId));

				GetRoomUserList(roomId);
			}
			// 创建群聊
			if (wParam == ID_BT_CREATE_ROOM) {
				// 接收信息
				wchar_t wxid1[0x100] = { 0 };
				wchar_t wxid2[0x100] = { 0 };
				GetDlgItemText(hwndDlg, ID_CREATE_ROOM_WXID1, wxid1, sizeof(wxid1));
				GetDlgItemText(hwndDlg, ID_CREATE_ROOM_WXID2, wxid2, sizeof(wxid2));

				CreateRoom(wxid1, wxid2);
			}
			// 拉好友入群
			if (wParam == ID_BT_ADD_ROOM_USER) {
				// 接收信息
				wchar_t chatroomwxid[0x100] = { 0 };
				wchar_t wxid[0x100] = { 0 };
				GetDlgItemText(hwndDlg, ID_ADD_ROOM_USER_ROOMID, chatroomwxid, sizeof(chatroomwxid));
				GetDlgItemText(hwndDlg, ID_ADD_ROOM_USER_WXID, wxid, sizeof(wxid));

				AddRoomUser(chatroomwxid, wxid);
			}
			// 发送群公告
			if (wParam == ID_BT_SET_ROOM_ANNOUNCEMENT) {
				// 接收信息
				wchar_t chatroomwxid[0x100] = { 0 };
				wchar_t Announcement[0x200] = { 0 };
				GetDlgItemText(hwndDlg, ID_SET_ROOM_ANNOUNCEMENT_ROOMID, chatroomwxid, sizeof(chatroomwxid));
				GetDlgItemText(hwndDlg, ID_SET_ROOM_ANNOUNCEMENT_TEXT, Announcement, sizeof(Announcement));

				SetRoomAnnouncement(chatroomwxid, Announcement);
			}
			// 修改群名称
			if (wParam == ID_BT_SET_ROOM_NAME) {
				// 接收信息
				wchar_t chatroomwxid[0x100] = { 0 };
				wchar_t roomname[0x200] = { 0 };
				GetDlgItemText(hwndDlg, ID_SET_ROOM_NAME_ROOMID, chatroomwxid, sizeof(chatroomwxid));
				GetDlgItemText(hwndDlg, ID_SET_ROOM_NAME_TEXT, roomname, sizeof(roomname));

				SetRoomName(chatroomwxid, roomname);
			}
			// 删除群成员
			if (wParam == ID_BT_DEL_ROOM_USER) {
				// 接收信息
				wchar_t chatroomwxid[0x100] = { 0 };
				wchar_t wxid[0x100] = { 0 };
				GetDlgItemText(hwndDlg, ID_DEL_ROOM_USER_ROOMID, chatroomwxid, sizeof(chatroomwxid));
				GetDlgItemText(hwndDlg, ID_DEL_ROOM_USER_WXID, wxid, sizeof(wxid));

				DelRoomUser(chatroomwxid, wxid);
			}
			// 退出群聊
			if (wParam == ID_BT_QUIT_ROOM) {
				// 接收信息
				wchar_t chatroomwxid[0x100] = { 0 };
				GetDlgItemText(hwndDlg, ID_QUIT_ROOM_ROOMID, chatroomwxid, sizeof(chatroomwxid));

				QuitRoom(chatroomwxid);
			}
			// 删除好友
			if (wParam == ID_BT_DELETE_USER) {
				// 接收信息
				wchar_t wxid[0x100] = { 0 };
				GetDlgItemText(hwndDlg, ID_DELETE_USER_WXID, wxid, sizeof(wxid));

				DeleteUser(wxid);
			}
			// 发送艾特群成员消息
			if (wParam == ID_BT_SEND_ROOM_AT) {
				// 接收信息
				wchar_t chatroomid[0x100] = {0};
				wchar_t wxid[0x100] = {0};
				wchar_t nickName[0x100] = {0};
				wchar_t message[0x200] = {0};
				GetDlgItemText(hwndDlg, ID_SEND_ROOM_AT_ROOMID, chatroomid, sizeof(chatroomid));
				GetDlgItemText(hwndDlg, ID_SEND_ROOM_AT_WXID, wxid, sizeof(wxid));
				GetDlgItemText(hwndDlg, ID_SEND_ROOM_AT_NICKNAME, nickName, sizeof(nickName));
				GetDlgItemText(hwndDlg, ID_SEND_ROOM_AT_MESSAGE, message, sizeof(message));

				// 发送消息
				SendRoomAtMsg(chatroomid, wxid, nickName, message);
			}
			// 发送消息
			if (wParam == ID_BT_SEND) {
				// 接收信息
				wchar_t wxid[0x100] = {0};
				wchar_t message[0x300] = {0};
				GetDlgItemText(hwndDlg, ID_SEND_WXID, wxid, sizeof(wxid));
				GetDlgItemText(hwndDlg, ID_SEND_MESSAGE, message, sizeof(wxid));

				// 发送消息
				SendTextMessage(wxid, message);
			}
			// 发送XML名片
			if (wParam == ID_BT_SEND_XML_CARD) {
				// 接收信息
				wchar_t recverWxid[0x100] = {0};
				wchar_t xml[0x2000] = {0};
				GetDlgItemText(hwndDlg, ID_SEND_XML_CARD_RECVER_WXID, recverWxid, sizeof(recverWxid));
				GetDlgItemText(hwndDlg, ID_SEND_XML_CARD_XML, xml, sizeof(xml));

				// <?xml version="1.0"?><msg bigheadimgurl="http://wx.qlogo.cn/mmhead/ver_1/7IiaGRVxyprWcBA9v2IA1NLRa1K5YbEX5dBzmcEKw4OupNxsYuYSBt1zG91O6p07XlIOQIFhPCC3hU1icJMk3z28Ygh6IhfZrV4oYtXZXEU5A/0" smallheadimgurl="http://wx.qlogo.cn/mmhead/ver_1/7IiaGRVxyprWcBA9v2IA1NLRa1K5YbEX5dBzmcEKw4OupNxsYuYSBt1zG91O6p07XlIOQIFhPCC3hU1icJMk3z28Ygh6IhfZrV4oYtXZXEU5A/132" username="Lemonice-cheng" nickname="Lemonice" fullpy="?" shortpy="" alias="微信大号" imagestatus="3" scene="17" province="北京" city="中国" sign="" sex="2" certflag="0" certinfo="" brandIconUrl="" brandHomeUrl="" brandSubscriptConfigUrl= "" brandFlags="0" regionCode="CN_BeiJing_BeiJing" />

				// 发送消息
				SendXmlCardMessage(recverWxid, xml);
			}
			// 发送XML文章
			if (wParam == ID_BT_SEND_XML_ARTICLE) {
				// 接收信息
				wchar_t recverWxid[0x100] = {0};
				wchar_t sendWxid[0x100] = {0};
				wchar_t imageFilePath[0x256] = {0};
				wchar_t xmlCentent[0x2000] = {0};
				GetDlgItemText(hwndDlg, ID_SEND_XML_ARTICLE_RECV_WXID, recverWxid, sizeof(recverWxid));
				GetDlgItemText(hwndDlg, ID_SEND_XML_ARTICLE_FROM_WXID, sendWxid, sizeof(sendWxid));
				GetDlgItemText(hwndDlg, ID_SEND_XML_ARTICLE_IMAGE_PATH, imageFilePath, sizeof(imageFilePath));
				GetDlgItemText(hwndDlg, ID_SEND_XML_ARTICLE_XML, xmlCentent, sizeof(xmlCentent));

				/*
				Lemonice-cheng

				wxid_vju0phxgdhgp22

				C:\Users\Lemonice\Documents\WeChat Files\supper-busy\FileStorage\Cache\2020-02\5daa8a38d559496923745330165948b3_t.jpg
				*/

				/*
				<msg><appmsg appid=\"\" sdkver=\"0\"><title>%s</title><des>%s</des><username></username><action>view</action"
				 "><type>5</type><showtype>0</showtype><content></content><url>%s</url><lowurl></lowurl><dataurl></dataurl><l"
				 "owdataurl></lowdataurl><contentattr>0</contentattr><streamvideo><streamvideourl></streamvideourl><streamvid"
				 "eototaltime>0</streamvideototaltime><streamvideotitle></streamvideotitle><streamvideowording></streamvideow"
				 "ording><streamvideoweburl></streamvideoweburl><streamvideothumburl></streamvideothumburl><streamvideoaduxin"
				 "fo></streamvideoaduxinfo><streamvideopublishid></streamvideopublishid></streamvideo><canvasPageItem><canvas"
				 "PageXml><![CDATA[]]></canvasPageXml></canvasPageItem><appattach><totallen>0</totallen><attachid></attachid>"
				 "<cdnattachurl></cdnattachurl><emoticonmd5></emoticonmd5><aeskey></aeskey><fileext></fileext><islargefilemsg"
				 ">0</islargefilemsg></appattach><extinfo></extinfo><androidsource>3</androidsource><thumburl>%s</thumburl><m"
				 "ediatagname></mediatagname><messageaction><![CDATA[]]></messageaction><messageext><![CDATA[]]></messageext>"
				 "<emoticongift><packageflag>0</packageflag><packageid></packageid></emoticongift><emoticonshared><packagefla"
				 "g>0</packageflag><packageid></packageid></emoticonshared><weappinfo><username></username><appid></appid></w"
				 "eappinfo><designershared><designeruin>0</designeruin><designername>null</designername><designerrediretcturl"
				 ">null</designerrediretcturl></designershared><emotionpageshared><tid>0</tid><title>null</title><desc>null</"
				 "desc><iconUrl>null</iconUrl><secondUrl>null</secondUrl><pageType>0</pageType></emotionpageshared><webviewsh"
				 "ared><shareUrlOriginal>%s</shareUrlOriginal><shareUrlOpen>%s</shareUrlOpen><jsAppId></jsAppId><publisherId>"
				 "</publisherId></webviewshared><template_id>0</template_id><statextstr></statextstr><md5></md5></appmsg><fro"
				 "musername></fromusername><scene>0</scene><appinfo><version>1</version><appname></appname></appinfo><comment"
				 "url></commenturl></msg>
				*/
				// <msg><fromusername>wxid_vju0phxgdhgp22</fromusername><scene>0</scene><commenturl></commenturl><appmsg appid="wx22571cb374464448" sdkver="0"><title>Redmi Note 8 Pro 8GB+128GB 贝母白</title><des>6400万旗舰级全场景四摄 / 前置2000万 AI美颜相机 / G90T专业游戏芯片，液冷散热 / 6.53英寸水滴全面屏 / 4500mAh大电量 / 标配18W快充 / 康宁大猩猩保护玻璃 / 多功能NFC / 红外遥控功能 / Game Turbo2.0游戏加速</des><action>view</action><type>5</type><showtype>0</showtype><content></content><url>https://youyu.xiaomiyoupin.com/detail?gid=112041&amp;invite_code=WYZG9F60</url><dataurl></dataurl><lowurl></lowurl><thumburl></thumburl><messageaction></messageaction><recorditem><![CDATA[]]></recorditem><weappinfo><pagepath></pagepath><username></username><appid></appid><appservicetype>0</appservicetype></weappinfo><md5>c0be00f9dc5f210fcdca5cd0903c2d1c</md5><extinfo></extinfo><sourceusername></sourceusername><sourcedisplayname></sourcedisplayname><commenturl></commenturl><appattach><totallen>0</totallen><attachid></attachid><emoticonmd5></emoticonmd5><fileext></fileext><cdnthumburl>305d020100045630540201000204464db91d02032f540602048d8a903a02045e506a23042f6175706170706d73675f353362383066366139306631393334345f313538323332383335333839345f3432313932310204010800030201000400</cdnthumburl><aeskey>0aca6957e98d8300db8e556df7f5086d</aeskey><cdnthumbaeskey>0aca6957e98d8300db8e556df7f5086d</cdnthumbaeskey><encryver>1</encryver><cdnthumblength>13225</cdnthumblength><cdnthumbheight>120</cdnthumbheight><cdnthumbwidth>120</cdnthumbwidth></appattach><websearch /></appmsg><appinfo><version>1</version><appname>有品有鱼</appname></appinfo></msg>

				// 发送消息
				SendXmlMessage(0x5, recverWxid, sendWxid, imageFilePath, xmlCentent);
			}
			// 发送XML小程序
			if (wParam == ID_BT_SEND_XML_XCX) {
				// 接收信息
				wchar_t recverWxid[0x100] = { 0 };
				wchar_t sendWxid[0x100] = { 0 };
				wchar_t imageFilePath[0x256] = { 0 };
				wchar_t xmlCentent[0x2000] = { 0 };
				GetDlgItemText(hwndDlg, ID_SEND_XML_XCX_RECV_WXID, recverWxid, sizeof(recverWxid));
				GetDlgItemText(hwndDlg, ID_SEND_XML_XCX_FROM_WXID, sendWxid, sizeof(sendWxid));
				GetDlgItemText(hwndDlg, ID_SEND_XML_XCX_IMAGE_PATH, imageFilePath, sizeof(imageFilePath));
				GetDlgItemText(hwndDlg, ID_SEND_XML_XCX_XML, xmlCentent, sizeof(xmlCentent));

				/*
				Lemonice-cheng

				wxid_vju0phxgdhgp22

				C:\Users\Lemonice\Documents\WeChat Files\supper-busy\FileStorage\Cache\2020-02\9620f0a152dd263eaba5f5b0bfe6ceeb_t.jpg
				*/
				// <msg><fromusername>wxid_vju0phxgdhgp22</fromusername><scene>0</scene><commenturl></commenturl><appmsg appid="" sdkver="0"><title>主播「赞播」正在直播【十步玩法，穿越直播时代！】</title><des></des><action>view</action><type>33</type><showtype>0</showtype><content></content><url>https://mp.weixin.qq.com/mp/waerrpage?appid=wx60d176f873ca2d67&amp;type=upgrade&amp;upgradetype=3#wechat_redirect</url><dataurl></dataurl><lowurl></lowurl><thumburl></thumburl><messageaction></messageaction><recorditem><![CDATA[]]></recorditem><weappinfo><pagepath>pages/room/main.html?r=1358529232&amp;p=0_114539&amp;from=1&amp;test_id=2&amp;o=o038N5Ns8_dXoxK2yULR3KUwMC18</pagepath><username>gh_5ce47abb1b76@app</username><appid>wx60d176f873ca2d67</appid><version>368</version><type>2</type><weappiconurl>http://wx.qlogo.cn/mmhead/Q3auHgzwzM6oVB1laWBdADOQqAWPoG7U4MSc4swS9VxoYnoickjH6Dw/96</weappiconurl><appservicetype>0</appservicetype><shareId>1_wx60d176f873ca2d67_2038130101_1582114038_0</shareId></weappinfo><md5>70bc225ea8666d1192c6c8d26f772f99</md5><extinfo></extinfo><sourceusername></sourceusername><sourcedisplayname>看点直播</sourcedisplayname><commenturl></commenturl><appattach><totallen>0</totallen><attachid></attachid><emoticonmd5></emoticonmd5><fileext></fileext><cdnthumburl>305d020100045630540201000204464db91d02032df7f902042ee7e16a02045e519cfe042f6175706170706d73675f353362383066366139306631393334345f313538323430363931303335375f3432323235370204010800030201000400</cdnthumburl><aeskey>1334a60620d4f2ebf13ba5d6a9dd6b44</aeskey><cdnthumbaeskey>1334a60620d4f2ebf13ba5d6a9dd6b44</cdnthumbaeskey><encryver>1</encryver><cdnthumblength>68916</cdnthumblength><cdnthumbheight>576</cdnthumbheight><cdnthumbwidth>720</cdnthumbwidth></appattach><websearch /></appmsg><appinfo><version>1</version><appname>Window wechat</appname></appinfo></msg>
				
				// 发送消息
				SendXmlMessage(0x21, recverWxid, sendWxid, imageFilePath, xmlCentent);
			}
			// 发送图片
			if (wParam == ID_BT_SEND_IMAGE) {
				// 接收信息
				wchar_t wxid[0x100] = {0};
				wchar_t filepath[MAX_PATH] = {0};
				GetDlgItemText(hwndDlg, ID_SEND_IMAGE_WXID, wxid, sizeof(wxid));
				GetDlgItemText(hwndDlg, ID_SEND_IMAGE_FILE_PATH, filepath, sizeof(filepath));

				// 发送消息
				SendImageMessage(wxid, filepath);
			}
			// 发送文件
			if (wParam == ID_BT_SEND_FILE) {
				// 接收信息
				wchar_t wxid[0x100] = {0};
				wchar_t filepath[MAX_PATH] = {0};
				GetDlgItemText(hwndDlg, ID_SEND_FILE_WXID, wxid, sizeof(wxid));
				GetDlgItemText(hwndDlg, ID_SEND_FILE_FILE_PATH, filepath, sizeof(filepath));

				// 发送消息
				SendFileMessage(wxid, filepath);
			}
			// 收款
			if (wParam == ID_BT_TRANSFER) {
				// 接收信息
				wchar_t wxid[0x100] = {0};
				wchar_t transferId[MAX_PATH] = {0};
				GetDlgItemText(hwndDlg, ID_TRANSFER_WXID, wxid, sizeof(wxid));
				GetDlgItemText(hwndDlg, ID_TRANSFER_ID, transferId, sizeof(transferId));

				// 发送消息
				CollectMoney(wxid, transferId);
			}
			break;
		default:
			return FALSE;
	}

	return FALSE;
}
