# ifndef _SERVICEAPP_H_
# define _SERVICEAPP_H_

# include "PublicHead.h"

//��ʼ������
BOOL InitService();

//����������
void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv);

//�����¼����պ���
void WINAPI Handler(DWORD fdwControl);

//���÷���״̬����
BOOL SetServiceStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint);

# endif