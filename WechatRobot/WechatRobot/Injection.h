#pragma once

#include "Config.h"
// 获取进程句柄
#include <TlHelp32.h>


// DLL注入
INT_PTR InjectDll(DWORD pid);

// 检查是否已经注入
INT_PTR CheckInject(DWORD dwProcessid);

// 卸载DLL注入
INT_PTR UnjectDll();

