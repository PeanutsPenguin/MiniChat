#pragma once

class Server
{
public:
	Server() = default;

	void Start(int majorVersion, int minorVersion);

	void Close();

	~Server();
};