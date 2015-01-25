# include "PublicHead.h"
# include "WinService.h"

int _tmain(int argc, TCHAR *argv[])
{
	if (argc == 2)
	{
		if (_tcscmp(TEXT("install"), argv[1]) == 0)
		{
			SerivceInstall();
		}
		else if (_tcscmp(TEXT("uninstall"), argv[1]) == 0)
		{
			SerivceUnInstall();
		}
	}
	return 0;
}