#pragma once 

#include "Network/network.h"

class chatServer
{
	public:
		chatServer();

		chatServer(int majorVersion, int minorVersion);

		~chatServer();

	private:
		Server server;

};