#pragma once

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <cstdint>
#pragma comment(lib, "Ws2_32.lib")

#include "AddressFamily.h"
#include "ProtocolType.h"
#include "SocketType.h"

#include <iostream>

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
