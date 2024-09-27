#pragma once

#include <vector>
#include <string>
#include <Windows.h>

class Server
{
public:
	Server();		//Default server constructor
	
	/// <summary>
	/// Create a server to a specified port
	/// </summary>
	/// <param name="port"> Por you want the server to be on</param>
	void CreateBindListen(const char* port);		

	/// <summary>
	/// Call the function Poll() to handle the event on the server
	/// </summary>
	void pollCall();

	~Server();		//Default destructor for the server 
private:
	/// <summary>
	/// Add a socket to the pfds vector 
	/// </summary>
	/// <param name="toAdd"> socket you want to add</param>
	void addPfds(uint64_t toAdd);

	/// <summary>
	/// Remove a socket from the pfds vector
	/// </summary>
	/// <param name="index"> where the socket is in the array</param>
	void removefds(int index);

	/// <summary>
	/// Add a userName to the name array in the server
	/// </summary>
	/// <param name="name"> : name you want to add</param>
	void addUserName(std::string name);

	/// <summary>
	/// Delete a userNAme from the array of name
	/// </summary>
	/// <param name="index">: where the name is in the array</param>
	void deleteUserName(int index);

	/// <summary>
	/// Remove a user from the server (remove the name from the array and shutdown the socket)
	/// </summary>
	/// <param name="index"> : socket index of the user in the array</param>
	void removeUser(int index);

	/// <summary>
	/// Handle a new connection on the server correctly
	/// </summary>
	/// <param name="addrlen">length of the address</param>
	/// <param name="newfd">Socket for te new connection</param>
	/// <param name="choice">6 if it's an ipv6 and 4 if it's and ipv4</param>
	void newConnection(int* addrlen, uint64_t* newfd, int choice);

	/// <summary>
	/// Handle the receive of a message correctly
	/// </summary>
	/// <param name="newfd">Socket that send us a message</param>
	/// <param name="nameBuffer">buffer that'ss contain the message</param>
	/// <param name="bufSize">siz of the buffer</param>
	/// <returns> return true if the server have to close</returns>
	bool receiveMessage(uint64_t* newfd, char* nameBuffer, int bufSize);

	/// <summary>
	/// Send a message to all the clients in the server (execpt the sender)
	/// </summary>
	/// <param name="sender">Socket of the sender</param>
	/// <param name="msg">message that'll be send</param>
	void sendMessagetoServ(uint64_t* sender, std::string msg);

	/// <summary>
	/// Send a message where there's a new connection
	/// </summary>
	/// <param name="newfd">new connection socket</param>
	/// <param name="nameBuffer">buffer that'll stock the name of the new connection</param>
	void newConnectionMsg(uint64_t* newfd, char* nameBuffer);

	/// <summary>
	/// disconnect all the clients and close all the sockets
	/// </summary>
	/// <param name="buf">buffer that contains the message to check if the server has to close</param>
	/// <returns> true if the server has to close</returns>
	bool clearServer(char* buf);

	uint64_t ListenSocket4;
	uint64_t ListenSocket6;
	std::vector<struct pollfd> fds;
	std::vector<std::string> names;
};