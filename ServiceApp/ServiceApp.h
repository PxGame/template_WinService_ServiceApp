# ifndef _SERVICEAPP_H_
# define _SERVICEAPP_H_

# include "PublicHead.h"

//初始化函数
BOOL InitService();

//服务主函数
void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv);

//服务事件接收函数
void WINAPI Handler(DWORD fdwControl);

//设置服务状态函数
BOOL SetServiceStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint);

# endif