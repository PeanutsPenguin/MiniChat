#include "chatServer.h"

constexpr int minorVersion = 2;
constexpr int majorVersion = 2;

int main()
{
	chatServer currentServ = chatServer(majorVersion, minorVersion);

	return 0;
}