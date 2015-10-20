#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <cstdint>
#pragma comment(lib, "Ws2_32.lib")

enum class SocketType {
	NONE     = 0b000,
	STREAM   = SOCK_STREAM,
	DATAGRAM = SOCK_DGRAM
};