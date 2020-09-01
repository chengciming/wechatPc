#include <Windows.h>
#include <string>
#include <list>
#include "struct.h"
using namespace std;

//数据库句柄
struct DbHandle
{
	int handler;
	char path[0x1000];
};

typedef int(*sqlite3_callback)(void*, int, char**, char**);

typedef int(WINAPI* Sqlite3_exec)(
	DWORD,
	const char*,
	sqlite3_callback,
	void*,
	char**
);
// 监听微信sqlite句柄
void ListenDbHandle();
// 执行SQL
int runSql(string dbName, string sqlStr, sqlite3_callback callBack);
// 获取所有数据库句柄
list<DbHandle> getDbHandleList();
// 清楚所有数据库句柄
VOID clearDbHandleList();
