# include "PublicHead.h"
# include "ServiceApp.h"

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