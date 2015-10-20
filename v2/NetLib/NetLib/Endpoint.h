#pragma once

#include <iostream>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <cstdint>
#pragma comment(lib, "Ws2_32.lib")
#include "AddressFamily.h"
#include "ProtocolType.h"
#include "SocketType.h"

#elif defined(__linux__) //end _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif //end __linux__

class Endpoint {
public:

	Endpoint(AddressFamily addressFamily, const char *ipAddress, u_short port);

	sockaddr* data() {
		return &_data.base;
	}

	AddressFamily addressFamily() const {
		return _addressFamily;
	}

	const char* ipAddress() const {
		return _ipAddress;
	}

	u_short port() const {
		return _port;
	}

private:
	AddressFamily _addressFamily;
	const char *_ipAddress;
	u_short _port;
	union {
		sockaddr     base;
		sockaddr_in  v4;
		sockaddr_in6 v6;
	} _data;
};
