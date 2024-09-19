#include "errorHandler.h"
#include "client.h"

client::client()
{
	WSADATA data;
	int resultCheck = WSAStartup(MAKEWORD(2, 2), &data);
	if (resultCheck)
		errorHandler::reportWindowsError(TEXT("WSAStartup"), resultCheck);

	this->sckt = INVALID_SOCKET;
}

void client::CreateAndConnect(const char* ipAdress, const char* port)
{
	struct addrinfo* res = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	int resultCheck = getaddrinfo(ipAdress, port, &hints, &res);

	if (resultCheck != 0)
		errorHandler::reportWindowsError("getaddrinfo failed: %d\n", resultCheck);

	this->sckt = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	if (this->sckt == INVALID_SOCKET) {
		errorHandler::reportWindowsError("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(res);
	}

	connect(this->sckt, res->ai_addr, (int)res->ai_addrlen);

	freeaddrinfo(res);
}

client::~client()
{
	closesocket(this->sckt);
	WSACleanup();
}
