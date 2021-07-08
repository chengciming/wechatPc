#include "stdafx.h"
#include "stdio.h"
#include "Common.h"

DWORD winAddress = GetWechatWinAddress();

VOID readData() 
{
	CHAR wxVersion[0x100] = { 0 };
	sprintf_s(wxVersion, "%s", winAddress + 0x161DA78);

}
