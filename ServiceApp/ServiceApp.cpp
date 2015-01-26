#include "ServiceApp.h"

//服务状态结构
SERVICE_STATUS serStatus;

//当前服务状态结构句柄
SERVICE_STATUS_HANDLE serStatusH;

//服务停止事件句柄
HANDLE stopEvent;


BOOL InitService()
{
	ZeroMemory(&serStatus, sizeof(serStatus));
	serStatus.dwCheckPoint = 0;
	serStatus.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_STOP;
	serStatus.dwCurrentState = SERVICE_STOPPED;
	serStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	serStatus.dwWaitHint = 0;
	serStatus.dwWin32ExitCode = NO_ERROR;
	serStatus.dwServiceSpecificExitCode = 0;

	//创建事件
	stopEvent = CreateEvent(
		NULL,    // default security attributes  
		TRUE,    // manual reset event  
		FALSE,   // not signaled  
		NULL);   // no name  
	if (stopEvent == NULL)
	{
		OutMsg(TEXT("SC#InitService() CreateEvent Error = %d\n"), GetLastError());
		return FALSE;
	}

	OutMsg(TEXT("SC#InitService() success\n"));
	return TRUE;
}

void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
	//初始化相关参数
	if (!InitService())
	{
		return;
	}
	
	serStatusH = RegisterServiceCtrlHandler(SERVICE_NAME, (LPHANDLER_FUNCTION)Handler);
	if (serStatusH == NULL)
	{
		DWORD errorNum = GetLastError();
		SetServiceStatus(SERVICE_STOPPED, ERROR_SERVICE_SPECIFIC_ERROR, errorNum);
		OutMsg(TEXT("SC#ServiceMain() Error = %d\n"), errorNum);
		return;
	}

	//设置服务开始运行状态
	SetServiceStatus(SERVICE_RUNNING, NO_ERROR, 3000);

	//直到接收到停止信号为止
	while (WaitForSingleObject(stopEvent, 0) != WAIT_OBJECT_0)
	{
		OutMsg(TEXT("SC#Sleep 2s\n"));
		Sleep(2000);
	}

	OutMsg(TEXT("SC#ServiceMain() success\n"));
}

void WINAPI Handler(DWORD fdwControl)
{
	switch (fdwControl)
	{
	case SERVICE_CONTROL_STOP:
		//设置停止状态
		SetServiceStatus(SERVICE_STOP_PENDING, NO_ERROR, 3000);
		//发送停止信号
		SetEvent(stopEvent);
		break;    
	case SERVICE_CONTROL_INTERROGATE:
		// Fall through to send current status.  
		break;
	default:
		break;
	}
	OutMsg(TEXT("SC#Handler() success\n"));
}


BOOL SetServiceStatus(DWORD dwCurrentState,	DWORD dwWin32ExitCode,DWORD dwWaitHint)
{
	if (serStatusH == NULL)
	{
		OutMsg(TEXT("SC#SetServiceStatus() SetServiceStatus Error = serStatusH is NULL\n"));
		return FALSE;
	}

	static DWORD dwCheckPoint = 1;

	//设置状态
	serStatus.dwCurrentState = dwCurrentState;				//是否错误
	serStatus.dwWin32ExitCode = dwWin32ExitCode;		//错误值
	serStatus.dwWaitHint = dwWaitHint;									//等待时间

	//if (dwCurrentState == SERVICE_START_PENDING)
	//{
	//	serStatus.dwControlsAccepted = 0;
	//}
	//else
	//{
	//	serStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	//}
/*
	if ((dwCurrentState == SERVICE_RUNNING) ||
		(dwCurrentState == SERVICE_STOPPED))
	{
		serStatus.dwCheckPoint = 0;
	}
	else
	{
		serStatus.dwCheckPoint = dwCheckPoint++;
	}
*/
	serStatus.dwCheckPoint = 0;

	BOOL bRet = FALSE;
	bRet = ::SetServiceStatus(serStatusH, &serStatus);
	if (bRet == FALSE)
	{
		OutMsg(TEXT("SC#SetServiceStatus() SetServiceStatus Error = %d\n"), GetLastError());
		return FALSE;
	}

	OutMsg(TEXT("SC#SetServiceStatus() success\n"));
	return TRUE;
}