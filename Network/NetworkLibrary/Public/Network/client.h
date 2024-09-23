#pragma once 

#include <vector>
#include <Windows.h>
#include <sstream>

struct pollfd;

class client
{
public:
	client();

	void CreateAndConnect(const char* ipAdress, const char* port);

	void createEvent();

	void sendMessaage(const char* msg, int length);

	void receivemessage();

	HANDLE getEvent();

	~client();

private:
	uint64_t data;
	HANDLE sckt_event;
};