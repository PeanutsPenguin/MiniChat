#pragma once 

class client
{
	client();

	void CreateAndConnect(const char* ipAdress, const char* port);

	void createEvent();
	HANDLE getEvent();
	~client();

private:
	UINT64 sckt;
};