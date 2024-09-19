#include "Network/server.h"


int main()
{
	Server serv = Server();

	serv.CreateBindListen("27015");

	serv.pollCall();
}