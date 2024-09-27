#include "errorHandler.h"
#include "client.h"
#include <iostream>

client::client()
{
	WSADATA Widta;
	int resultCheck = WSAStartup(MAKEWORD(2, 2), &Widta);
	if (resultCheck)
		errorHandler::reportWindowsError(TEXT("WSAStartup"), resultCheck);

    this->data = INVALID_SOCKET;
    this->read_event = WSACreateEvent();
}

std::string client::CreateAndConnect(const char* port)
{
	std::string name = "";
	std::string ipAdress = "";

	errorHandler::consolPrint("Enter a name : ");
	std::cin >> name;

	errorHandler::consolPrint("Enter an IP Adress : ");
	std::cin >> ipAdress;

	struct addrinfo* res = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int resultCheck = getaddrinfo(ipAdress.c_str(), port, &hints, &res);
	if(resultCheck != 0)
		errorHandler::reportWindowsError("getaddrinfo ERR: \n", WSAGetLastError());

	while (resultCheck != 0)
	{
		errorHandler::consolPrint("Enter an IP Adress : ");
		std::cin >> ipAdress;

		resultCheck = getaddrinfo(ipAdress.c_str(), port, &hints, &res);

	if (resultCheck != 0)
			errorHandler::reportWindowsError("getaddrinfo ERR: \n", WSAGetLastError());
	}

	this->data = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	if (this->data == INVALID_SOCKET)
	{
		errorHandler::reportWindowsError("Error at socket(): \n", WSAGetLastError());
		resultCheck = -1;
	}
	else
		resultCheck = 0;

	if (connect(this->data, res->ai_addr, (int)res->ai_addrlen) < 0)
	{
		errorHandler::reportWindowsError("UNABLE TO CONNECT\n", WSAGetLastError());
		resultCheck = -1;
	}
	else
	{
		errorHandler::consolPrint("SUCCESSFULLY CONNECTED\n");
		resultCheck = 0;
	}


	while (resultCheck != 0)
	{
		errorHandler::consolPrint("Enter an IP Adress : ");
		std::cin >> ipAdress;

		resultCheck = getaddrinfo(ipAdress.c_str(), port, &hints, &res);
	this->data = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

		if (this->data == INVALID_SOCKET) 
		{
		errorHandler::reportWindowsError("Error at socket(): \n", WSAGetLastError());
			resultCheck = -1;
	}
		else
			resultCheck = 0;

	if (connect(this->data, res->ai_addr, (int)res->ai_addrlen) < 0)
		{
		errorHandler::reportWindowsError("UNABLE TO CONNECT\n", WSAGetLastError());
			resultCheck = -1;
		}
	else
		{
		errorHandler::consolPrint("SUCCESSFULLY CONNECTED\n");
			resultCheck = 0;
		}

	}


	this->sendMessaage(name.c_str() + '\0', (int)name.length() + 1);

	freeaddrinfo(res);

	return name;
}

HANDLE client::createReadEvent()
{
	WSAEventSelect(this->data, this->read_event, FD_READ | FD_CLOSE);
	return this->read_event;
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
