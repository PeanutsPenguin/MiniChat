#pragma once

#include <vector>

class Server
{
public:
	Server();

	void CreateBindListen(const char* port);

	void addPfds(uint64_t toAdd);

	void removefds(int index);

	void pollCall();

	~Server();
private:
	uint64_t ListenSocket;
	std::vector<struct pollfd> fds;
};