/*
Neelay Junnarkar
================

Windows TCP client
Works.
Tested with tcp_server_main.cpp
Send messages in format: <double> <str>
*/

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define MAX_DATA_SIZE 512
#define PORT "3490"

int __cdecl main(int argc, char **argv) {
	WSADATA wsaData;
	SOCKET sock = INVALID_SOCKET;
	struct addrinfo *result = nullptr,
		*ptr = nullptr,
		hints;

	char recvbuf[MAX_DATA_SIZE];
	int status;

	status = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(status != 0) {
		printf("WSAStartup failed with error: %d\n", status);
		std::cin.ignore();
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	status = getaddrinfo("192.168.2.100", PORT, &hints, &result);
	if(status != 0) {
		printf("getaddrinfo failed with error: %d\n", status);
		WSACleanup();
		std::cin.ignore();
		return 1;
	}

	for(ptr = result; ptr != nullptr; ptr = ptr->ai_next) {

		sock = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if(sock == INVALID_SOCKET) {
			printf("sock failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		status = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
		if(status == SOCKET_ERROR) {
			closesocket(sock);
			sock = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if(sock == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		std::cin.ignore();
		return 1;
	}

	if((status = recv(sock, recvbuf, MAX_DATA_SIZE-1, 0)) == -1) {
		printf("recv failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		std::cin.ignore();
		return 1;
	}

	printf("Bytes Sent: %ld\n", status);

	char *msg = new char[MAX_DATA_SIZE];
	while(std::cout << "client: type in message: " && std::cin.getline(msg, 128)) {
		std::cout << "in loop" << std::endl;
		status = send(sock, msg, (int)strlen(msg), 0);
		if(status == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			continue;
		}

	}

	closesocket(sock);
	WSACleanup();

	std::cin.ignore();
	return 0;
}
