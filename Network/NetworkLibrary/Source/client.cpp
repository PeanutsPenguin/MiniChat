#include "errorHandler.h"
#include "client.h"

client::client()
{
	WSADATA Widta;
	int resultCheck = WSAStartup(MAKEWORD(2, 2), &Widta);
	if (resultCheck)
		errorHandler::reportWindowsError(TEXT("WSAStartup"), resultCheck);

    this->data = INVALID_SOCKET;
    this->sckt_event = WSACreateEvent();
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

	this->data = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	if (this->data == INVALID_SOCKET) {
		errorHandler::reportWindowsError("Error at socket(): \n", WSAGetLastError());
		freeaddrinfo(res);
	}

	if (connect(this->data, res->ai_addr, (int)res->ai_addrlen) < 0)
		errorHandler::reportWindowsError("UNABLE TO CONNECT\n", WSAGetLastError());
	else
		errorHandler::consolPrint("SUCCESSFULLY CONNECTED\n");

	freeaddrinfo(res);
}

void client::createEvent()
{
	WSAEventSelect(this->data, this->sckt_event, FD_READ);
}

void client::sendMessaage(const char* msg, int length)
{
    int byteSent = send(this->data, msg, length, 0);

    if (byteSent < 0)
		errorHandler::reportWindowsError("SEND CLIENT ERR :", WSAGetLastError());
}

void client::receivemessage()
{
	char buf[512];

	int byteReceive = recv(this->data, buf, 512, 0);
	
	if (byteReceive <= 0)
		errorHandler::reportWindowsError("RECEIVE CLIENT ERR :", WSAGetLastError());
	else
	{
		buf[byteReceive - (byteReceive == sizeof buf ? 1 : 0)] = '\0';
		errorHandler::consolPrint(buf);
	}
	ResetEvent(this->sckt_event);
	errorHandler::consolPrint("\n");
}

HANDLE client::getEvent()
{
	return this->sckt_event;
}


client::~client()
{
	closesocket(this->data);
	WSACleanup();
}
