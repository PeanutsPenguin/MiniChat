#include <Network/Network.h>
#include "chatServer.h"

chatServer::chatServer()
{
	this->server = Server();

	this->server.Start(2, 2);
}

chatServer::chatServer(int majorVersion, int minorVersion)
{
	this->server = Server();

	this->server.Start(minorVersion, majorVersion);
}

chatServer::~chatServer()
{
	this->server.Close();
}
