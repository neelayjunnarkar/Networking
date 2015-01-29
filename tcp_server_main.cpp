/*
Neelay Junnarkar
================

Unix TCP Server
Works.
Tested with tcp_client_main.cpp and win_tcp_client_main.cpp
Recieves messages in format: <double> <string>
*/
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
#include <signal.h>
#include <iostream>
#include <string>


#define PORT "3490"
#define BACKLOG 10
#define BYTESMAX 100

int sockfd, new_fd;
int status;
struct addrinfo hints, *servinfo, *p;
struct sockaddr_storage their_addr;
socklen_t sin_size;
int yes = 1;
char s[INET_ADDRSTRLEN];
int bytesn;
char buf[BYTESMAX];

double dist;
std::string side;

int main() {
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
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
		
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			std::cerr << "sockopt" << std::endl;
			return 1;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			std::cerr << "server: bind" << std::endl;
			continue;
		}

		break;
	}

	if (p == nullptr) {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}
	freeaddrinfo(servinfo);

	if (listen(sockfd, BACKLOG) == -1) {
		std::cerr << "listen" << std::endl;
		return 1;
	}
	
	std::cout << "server: waiting for connection" << std::endl;

	while (true) {
		sin_size = sizeof their_addr;
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
			std::cerr << "accept" << std::endl;
			continue;
		}	

		std::cout << "server: connection achieved" << std::endl;

		if (!fork()) {
			close(sockfd);
	
			if (send(new_fd, "server: connection achieved", 27, 0) == -1)
				std::cerr << "send" << std::endl;
			
		}
		break;
	}
	
	while (true) {
						
			if ((bytesn = recv(new_fd, buf, BYTESMAX-1, 0)) == -1) {
				std::cerr << "recv" << std::endl;
			}			
			if (bytesn == 0) {
				std::cerr << "server: connection closed" << std::endl;
				return 0;
			}


			buf[bytesn] = '\0';
			std::string buf_str{buf};
			std::string::size_type sz;
			try {
				dist = std::stod(buf_str, &sz);
			} catch (std::exception e) {
				continue;
			}
			buf_str.resize(strlen(buf));
			side = buf_str.substr(sz);
			std::cout << "dist: " << dist << "     side: " << side << std::endl;

			memset(&buf, 0, sizeof(buf));
			buf_str.clear();

	}

	close(new_fd);
	close(sockfd);
	return 0;
}
