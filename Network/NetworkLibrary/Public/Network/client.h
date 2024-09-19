#pragma once 

class client
{
	client();

	void CreateAndConnect(const char* ipAdress, const char* port);

	~client();

private:
	UINT64 sckt;
};