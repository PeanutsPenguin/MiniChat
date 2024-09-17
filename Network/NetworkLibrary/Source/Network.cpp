#include "network.h"
#include "errorHandler.h"


void Server::Start(int majorVersion, int minorVersion)
{
	WSADATA data;
	int resultCheck = WSAStartup(MAKEWORD(majorVersion, minorVersion), &data);
	if (resultCheck)
	{
		errorHandler::reportWindowsError(TEXT("WSAStartup error :"), resultCheck);
	}

	if (LOBYTE(data.wVersion) == minorVersion && HIBYTE(data.wVersion) == majorVersion)
		errorHandler::consolPrint(TEXT("Server is open\n"));

	else
		errorHandler::consolPrint(TEXT("Server couldn't open\n"));
}

void Server::Close()
{
	WSACleanup();
}

Server::~Server()
{
	WSACleanup();
}
