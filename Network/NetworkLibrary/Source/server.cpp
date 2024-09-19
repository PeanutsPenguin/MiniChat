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

	this->ListenSocket = INVALID_SOCKET;
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

	this->ListenSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	if (this->ListenSocket == INVALID_SOCKET) {
		errorHandler::reportWindowsError("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(res);
	}

	resultCheck = bind(this->ListenSocket, res->ai_addr, (int)res->ai_addrlen);
	if (resultCheck == SOCKET_ERROR) {
		errorHandler::reportWindowsError("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(res);
		closesocket(this->ListenSocket);
	}
	
	if (listen(this->ListenSocket, SOMAXCONN ) == SOCKET_ERROR) {
		errorHandler::reportWindowsError("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(this->ListenSocket);
	}

	this->addPfds(this->ListenSocket);

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

void Server::pollCall()
{
	SOCKET newfd;        
	struct sockaddr_storage remoteaddr; 
	socklen_t addrlen;
	char buf[512];


	for(;;)
	{
		int poll_count = WSAPoll(&this->fds[0], (u_long)this->fds.size(), -1);

		if(poll_count == -1)
		{
			errorHandler::consolPrint("POLL ERROR");
			break;
		}

		for(int i = 0; i < this->fds.size(); i++)
		{
			if (this->fds[i].revents & POLLIN)
			{
				if (fds[i].fd == this->ListenSocket)
				{
					addrlen = sizeof remoteaddr;
					newfd = accept(this->ListenSocket, (struct sockaddr*)&remoteaddr, &addrlen);


					if (newfd == -1)
						errorHandler::consolPrint("accept ERROR\n");
					else
					{
						this->addPfds(newfd);
						errorHandler::consolPrint("NEW CONNECTION ON THE SERVER\n");
					}
				}
				
				else 
				{
					int nbytes = recv(this->fds[i].fd, buf, sizeof buf, 0);

					SOCKET sender = this->fds[i].fd;

					if(nbytes <= 0)
					{
						if (nbytes == 0)
							errorHandler::consolPrint("Connection closed\n");
						else
							errorHandler::consolPrint("RECEIVE ERROR\n");

						closesocket(this->fds[i].fd);

						this->removefds(i);
					}

					else 
					{ 
						buf[nbytes - (nbytes == sizeof buf ? 1 : 0)] = '\0';
						errorHandler::consolPrint(buf);

						for(int j = 0; j < this->fds.size(); j++)
						{
							SOCKET dest_sckt = this->fds[j].fd;

							if(dest_sckt != this->ListenSocket && dest_sckt != sender)
							{
								if(send(dest_sckt, buf, nbytes, 0) == -1)
								{
									errorHandler::consolPrint("SEDN ERROR");
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


