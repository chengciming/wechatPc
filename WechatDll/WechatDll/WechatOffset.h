#pragma once

#define ISLOGIN 0x161C964  //是否登录标识，0-未登录|1-已登录  (WX_USER_SEX - 0x10)

/* 微信登录账号详情的偏移值 */
#define WX_USER_ID 0x161C814  // 微信ID
#define WX_USER_USERNAME 0x161C9F0  // 微信账号
#define WX_USER_USERNAME_OLD 0x161C814  // 旧微信账号
#define WX_USER_NICKNAME 0x161C88C  // 昵称
#define WX_USER_DEVICE 0x161CCC8  // 设备
#define WX_USER_MOBILE 0x161C8C0  // 手机号码
#define WX_USER_EMAIL 0x161C8A8   // 邮箱
#define WX_USER_SEX 0x161C974  // 性别
#define WX_USER_NATIONALITY 0x161CA68  // 国籍
#define WX_USER_PROVINCE 0x161C978   // 省份
#define WX_USER_CITY 0x161C990  // 城市
#define WX_USER_SIGN 0x161C9A8  // 个性签名
#define WX_USER_HEAD_URL 0x161CB54  // 头像  内存里面的地址，连续两个，取第一个

/* 微信基础偏移值 */
#define WX_QRCODE_URL_CODE 0x1635C58  //二维码URL值
#define GOTO_QRCODE_1 0x210280  //跳转到二维码
#define GOTO_QRCODE_2 0x321660  //跳转到二维码
#define LOGOUT 0x440020  //退出微信

#define SEND_MESSAGE_CALL 0x329BF0  //发送消息 - call同发送xml名片


#define WX_SEND_XML_CARD 0x329BF0	//发送xml名片

#define WX_SEND_XML_CALL1 0x25E2C0	//发送xml
#define WX_SEND_XML_CALL2 0x70840	//发送xml
#define WX_SEND_XML_CALL3 0x4CB0B0	//发送xml
#define WX_SEND_XML_CALL4 0x4CFA0	//发送xml
#define WX_SEND_XML_CALL5 0x4CFA60	//发送xml
#define WX_SEND_XML_CALL6 0x4B4DC0	//发送xml
#define WX_SEND_XML_PARAM 0x16297A4	//发送xml
#define WX_SEND_XML_CALL7 0x25E4D0	//发送xml


#define WX_SEND_ATTACH_PARAM 0x1368908	//发送附件参数
#define WX_SEND_ATTACH_CALL1 0x4DEDB0	//发送附件
#define WX_SEND_ATTACH_CALL2 0x4DEDF0	//发送附件
#define WX_SEND_ATTACH_CALL3 0x546E0	//发送附件
#define WX_SEND_ATTACH_CALL4 0x25E5C0	//发送附件

#define WX_SEND_IMAGE_CALL1 0x4E7E0			//发送图片消息
#define WX_SEND_IMAGE_CALL2 0x3295E0		//发送图片消息
#define WX_SEND_IMAGE_PATCH_ADDR 0x4D94E0	//发送图片消息-文件解锁

#define TRANSFER_CALL1 0x7AA1F0  //收款CALL1
#define TRANSFER_CALL2 0x7AA270  //收款CALL2

#define LISTEN_MESSAGE_HOOK 0x354AA3  // 接收消息
#define LISTEN_MESSAGE_CALL 0x1633C98  // 接收消息

#define LISTEN_FRIENDS_HOOK 0x479F28  // 获取好友数据
#define LISTEN_FRIENDS_CALL 0x4DF410   // 获取好友数据

#define WX_GET_USER_INFO_CALL1 0x4DEDF0	//获取好友详情
#define WX_GET_USER_INFO_CALL2 0x4E720	//获取好友详情
#define WX_GET_USER_INFO_CALL3 0x2ACA80	//获取好友详情

#define WX_SET_USER_REMARK_CALL 0x2ADE30	//设置好友备注
#define WX_SET_USER_REMARK_PARAM 0x13BF93C	//设置好友备注

#define WX_VERIFY_USER_PARAM 0x161CEE8	    //同意加好友请求
#define WX_VERIFY_USER_CALL1 0x1A5050		//同意加好友请求
#define WX_VERIFY_USER_CALL2 0x485C0		//同意加好友请求
#define WX_VERIFY_USER_CALL3 0xE5AC0		//同意加好友请求
#define WX_VERIFY_USER_CALL4 0x1899D0		//同意加好友请求

#define WX_ADD_USER_PARAM 0x1369C94	    //通过wxid添加好友
#define WX_ADD_USER_CALL1 0x52550		//通过wxid添加好友
#define WX_ADD_USER_CALL2 0x6E910		//通过wxid添加好友
#define WX_ADD_USER_CALL3 0x4DEDB0		//通过wxid添加好友
#define WX_ADD_USER_CALL4 0x4DEDF0		//通过wxid添加好友
#define WX_ADD_USER_CALL5 0x4E720		//通过wxid添加好友
#define WX_ADD_USER_CALL6 0x2AAF70		//通过wxid添加好友

#define WX_ADD_CARD_USER_PARAM 0x1369C94	//通过推荐卡片添加好友
#define WX_ADD_CARD_USER_CALL1 0x52550		//通过推荐卡片添加好友
#define WX_ADD_CARD_USER_CALL2 0x6E910		//通过推荐卡片添加好友
#define WX_ADD_CARD_USER_CALL3 0x4DEDB0 	//通过推荐卡片添加好友
#define WX_ADD_CARD_USER_CALL4 0x4DEDF0	    //通过推荐卡片添加好友
#define WX_ADD_CARD_USER_CALL5 0x2AAF70	    //通过推荐卡片添加好友

#define WX_DELETE_USER 0x2ADF60		//删除好友

#define WX_GET_ROOM_USER_WXID_CALL1 0x4681E0	//获取群成员微信ID
#define WX_GET_ROOM_USER_WXID_CALL2 0x2F8530	//获取群成员微信ID
#define WX_GET_ROOM_USER_WXID_CALL3 0x46EB00	//获取群成员微信ID
#define WX_GET_ROOM_USER_WXID_CALL4 0x468CD0	//获取群成员微信ID

#define WX_CREATE_ROOM_PARAM 0x1633D20	//创建群聊
#define WX_CREATE_ROOM_CALL 0x299D10	//创建群聊
//#define WxDatabaseKey 0x275B1A
#define WX_ADD_ROOM_MEMBER_CALL1 0x6C2F0	//添加群成员
#define WX_ADD_ROOM_MEMBER_CALL2 0x4DEF00	//添加群成员
#define WX_ADD_ROOM_MEMBER_CALL3 0x7C0C0	//添加群成员
#define WX_ADD_ROOM_MEMBER_CALL4 0x29A0D0	//添加群成员
#define WX_ADD_ROOM_MEMBER_PARAM1 0x161C814	//添加群成员
#define WX_ADD_ROOM_MEMBER_PARAM2 0x1633D20	//添加群成员

#define WX_SET_ROOM_ANNOUNCEMENT 0x2A2240			//发送群公告

#define WX_SET_ROOM_NAME 0x29E9F0		//修改群名称

#define WX_QUIT_ROOM 0x29F520		//退出群聊

#define WX_DEL_ROOM_USER_CALL1 0x4DEDF0	  //删除群成员
#define WX_DEL_ROOM_USER_CALL2 0x66340	  //删除群成员
#define WX_DEL_ROOM_USER_CALL3 0x29A2F0	  //删除群成员

