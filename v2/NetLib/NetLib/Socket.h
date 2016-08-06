#pragma once
#include "AddressFamily.h"
#include "SocketType.h"
#include "ProtocolType.h"
#include "Error.h"

#include "Endpoint.h"

#include <vector>
#include <iostream>

class Socket {
public:
	static const unsigned int MAXBYTES = 256;
	Socket(AddressFamily addressFamily, SocketType socketT, ProtocolType protocolT);

	static int connect(const Socket &socket, Endpoint endpoint);
	int connectTo(Endpoint endpoint);

	static int bind(const Socket &socket, Endpoint endpoint);
	int bindTo(const Endpoint &endpoint);

	Socket accept(Endpoint *const endpoint = nullptr);

	int listen(unsigned maxBacklog = SOMAXCONN);

	///<returns>Returns the bytes recieved</returns>
	char* recv(int *num_bytes, unsigned bytesLen = MAXBYTES, int flags = 0);
	std::string recv(unsigned bytesLen = MAXBYTES, int flags = 0);

	///<returns>Returns the number of bytes sent</returns>
	int send(char *bytes, unsigned bytesLen, int flags = 0);
	int send(std::string bytes, int flags = 0);
private:
	int _fileDescriptor;
	AddressFamily _addressFamily;
	SocketType _socketT;
	ProtocolType _protocolT;

};