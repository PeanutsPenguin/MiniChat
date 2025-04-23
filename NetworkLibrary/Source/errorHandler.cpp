#include "errorHandler.h"

void errorHandler::reportWindowsError(LPCTSTR context, DWORD error)
{
	DWORD dwLangId = 0;

#if 1
#if 0
	dwLangId = MAKELANGID(LANG_FRENCH, SUBLANG_BRETON_FRANCE);
#else 
	DWORD temp;
	if (GetLocaleInfoEx(L"Fr-FR", LOCALE_ILANGUAGE | LOCALE_RETURN_NUMBER, (LPWSTR)&temp, 2))
	{
		dwLangId = temp;
	}
#endif
#endif

	LPTSTR buffer;
	DWORD charCount = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER
		| FORMAT_MESSAGE_FROM_SYSTEM
		| FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, error, dwLangId, (LPTSTR)&buffer, 0, nullptr);

	if (charCount)
	{
		consolPrint(TEXT("%1: %2\n"), context, buffer);
	}
	else
	{
		consolPrint(TEXT("%1: error %2!d!\nFormat message error: %3!d!\n"), context, error, GetLastError());
	}

	LocalFree(buffer);
}

void errorHandler::consolPrint(LPCTSTR format, ...)
{
	va_list args;
	va_start(args, format);
	LPTSTR buffer;

	DWORD charCount = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING, format, 0, 0, (LPTSTR)&buffer, 0, &args);

	if (charCount)
	{
		HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
		bool success = WriteConsole(out, buffer, charCount, nullptr, NULL);

		if (success == 0)
		{
			//int error = GetLastError();
			DebugBreak();
			OutputDebugString(TEXT("WriteConsole : error\n"));
		}
	}
	else
	{
		//int error = GetLastError();
		DebugBreak();
		OutputDebugString(TEXT("FormatMessage : error\n"));
	}

	LocalFree(buffer);
}

