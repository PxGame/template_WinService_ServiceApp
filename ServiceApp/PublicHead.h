# ifndef _PUBLICHEAD_H_
# define _PUBLICHEAD_H_

# include <Windows.h>
# include <tchar.h>

# define INFOMATION_SIZE 1024

# define SERVICE_NAME TEXT("MyService")

//�����Ϣ
void OutMsg(LPTSTR pfInfo, ...);

# endif