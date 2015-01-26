#include "WinService.h"

BOOL SerivceInstall()
{
	//�򿪷��������
	SC_HANDLE pSCM = NULL;
	pSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (pSCM == NULL)
	{
		OutMsg(TEXT("SC#SerivceInstall() OpenSCManager Error = %d\n"), GetLastError());
		return FALSE;
	}

	//����ִ���ļ���·��
	TCHAR tsFilePath[MAX_PATH];
	ZeroMemory(tsFilePath, MAX_PATH);
	GetCurrentDirectory(MAX_PATH, tsFilePath);
	_tcscat_s<MAX_PATH>(tsFilePath, TEXT("\\ServiceApp32.exe"));
	//_tcscpy_s	<MAX_PATH>(tsFilePath, TEXT("\\ServiceApp.exe"));

	//��������
	SC_HANDLE pService = NULL;
	pService = CreateService(
		pSCM,
		SERVICE_NAME,
		SERVICE_NAME,
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
		SERVICE_AUTO_START,
		SERVICE_ERROR_NORMAL,
		tsFilePath,
		NULL, NULL, NULL, NULL, NULL);
	if (pService == NULL)
	{
		OutMsg(TEXT("SC#SerivceInstall() CreateService Error = %d\n"), GetLastError());
		CloseServiceHandle(pSCM);
		return FALSE;
	}

	//��ѯ����״̬
	SERVICE_STATUS serStatus;
	ZeroMemory(&serStatus, sizeof(serStatus));
	BOOL bRet = QueryServiceStatus(pService, &serStatus);
	if (bRet == FALSE)
	{
		OutMsg(TEXT("SC#SerivceInstall() QueryServiceStatus Error = %d\n"), GetLastError());
		CloseServiceHandle(pService);
		CloseServiceHandle(pSCM);
		return FALSE;
	}

	//�жϷ����Ƿ�����
	if (serStatus.dwCurrentState != SERVICE_RUNNING)
	{
		//���з���
		bRet = StartService(pService, 0, NULL);
		if (bRet == FALSE)
		{
			OutMsg(TEXT("SC#SerivceInstall() StartService Error = %d\n"), GetLastError());
			CloseServiceHandle(pService);
			CloseServiceHandle(pSCM);
			return FALSE;
		}
	}

	OutMsg(TEXT("SC#SerivceInstall() is Install and Start success!\n"));
	CloseServiceHandle(pService);
	CloseServiceHandle(pSCM);
	return TRUE;
}

BOOL SerivceUnInstall()
{
	//�򿪷��������
	SC_HANDLE pSCM = NULL;
	pSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (pSCM == NULL)
	{
		OutMsg(TEXT("SC#SerivceUnInstall() OpenSCManager Error = %d\n"), GetLastError());
		return FALSE;
	}

	//�򿪷���
	SC_HANDLE pService = NULL;
	pService = OpenService(pSCM, SERVICE_NAME, SERVICE_STOP | DELETE);
	if (pService == NULL)
	{
		OutMsg(TEXT("SC#SerivceUnInstall() OpenService Error = %d\n"), GetLastError());
		CloseServiceHandle(pSCM);
		return FALSE;
	}

	//����ֹͣ��Ϣ
	SERVICE_STATUS serStatus;
	ZeroMemory(&serStatus, sizeof(serStatus));
	BOOL bRet = FALSE;
	bRet = ControlService(pService, SERVICE_CONTROL_STOP, &serStatus);
	if (bRet == FALSE)
	{
		OutMsg(TEXT("SC#SerivceUnInstall() ControlService Error = %d\n"), GetLastError());
		CloseServiceHandle(pService);
		CloseServiceHandle(pSCM);
		return FALSE;
	}

	//ɾ������
	bRet = DeleteService(pService);
	if (bRet == FALSE)
	{
		OutMsg(TEXT("SC#SerivceUnInstall() DeleteService Error = %d\n"), GetLastError());
		CloseServiceHandle(pService);
		CloseServiceHandle(pSCM);
		return FALSE;
	}

	OutMsg(TEXT("SC#SerivceUnInstall() is UnInstall and Stop success!\n"));
	CloseServiceHandle(pService);
	CloseServiceHandle(pSCM);
	return TRUE;
}