#pragma once 

class client
{
	client();

	void CreateAndConnect(const char* ipAdress, const char* port);

	void createEvent();
	void sendMessaage(const char* msg, int length);
	HANDLE getEvent();
	~client();

private:
	uint64_t data;
	HANDLE sckt_event;
};