# MiniChat
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#demo">Demo</a>
    </li>
    <li>
      <a href="#server">Server</a>
    </li>
    <li>
      <a href="#client">Client</a>
    </li>
    <li>
    <a href="#contact">Contact</a>
    </li>
  </ol>
</details>

## Summary

School Project to discover the basics of networking with Windows. The goal was to create a chat base on TCP/IP protocol.

## Demo
https://github.com/user-attachments/assets/c0c2e150-dced-4c2c-ba98-5f70d043815f

## Server 
The main idea of the server part was to hide everything from the client but still send the right data to them. 
This is for exemple a snippet of Server's main loop :

```cpp
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
```
## Client 
The client have only two main phases wich are the connection to the server and the send/reception of messages. 
Here's a snippet of the Client connection code : 

```cpp
	while (resultCheck != 0)
	{
		errorHandler::consolPrint("Enter an IP Adress : ");
		std::cin >> ipAdress;

		resultCheck = getaddrinfo(ipAdress.c_str(), port, &hints, &res);
		if (resultCheck != 0)
			continue;

		this->data = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

		if (this->data == INVALID_SOCKET) 
		{
			errorHandler::reportWindowsError("Error at socket(): \n", WSAGetLastError());
			resultCheck = -1;
			continue;
		}
		else
			resultCheck = 0;

		if (connect(this->data, res->ai_addr, (int)res->ai_addrlen) < 0)
		{
			errorHandler::reportWindowsError("UNABLE TO CONNECT\n", WSAGetLastError());
			resultCheck = -1;
			continue;
		}
		else
		{
			errorHandler::consolPrint("SUCCESSFULLY CONNECTED\n");
			resultCheck = 0;
		}
			
	}
```
## Contact
<u>**Malo Sadoine**</u>

- [![Linkedin][LinkedIn]][LinkedIn-url]
- mal.sadoine@gmail.com

[LinkedIn]: https://img.shields.io/badge/linkedin-34a8eb?style=for-the-badge&logo=linkedin
[LinkedIn-url]: https://www.linkedin.com/in/malo-sadoine-098b7a254/

