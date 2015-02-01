#pragma once

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

#include <iostream>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

/**
  * @author Neelay Junnarkar
  *
  * The Win_UDP_Listener class 
  *
  * Status: Works, Jan. 31, 2015
  */

#define BYTESMAX 100

class Win_UDP_Listener {
private:
	const std::string port;

	SOCKET sockfd;
	sockaddr_storage their_addr;
	socklen_t addr_len;
	std::string msg;

public:
	Win_UDP_Listener(const std::string &port = "4950");
	~Win_UDP_Listener();

	inline std::string getPort() const { return port; }

	std::string recv();
};

