#pragma once
#include "OpCode.h"
#include "PJson.h"
#include "LoginInfo.h"
#include "Message.h"
#include "Package.h"

using namespace rapidjson;

// 好友列表
typedef struct FriendList
{
	CHAR wxid[0x100] = { 0 };
	CHAR username[0x100] = { 0 };
	int sex = 0;  // 性别：1=男，2=女
	CHAR v1[0x256] = { 0 };
	CHAR v2[0x256] = { 0 };
	int type = 0;
	CHAR nickname[0x100] = { 0 };
	int roomType = 0;
	CHAR remark[0x128] = { 0 };
	CHAR bigHeadImgUrl[0x512] = { 0 };
	CHAR smallHeadImgUrl[0x512] = { 0 };
	CHAR roomWxidList[0x3A98] = { 0 };  // 30 x 500 = 30长度 x 500个群成员ID
	int roomOwner = 0;
	CHAR roomAnnouncement[0x512] = { 0 };
	CHAR roomAnnouncementEditor[0x100] = { 0 };
	CHAR roomAnnouncementPublishTime[0x100] = { 0 };
	int roomStatus = 0;
	int bizType = 0;
	int bizAcceptType = 0;
	CHAR bizBrandList[0x3A98] = { 0 };
	int bizBrandFlag = 0;
	CHAR bizBrandIconURL[0x512] = { 0 };
	CHAR bizExtInfo[0x3A98] = { 0 };
	CHAR extraBuf[0x3A98] = { 0 };
	int delFlag = 0;
	int verifyFlag = 0;
	int roomNotify = 0;

	FriendList *next;
} FriendList;
// 登录详情
struct LoginInfo
{
	CHAR wxid[0x100] = { 0 };  // 微信ID
	CHAR username[0x100] = { 0 };  // 微信号
	CHAR nickname[0x100] = { 0 };  // 昵称
	CHAR headUrl[0x256] = { 0 };  // 头像URL地址
	CHAR mobile[0x32] = { 0 };  // 手机号码
	CHAR email[0x32] = { 0 };  // 游戏
	int sex = 0;  // 性别：1=男，2=女，0=未知
	CHAR nation[0x32] = { 0 };  // 国籍
	CHAR province[0x32] = { 0 };  // 省份
	CHAR city[0x32] = { 0 };  // 城市
	CHAR sign[0x32] = { 0 };  // 个性签名
	CHAR device[0x32] = { 0 };  // 登录的手机设备
};

// 发送回服务器端的消息结构体
struct WebsocketMessageStruct
{
	char msgId[0x100] = { 0 };  // 消息ID
	int isOwner = 0;  // 是否自己发的消息：0=否，1=是
	int msgType = 0;  // 消息类型
	int msgSource = 0;  // 消息来源：0=好友消息，1=群消息
	char msgSourceXml[0x2048] = { 0 };  // 群其他消息
	char wxid[0x100] = { 0 };  // 发送人微信ID
	char roomId[0x100] = { 0 };  // 群ID
	char content[0x8092] = { 0 };  // 消息内容
};


class Send
{
public:
	static void SendFriendList(FriendList *list, Package *package = NULL, int page = 1, int pageSize = 0, int total = 0);
	static void SendWxMessage(WebsocketMessageStruct *message, Package *package = NULL);
	static void SendLoginInfo(LoginInfo *info, Package *package = NULL);
	static void SendLoginStatus(int loginStatus, Package *package = NULL);
	static void SendLoginQrcode(int loginStatus, char *qrcode, Package *package = NULL);
};