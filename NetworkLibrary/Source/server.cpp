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

	this->ListenSocket6 = INVALID_SOCKET;
}

void Server::CreateBindListen(const char* port)
{
	this->createSckt(port);
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
	this->names.erase(std::next(this->names.begin(), index - 1));
}

void Server::removeUser(int index)
{
	closesocket(this->fds[index].fd);

	this->removefds(index);

	this->deleteUserName(index);
}

void Server::newConnection(int* addrlen, uint64_t* newfd)
{
	struct sockaddr_storage remoteaddr;

	*addrlen = sizeof remoteaddr;

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

		if (dest_sckt != *sender && dest_sckt != this->ListenSocket6)
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

bool Server::clearServer(char* buf)
{
	std::string msg = buf;
	std::string check = "/closeServer";
	if (buf == check)
	{
		for (int i = 1; i < this->fds.size(); i++)
		{
			int iResult = shutdown(this->fds[i].fd, SD_SEND);


			if (iResult == SOCKET_ERROR)
				errorHandler::reportWindowsError("shutdown failed\n", WSAGetLastError());


			closesocket(this->fds[i].fd);
		}

		closesocket(this->fds[0].fd);

		return true;
	}

	return false;
}

void Server::createSckt(const char* port)
{
	struct addrinfo* res = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	int resultCheck = getaddrinfo(NULL, port, &hints, &res);

	if (resultCheck != 0)
		errorHandler::reportWindowsError("getaddrinfo failed: %d\n", resultCheck);
	
	this->ListenSocket6 = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	if (this->ListenSocket6 == INVALID_SOCKET) 
	{
		errorHandler::reportWindowsError("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(res);
	}

	int disable = 0;
	int result = setsockopt(this->ListenSocket6, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&disable, sizeof(disable));
	if (result == SOCKET_ERROR)
		errorHandler::reportWindowsError("SOCKET OPTION ERR:", WSAGetLastError());

	resultCheck = bind(this->ListenSocket6, res->ai_addr, (int)res->ai_addrlen);
	
	if (resultCheck == SOCKET_ERROR) 
	{
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

void Server::pollCall()
{
	SOCKET newfd;        
	socklen_t addrlen;
	std::string buf;
	char nameBuffer[100];
	char receiveBuf[512];

	for(;;)
	{
		int poll_count = WSAPoll(&this->fds[0], (u_long)this->fds.size(), -1);

		if(poll_count == -1)
		{
			errorHandler::reportWindowsError("POLL ERROR", WSAGetLastError());
			break;
		}

		for(int i = 0; i < this->fds.size(); i++)
		{
			//If the socket is disconnected
			if(this->fds[i].revents & POLLERR | this->fds[i].revents & POLLHUP)
			{
				std::string msg = this->names[i - 1];
				this->removeUser(i);
				msg += " has been disconnected";

				this->sendMessagetoServ(&this->ListenSocket6, msg);
			}
				

			else if (this->fds[i].revents & POLLIN)
			{
				//If this is a new socket 
				if (fds[i].fd == this->ListenSocket6)
				{
					this->newConnection(&addrlen, &newfd);

					this->newConnectionMsg(&newfd, &nameBuffer[0]);
				}
				
				else 
				{
					SOCKET sender = this->fds[i].fd;

					//If this is a new message
					if (this->receiveMessage(&this->fds[i].fd, &receiveBuf[0], 512))
					{
						if (this->clearServer(receiveBuf))
							return;

						buf = this->names[i - 1] + " > " + receiveBuf;

						errorHandler::consolPrint(buf.c_str());
						errorHandler::consolPrint("\n");

						this->sendMessagetoServ(&sender, buf);
					}
					else
						this->deleteUserName(i);
				}
			}	
		}
	}
}

Server::~Server()
{
	WSACleanup();
}

