
#ifndef UDP_LISTENER_H
#define UDP_LISTENER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include <iostream>
#include <string>

/*
A rpi class for listening to msgs from rpi to cRIO or roboRIO or whatever
*/

#define BYTESMAX 100

class UDP_Listener {
private:

	const std::string port;

	int sockfd;
	struct sockaddr_storage their_addr;
	socklen_t addr_len;
	std::string msg;

public:
	UDP_Listener(std::string port = "4950");
	~UDP_Listener();

	inline std::string getIP() const { return ip; }
	inline std::string getPort() const { return port; }

	std::string recv();
};

#endif
