/*
Neelay Junnarkar
================

Windows UDP talker
Works. 
Tested with udp_listener_main.cpp
Send messages in format: <string>
*/
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

#define PORT 4950
#define LISTENERIP "192.168.2.100"

int main() {

	int status;
	WSADATA wsaData;

	SOCKET sock = INVALID_SOCKET;
	sockaddr_in recvAddr;
	
	std::string msg;

	if((status = WSAStartup(MAKEWORD(2, 2), &wsaData)) != NO_ERROR) {
		printf("talker: WSAStartup failed with error: %d\n", status);
		std::cin.ignore();
		return 1;
	}

	if((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
		printf("talker: socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		std::cin.ignore();
		return 1;
	}

	recvAddr.sin_family = AF_INET;
	recvAddr.sin_port = htons(PORT);
	recvAddr.sin_addr.s_addr = inet_addr(LISTENERIP);
	char *buf = new char[100];

	//while(std::cout << "talker: type msg: " && std::cin.getline(buf, 100)) {
	while (true) {
		msg = buf;
		status = sendto(sock, "hii", 3, 0,
			(SOCKADDR *) &recvAddr, sizeof(recvAddr));
		if(status == SOCKET_ERROR) {
			printf("talker: sendto failed with error: %d\n", WSAGetLastError());
			closesocket(sock);
			WSACleanup();
			std::cin.ignore();
			return 1;
		}

		std::this_thread::sleep_for((std::chrono::milliseconds)4);
	}

	printf("talker: Finished sending. Closing socket.\n");
	if((status = closesocket(sock)) == SOCKET_ERROR) {
		printf("closesocket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		std::cin.ignore();
		return 1;
	}

	printf("talker: Exiting.\n");
	WSACleanup();
	std::cin.ignore();
	return 0;
}
