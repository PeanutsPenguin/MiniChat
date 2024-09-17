#include "sckt.h"

my_socket::my_socket() 
{
	this->data = new addrinfo();

	struct addrinfo* hints;	

	ZeroMemory(&hints, sizeof(hints));
	hints->ai_family= AF_INET;
	hints->ai_socktype = SOCK_STREAM;
	hints->ai_protocol = IPPROTO_TCP;
	hints->ai_flags = AI_PASSIVE;

	int resultCheck = getaddrinfo(NULL, "27015", hints, &this->data);
	
	if (resultCheck != 0) 
		errorHandler::consolPrint(TEXT("getaddrinfo failed: %d\n"), resultCheck);
}

my_socket::my_socket(int family, int type, int protocol, int flag, const char* port)
{
	this->data = new addrinfo();

	struct addrinfo* hints;

	ZeroMemory(&hints, sizeof(hints));
	hints->ai_family = family;
	hints->ai_socktype = type;
	hints->ai_protocol = protocol;
	hints->ai_flags = flag;

	int resultCheck = getaddrinfo(NULL, port, hints, &this->data);

	if (resultCheck != 0)
		errorHandler::consolPrint(TEXT("getaddrinfo failed: %d\n"), resultCheck);
}

my_socket::~my_socket()
{
	freeaddrinfo(this->data);
}
