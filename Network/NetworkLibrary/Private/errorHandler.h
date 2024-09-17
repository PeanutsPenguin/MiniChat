#pragma once 

#include <WinSock2.h>

namespace errorHandler 
{
	void reportWindowsError(LPCTSTR context, DWORD error);

	void consolPrint(LPCTSTR format, ...);
}

