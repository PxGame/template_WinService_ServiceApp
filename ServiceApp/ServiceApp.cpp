#include "ServiceApp.h"

//����״̬�ṹ
SERVICE_STATUS serStatus;

//��ǰ����״̬�ṹ���
SERVICE_STATUS_HANDLE serStatusH;

//����ֹͣ�¼����
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

	//�����¼�
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
	//��ʼ����ز���
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

	//���÷���ʼ����״̬
	SetServiceStatus(SERVICE_RUNNING, NO_ERROR, 3000);

	//ֱ�����յ�ֹͣ�ź�Ϊֹ
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
		//����ֹͣ״̬
		SetServiceStatus(SERVICE_STOP_PENDING, NO_ERROR, 3000);
		//����ֹͣ�ź�
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

	//����״̬
	serStatus.dwCurrentState = dwCurrentState;				//�Ƿ����
	serStatus.dwWin32ExitCode = dwWin32ExitCode;		//����ֵ
	serStatus.dwWaitHint = dwWaitHint;									//�ȴ�ʱ��

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