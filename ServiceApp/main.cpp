# include "PublicHead.h"
# include "ServiceApp.h"

/*
要是DebugView正常输出消息，必须将其设置成捕获全局为win32。
调试的时候直接附加在进程中，且在ServiceMain中设置断点，其他地方设置未能命中。
*/

int _tmain(int argc, _TCHAR *argv[])
{
	OutMsg(TEXT("SC#_tmain() Start\n"));
	SERVICE_TABLE_ENTRY serTab[] =
	{
		{ SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
		{ NULL, NULL }
	};
	BOOL bRet = StartServiceCtrlDispatcher(serTab);
	if (bRet == FALSE)
	{
		OutMsg(TEXT("SC#_tmain() StartServiceCtrlDispatcher Error = %d\n"), GetLastError());
		return -1;
	}

	OutMsg(TEXT("SC#_tmain() Stop\n"));
	return 0;
}