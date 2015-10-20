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

enum class ProtocolType {
	NONE = 0b000,
	TCP  = IPPROTO_TCP,
	UDP  = IPPROTO_UDP
};