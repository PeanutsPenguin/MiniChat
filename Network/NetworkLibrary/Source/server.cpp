#include "errorHandler.h"
#include "server.h"

Server::Server()
{
	WSADATA data;
	int resultCheck = WSAStartup(MAKEWORD(2, 2), &data);
	if (resultCheck)
		errorHandler::reportWindowsError(TEXT("WSAStartup"), resultCheck);
	else
		errorHandler::consolPrint("SERVER'S OPEN\n");

	this->ListenSocket4 = INVALID_SOCKET;
	this->ListenSocket6 = INVALID_SOCKET;
}


void Server::CreateBindListen(const char* port)
{
	struct addrinfo* res = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	int resultCheck = getaddrinfo(NULL, port, &hints, &res);

	if (resultCheck != 0)
		errorHandler::reportWindowsError("getaddrinfo failed: %d\n", resultCheck);

	this->ListenSocket4 = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	if (this->ListenSocket4 == INVALID_SOCKET) {
		errorHandler::reportWindowsError("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(res);
	}

	resultCheck = bind(this->ListenSocket4, res->ai_addr, (int)res->ai_addrlen);
	if (resultCheck == SOCKET_ERROR) {
		errorHandler::reportWindowsError("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(res);
		closesocket(this->ListenSocket4);
	}
	
	if (listen(this->ListenSocket4, SOMAXCONN ) == SOCKET_ERROR) {
		errorHandler::reportWindowsError("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(this->ListenSocket4);
	}

	this->addPfds(this->ListenSocket4);

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	resultCheck = getaddrinfo(NULL, port, &hints, &res);

	if (resultCheck != 0)
		errorHandler::reportWindowsError("getaddrinfo failed: %d\n", resultCheck);

	this->ListenSocket6 = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	if (this->ListenSocket6 == INVALID_SOCKET) {
		errorHandler::reportWindowsError("Error at socket(): %ld\n", WSAGetLastError());
	freeaddrinfo(res);
}

	resultCheck = bind(this->ListenSocket6, res->ai_addr, (int)res->ai_addrlen);
	if (resultCheck == SOCKET_ERROR) {
		errorHandler::reportWindowsError("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(res);
		closesocket(this->ListenSocket6);
	}

	if (listen(this->ListenSocket6, SOMAXCONN) == SOCKET_ERROR) {
		errorHandler::reportWindowsError("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(this->ListenSocket6);
	}

	this->addPfds(this->ListenSocket6);

	freeaddrinfo(res);
}

void Server::addPfds(uint64_t toAdd)
{
	struct pollfd newfd;

	newfd.fd = toAdd;
	newfd.events = POLLIN;

	this->fds.push_back(newfd);
}

void Server::removefds(int index)
{
	this->fds.erase(std::next(this->fds.begin(), index));
}

void Server::addUserName(std::string name)
{
	this->names.push_back(name);
}

void Server::deleteUserName(int index)
	{
	this->names.erase(std::next(this->names.begin(), index - 2));
}

void Server::removeUser(int index)
		{
	closesocket(this->fds[index].fd);

	this->removefds(index);

	this->deleteUserName(index);
		}

void Server::newConnection(int* addrlen, uint64_t* newfd, int choice)
		{
	struct sockaddr_storage remoteaddr;

	*addrlen = sizeof remoteaddr;

	if(choice == 4)
		*newfd = accept(this->ListenSocket4, (struct sockaddr*)&remoteaddr, addrlen);
					else
		*newfd = accept(this->ListenSocket6, (struct sockaddr*)&remoteaddr, addrlen);

	if (*newfd == -1)
		errorHandler::reportWindowsError("accept ERROR\n", WSAGetLastError());
	else
					{
		this->addPfds(*newfd);
						errorHandler::consolPrint("NEW CONNECTION ON THE SERVER\n");
					}
				}
				
bool Server::receiveMessage(uint64_t* newfd, char* buf, int bufSize)
				{
	int nbytes = recv(*newfd, buf, bufSize, 0);

	if (nbytes <= 0)
					{
						if (nbytes == 0)
							errorHandler::consolPrint("Connection closed\n");
						else
			errorHandler::reportWindowsError("RECEIVE ERROR\n", WSAGetLastError());

		closesocket(*newfd);

		this->removefds((int)this->fds.size() - 1);

		return false;
	}
	else 
	{
		if (nbytes >= bufSize)
			buf[bufSize - 1] = '\0';
		else
			buf[nbytes] = '\0';

		return true;
	}
}

void Server::sendMessagetoServ(uint64_t* sender, std::string msg)
{
	for (int i = 0; i < this->fds.size(); i++)
	{
		SOCKET dest_sckt = this->fds[i].fd;
	
		if (dest_sckt != this->ListenSocket4 && dest_sckt != *sender && dest_sckt != this->ListenSocket6)
		{
			if (send(dest_sckt, msg.c_str(), (int)msg.length(), 0) == -1)
			{
				errorHandler::reportWindowsError(TEXT("SEND TO SERV ERROR"), WSAGetLastError());
				this->removeUser(i);
}
		}
	}
}

void Server::newConnectionMsg(uint64_t* newfd, char* nameBuffer)
{
	if (this->receiveMessage(newfd, nameBuffer, 100))
	{
		std::string name = nameBuffer;
		std::string userThere = "User already there : ";
		std::string justConnect = name + " just joined us";
	
		for (int j = 0; j < this->names.size(); j++)
			userThere += this->names[j] + ", ";

		this->addUserName(name);

		this->sendMessagetoServ(newfd, justConnect);

		if (send(*newfd, userThere.c_str(), (int)userThere.length(), 0) == -1)
		{
			errorHandler::reportWindowsError(TEXT("SEND TO NEW USER ERROR"), WSAGetLastError());
			this->removeUser((int)this->fds.size() - 1);
}
}
}

						closesocket(this->fds[i].fd);

						this->removefds(i);
					}

					else 
					{ 
						buf[nbytes - (nbytes == sizeof buf ? 1 : 0)] = '\0';
						errorHandler::consolPrint(buf);
						errorHandler::consolPrint("\n");

						for(int j = 0; j < this->fds.size(); j++)
						{
							SOCKET dest_sckt = this->fds[j].fd;

							if(dest_sckt != this->ListenSocket && dest_sckt != sender)
							{
								if(send(dest_sckt, buf, nbytes, 0) == -1)
								{
									errorHandler::reportWindowsError(TEXT("SEND SERV ERROR"), WSAGetLastError());
								}
							}
						}


					}

				}
			}
		}
	}

}

Server::~Server()
{
	closesocket(this->ListenSocket);
	WSACleanup();
}


