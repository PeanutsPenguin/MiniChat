#pragma once 

#include "errorHandler.h"

class my_socket
{
	my_socket();

	my_socket(int family, int type, int protocol, int flag, const char* port);

	~my_socket();

	private:
		struct addrinfo* data;
};


