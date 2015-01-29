/*
Neelay Junnarkar
================

Unix UDP listener
Works.
Tested with udp_talker_main.cpp and win_udp_talker_main.cpp
Recieves messages in format: <string>
*/

#define PORT "4950"
#define BYTESMAX 100

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
#include <iostream>
#include <string>

timeval tv;
fd_set masterfds, readfds;
int fdmax;

int sockfd;
int status;
struct addrinfo hints, *servinfo, *p;
struct sockaddr_storage their_addr;
socklen_t addr_len;;
char s[INET_ADDRSTRLEN];
int bytesn;
char buf[BYTESMAX];

int main() {
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
		return 1;
	}

	for (p = servinfo; p != nullptr; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			std::cerr << "server: socket" << std::endl;
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			std::cerr << "server: bind" << std::endl;
			continue;
		}

		break;
	}
	freeaddrinfo(servinfo);
	if (p == nullptr) {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}

	FD_SET(sockfd, &masterfds);
	FD_SET(sockfd, &readfds);
	fdmax = sockfd;

	while (true) {
		readfds = masterfds;
		if (select(fdmax + 1, &readfds, nullptr, nullptr, &tv) == -1) {
			std::cerr << "select" << std::endl;
			close(sockfd);
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

	}

	close(sockfd);
	return 0;
}
