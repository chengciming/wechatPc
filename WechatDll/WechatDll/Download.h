#pragma once

// 保存图片文件 - 保存路径结构
struct DownloadImageSavePathStruct
{
	wchar_t *path;
	int pathLen;
	int maxPathLen;
	char fill[0xC] = { 0 };
};
// 保存图片文件 - 消息结构
struct DownloadImageMessageStruct
{
	wchar_t *path;
	int pathLen;
	int maxPathLen;
	char fill1[0x8] = { 0 };
	wchar_t *path2;
	int pathLen2;
	int maxPathLen2;
	char fill2[0x50] = { 0 };
	wchar_t *wxid;
	int wxidLen;
	int maxWxidLen;
	char fill3[0x1C] = { 0 };
	wchar_t *contentXml;
	int contentXmlLen;
	int maxContentXmlLen;
	char fill4[0xB8] = { 0 };
	wchar_t *msgId;
	int msgIdLen;
	int maxMsgIdLen;
	int buff = 0x0;
	int buff2 = 0x0;
	wchar_t *encodePath1;
	int encodePathLen1;
	int maxEncodePathLen1;
	int buff3 = 0x0;
	int buff4 = 0x0;
	wchar_t *encodePath2;
	int encodePathLen2;
	int maxEncodePathLen2;
	int buff5 = 0x0;
	int buff6 = 0x0;
	int buff7 = 0x0;
	wchar_t *msgsourceXml;
	int msgsourceXmlLen;
	int maxMsgsourceXmlLen;
	char fill6[0x1000] = { 0 };
};



// 解密图片文件
void DecodeImage(char *datFile, char *saveFile);
// 下载图片
void DownloadImage();