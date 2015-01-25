#include "PublicHead.h"

void OutMsg(LPTSTR pfInfo, ...)
{
	TCHAR szBuffer[INFOMATION_SIZE + 1];
	int nWritten = 0;
	va_list args;
	va_start(args, pfInfo);//获取pfInfo后面的参数列表
	nWritten = _vsntprintf_s(szBuffer, INFOMATION_SIZE, pfInfo, args);
	va_end(args);//释放
# ifdef _DEBUG
	OutputDebugString(szBuffer);
# else
	OutputDebugString(szBuffer);
# endif
}