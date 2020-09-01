#include "stdafx.h"
#include "resource.h"
#include "Config.h"
#include "WechatOffset.h"
#include "Common.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <tchar.h> 
#include <sstream>
#include <iomanip>
#include <strstream>
#include <shlobj.h>                                                                                                                                
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "Download.h"

#pragma comment(lib, "shell32.lib")

using namespace std;

// 解密图片文件
void DecodeImage(char *datFile, char *saveFile)
{
	//CHAR documentDir[MAX_PATH] = {0};
	//SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, documentDir);
	// 加密文件路径
	//char datFile[] = "C:\\Users\\Lemonice\\Desktop\\c14d0257b67d5291bef56853ee3e3c6d.dat";
	// 保存的文件路径
	//char saveFile[] = "C:\\Users\\Lemonice\\Desktop\\c14d0257b67d5291bef56853ee3e3c6d.jpg";

	// 打开加密文件
	ifstream is(datFile, ifstream::in);
	// 计算文件长度
	is.seekg(0, is.end);
	int length = (int)is.tellg();
	is.seekg(0, is.beg);
	// 创建内存缓存区
	char * buffer = new char[length];
	// 读取加密内容
	is.read(buffer, length);
	
	// 判断文件头 - JPG
	char password = buffer[0] ^ 0xFF;
	char check1 = buffer[0] ^ password;
	char check2 = buffer[1] ^ password;
	char h1 = 0xFF;
	char h2 = 0xD8;
	if (check1 != h1 || check2 != h2) {
		// 判断文件头 - PNG
		password = buffer[0] ^ 0x89;
		check1 = buffer[0] ^ password;
		check2 = buffer[1] ^ password;
		h1 = 0x89;
		h2 = 0x50;
		if (check1 != h1 || check2 != h2) {
			// 判断文件头 - GIF
			password = buffer[0] ^ 0x47;
			check1 = buffer[0] ^ password;
			check2 = buffer[1] ^ password;
			h1 = 0x47;
			h2 = 0x49;
			if (check1 != h1 || check2 != h2) {
				password = NULL;
			}
		}
	}
	// 找不到类型
	if (password == NULL) {
		MessageBoxA(NULL, "找不到文件类型", "", NULL);
		is.close();
		return;
	}

	// 解密过程
	int i = 0;
	while (i < length) {
		buffer[i] ^= password;
		i++;
	}

	// 写入到保存的文件路径
	FILE *fp = NULL;
	fopen_s(&fp, saveFile, "wb");
	fwrite(buffer, length, 1, fp);
	fclose(fp);
	
	delete[] buffer;
	fclose(fp);
	is.close();
}

// 下载图片
void DownloadImage()
{
	// 获取微信基址
	DWORD winAddress = GetWechatWinAddress();

	DWORD dwCallAddr = winAddress + 0x4B5C30;
	DWORD dwParamAddr1 = winAddress + 0x1380C64;  // ebx\edi
	DWORD dwParamAddr2 = winAddress + 0x137ED90;  // esi

	wchar_t wxid[] = L"Lemonice-cheng";
	wchar_t msgId[] = L"864a39b9836477fd76b9526e6d767495";
	wchar_t savePath[] = L"C:/Users/Lemonice/Desktop/MM_WeChat_Image.dat";

	wchar_t imagePath[] = L"C:\\Users\\Lemonice\\Documents\\WeChat Files\\supper-busy\\FileStorage\\Image\\2020-03\\0adce5d4785780dee857def881d279bd.dat";

	wchar_t imagePathThumb[] = L"C:\\Users\\Lemonice\\Documents\\WeChat Files\\supper-busy\\FileStorage\\Image\\Thumb\\2020-03\\0adce5d4785780dee857def881d279bd_t.dat";
		
	wchar_t encodePath1[] = L"supper-busy\\FileStorage\\Image\\Thumb\\2020-03\\0adce5d4785780dee857def881d279bd_t.dat";
	wchar_t encodePath2[] = L"C:\\Users\\Lemonice\\Documents\\WeChat Files\\supper-busy\\FileStorage\\Image\\2020-03\\0adce5d4785780dee857def881d279bd.dat";

	wchar_t contentXml[] = L"<?xml version=\"1.0\"?><msg><img aeskey=\"f835b8acd6e330d1edfc1c50a8847cbb\" encryver=\"0\" cdnthumbaeskey=\"f835b8acd6e330d1edfc1c50a8847cbb\" cdnthumburl=\"3053020100044c304a02010002040ba253b702032df7f9020455e7e16a02045e5be1300425617570696d675f353362383066366139306631393334345f313538333037393732373636310204010838010201000400\" cdnthumblength=\"2878\" cdnthumbheight=\"120\" cdnthumbwidth=\"68\" cdnmidheight=\"0\" cdnmidwidth=\"0\" cdnhdheight=\"0\" cdnhdwidth=\"0\" cdnmidimgurl=\"3053020100044c304a02010002040ba253b702032df7f9020455e7e16a02045e5be1300425617570696d675f353362383066366139306631393334345f313538333037393732373636310204010838010201000400\" length=\"35397\" cdnbigimgurl=\"3053020100044c304a02010002040ba253b702032df7f9020455e7e16a02045e5be1300425617570696d675f353362383066366139306631393334345f313538333037393732373636310204010838010201000400\" hdlength=\"778229\" md5=\"0e01a7226bd06ce54aea4d448bb208b3\" /></msg>";
	wchar_t msgsourceXml[] = L"<msgsource><sec_msg_node><uuid>777bcac044e7e71d45347245018d91b1_</uuid></sec_msg_node></msgsource>";
	
	DWORD buffer[0x1EC0] = {0};

	buffer[0] = (DWORD)imagePath;
	buffer[1] = (DWORD)::wcslen(imagePath);
	buffer[2] = (DWORD)ARRAYSIZE(imagePath);

	buffer[5] = (DWORD)imagePathThumb;
	buffer[6] = (DWORD)::wcslen(imagePathThumb);
	buffer[7] = (DWORD)ARRAYSIZE(imagePathThumb);

	//buffer[10] = 0x1;
	//buffer[18] = 0x263;
	//buffer[20] = 0x1;

	buffer[28] = (DWORD)wxid;
	buffer[29] = (DWORD)::wcslen(wxid);
	buffer[30] = (DWORD)ARRAYSIZE(wxid);

	buffer[38] = (DWORD)contentXml;
	buffer[39] = (DWORD)::wcslen(contentXml);
	buffer[40] = (DWORD)ARRAYSIZE(contentXml);

	buffer[64] = 0x1;

	buffer[87] = (DWORD)msgId;
	buffer[88] = (DWORD)::wcslen(msgId);
	buffer[89] = (DWORD)ARRAYSIZE(msgId);
	
	buffer[92] = (DWORD)encodePath1;
	buffer[93] = (DWORD)::wcslen(encodePath1);
	buffer[94] = (DWORD)ARRAYSIZE(encodePath1);

	buffer[97] = (DWORD)encodePath2;
	buffer[98] = (DWORD)::wcslen(encodePath2);
	buffer[99] = (DWORD)ARRAYSIZE(encodePath2);

	buffer[103] = (DWORD)msgsourceXml;
	buffer[104] = (DWORD)::wcslen(msgsourceXml);
	buffer[105] = (DWORD)ARRAYSIZE(msgsourceXml);
	
	//buffer[124] = 0x1;
	//buffer[125] = 0xFF;
	//buffer[126] = 0x1;
	//buffer[128] = 0x263;
	//buffer[131] = 0x1;

	DWORD *asmP1 = (DWORD*)dwParamAddr1;
	DWORD *asmP2 = (DWORD*)dwParamAddr2;

	DownloadImageSavePathStruct pathStruct = { 0 };  // eax
	pathStruct.path = savePath;
	pathStruct.pathLen = wcslen(savePath);
	pathStruct.maxPathLen = wcslen(savePath) * 2;
	char *pPath = (char *)&pathStruct.path;  // esp

	char buff[0x1000] = {0};
	//char *pPath = (char *)&buff;

	/*
	DownloadImageSavePathStruct pathStruct = {0};
	pathStruct.path = savePath;
	pathStruct.pathLen = wcslen(savePath);
	pathStruct.maxPathLen = wcslen(savePath) * 2;

	DownloadImageMessageStruct msgStruct = {0};

	msgStruct.wxid = wxid;
	msgStruct.wxidLen = wcslen(wxid);
	msgStruct.maxWxidLen = wcslen(wxid) * 2;

	msgStruct.path = savePath;
	msgStruct.pathLen = wcslen(savePath);
	msgStruct.maxPathLen = wcslen(savePath) * 2;
	msgStruct.path2 = savePath;
	msgStruct.pathLen2 = wcslen(savePath);
	msgStruct.maxPathLen2 = wcslen(savePath) * 2;

	msgStruct.contentXml = contentXml;
	msgStruct.contentXmlLen = wcslen(contentXml);
	msgStruct.maxContentXmlLen = wcslen(contentXml) * 2;

	msgStruct.msgId = msgId;
	msgStruct.msgIdLen = wcslen(msgId);
	msgStruct.maxMsgIdLen = wcslen(msgId) * 2;

	msgStruct.encodePath1 = encodePath1;
	msgStruct.encodePathLen1 = wcslen(encodePath1);
	msgStruct.maxEncodePathLen1 = wcslen(encodePath1) * 2;

	msgStruct.encodePath2 = encodePath2;
	msgStruct.encodePathLen2 = wcslen(encodePath2);
	msgStruct.maxEncodePathLen2 = wcslen(encodePath2) * 2;

	msgStruct.msgsourceXml = msgsourceXml;
	msgStruct.msgsourceXmlLen = wcslen(msgsourceXml);
	msgStruct.maxMsgsourceXmlLen = wcslen(msgsourceXml) * 2;

	char *pPath = (char *)&pathStruct.path;
	char *pMsg = (char *)&msgStruct.path;
	char *pSavePathChar = (char *)&savePathChar;
	DWORD *asmP1 = (DWORD*)dwParamAddr1;
	DWORD *asmP2 = (DWORD*)dwParamAddr2;
	*/

	__asm {
		pushad

		lea ebx, dwParamAddr1
		lea edi, dwParamAddr1
		lea esi, dwParamAddr2

		lea esp, pathStruct
		lea eax, pathStruct
		lea edx, dword ptr buffer
		lea ecx, dword ptr buff  // ecx有问题，未解决
		call dwCallAddr;

		popad
	}
}