#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

#include <iostream>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

#define PORT "4950"
#define BYTESMAX 100

SOCKET sockfd = INVALID_SOCKET;
int status;
addrinfo hints, *servinfo, *p;
sockaddr_storage their_addr;
socklen_t addr_len;
char s[INET_ADDRSTRLEN];
int bytesn;
char buf[BYTESMAX];

timeval tv;
fd_set masterfds, readfds;
int fdmax;

int main() {
	WSADATA wsaData;
	status = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (status != 0) {
		printf("WSAStartup failed with error: %d\n", status);
		std::cin.ignore();
		return 1;
	}

	tv.tv_sec = 10;
	tv.tv_usec = 0;
	FD_ZERO(&masterfds);
	FD_ZERO(&readfds);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		WSACleanup();
		return 1;
	}

	for (p = servinfo; p != nullptr; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == INVALID_SOCKET) {
			std::cerr << "server: socket" << std::endl;
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == SOCKET_ERROR) {
			closesocket(sockfd);
			std::cerr << "server: bind" << std::endl;
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo);

	if (sockfd == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		std::cin.ignore();
		return 1;
	}
	FD_SET(sockfd, &masterfds);
	FD_SET(sockfd, &readfds);
	fdmax = sockfd;

	while (true) {

		readfds = masterfds;
		if (select(fdmax + 1, &readfds, nullptr, nullptr, &tv) == -1) {
			std::cerr << "select" << std::endl;
			closesocket(sockfd);
			WSACleanup();
			std::cin.ignore();
			return 1;
		}

		for (int i = 0; i <= fdmax; ++i) {
			if (FD_ISSET(i, &readfds)) {
				addr_len = sizeof their_addr;
				if ((bytesn = recvfrom(sockfd, buf, BYTESMAX - 1, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
					std::cerr << "recvfrom" << std::endl;
				}
				buf[bytesn] = '\0';
				std::cout << "talker: " << buf << std::endl;
				memset(&buf, 0, sizeof(buf));
			}
		}
		std::cout << "end while loop" << std::endl;

	}
	closesocket(sockfd);
	WSACleanup();

	return 0;
}