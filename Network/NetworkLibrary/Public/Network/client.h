#pragma once 

#include <vector>
#include <Windows.h>
#include <sstream>

struct pollfd;

class client
{
public:
	client();		//Client default constructor
	
	/// <summary>
	/// Create a socket with an adress and a name requested
	/// </summary>
	/// <param name="port"> :current server port</param>
	/// <returns>your userName</returns>
	std::string CreateAndConnect(const char* port);

	/// <summary>
	/// Create a read event to know when the client is ready to receive a messgae
	/// </summary>
	/// <returns>the HANDLE type of the event</returns>
	HANDLE createReadEvent();

	/// <summary>
	/// Send a message to the connected server
	/// </summary>
	/// <param name="msg">Buffer that contains your message</param>
	/// <param name="length">length of the buffer</param>
	void sendMessaage(const char* msg, int length);
	
	/// <summary>
	/// Handle the reception of a message corerctly
	/// </summary>
	/// <returns>Return a boolean to know if the client have to close</returns>
	bool receivemessage();

	/// <summary>
	/// Getter for the read event
	/// </summary>
	/// <returns>the read event</returns>
	HANDLE getReadEvent();

	~client();		//Default destructor

private:
	uint64_t data;		//Socket of the client
	HANDLE read_event;		//Read event for the client
};