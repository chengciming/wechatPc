#pragma once


//用户个人信息结构体
struct Information
{
	wchar_t wxid[0x100] = L"NULL";	//微信ID
	wchar_t account[0x100] = L"NULL";	//自定义账号
	wchar_t nickname[0x200] = L"NULL";	//昵称
	wchar_t device[0x100] = L"NULL";	//登陆设备
	wchar_t phone[0x100] = L"NULL";	//绑定手机号
	wchar_t email[0x200] = L"NULL";	//绑定邮箱
	wchar_t sex[0x8] = L"NULL";	//性别
	wchar_t nation[0x100] = L"NULL";	//国籍
	wchar_t province[0x100] = L"NULL";	//省份
	wchar_t city[0x100] = L"NULL";	//城市
	wchar_t signName[0x1000] = L"NULL";	//签名
	wchar_t header[0x1000] = L"NULL";	//头像
};

//用户消息结构体
struct recieveMsgStruct
{
	int type;
	int isSelf;
	wchar_t* fromWxid;
	wchar_t* senderWxid;
	wchar_t* unkonwStr;
	wchar_t* content;
};

//好友列表结构体
struct friendStruct
{
	wchar_t wxid[0x100] = L"NULL";
	wchar_t account[0x200] = L"NULL";
	wchar_t nickname[0x200] = L"NULL";
	wchar_t head[0x1000] = L"NULL";
	wchar_t v1[0x200] = L"NULL";
	wchar_t reMark[0x200] = L"NULL";
};


