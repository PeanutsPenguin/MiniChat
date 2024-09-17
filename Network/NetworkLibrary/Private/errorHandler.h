#pragma once 

#include <ws2tcpip.h>

namespace errorHandler 
{
	void reportWindowsError(LPCTSTR context, DWORD error);

	void consolPrint(LPCTSTR format, ...);
}

