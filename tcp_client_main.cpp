/*
Neelay Junnarkar
================

Unix TCP client
Works.
Tested with tcp_server_main.cpp
Send messages in format: <double> <string>
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>

#define MAX_DATA_SIZE 100

struct addrinfo hints, *servinfo, *p;
int sockfd;
int status; 
int bytesn;
char buf[MAX_DATA_SIZE];
char msg[MAX_DATA_SIZE];

int main(int argc, char **argv) {
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo("192.168.2.100", "3490", &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return 1;
	}

	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
		        std::cerr << "client: socket" << std::endl;
		        continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
		        close(sockfd);
		        perror("client: connect");
		        continue;
		}

		break;
	}

	if (p == nullptr) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	freeaddrinfo(servinfo);
	if ((bytesn = recv(sockfd, buf, MAX_DATA_SIZE-1, 0)) == -1) {
		std::cerr << "recv" << std::endl;
		return 3;
	}	
	buf[bytesn] = '\0';

	std::cout << buf << std::endl;
	std::cout << "client: type in message: ";
	while (std::cin.getline(msg, 128) && std::cout << "client: type in message: ") {

		if (send(sockfd, msg, sizeof(buf), 0) == -1)
			std::cerr << "send" << std::endl;
	}

	close(sockfd);
	return 0;
}
