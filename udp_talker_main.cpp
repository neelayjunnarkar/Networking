/*
Neelay Junnarkar
================

Unix UDP talker
Works.
Tested with udp_listener_main.cpp
Send messages in format: <string>
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <iostream>
#include <thread>

#define PORT "4950"

int main() {

	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int status;
	int bytesn;
	std::string msg;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	if ((status = getaddrinfo("192.168.2.184", PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return 1;
	}
	
	for (p = servinfo; p != nullptr; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, 
		  p->ai_protocol)) == -1) {
			std::cerr << "talker: socket" << std::endl;
			continue;
		}
		break;
	}
	if (p == nullptr) {
		fprintf(stderr, "talker: failed to bind socket\n");
		return 2;
	}


	freeaddrinfo(servinfo);

	while (true) {
				
		if ((bytesn = sendto(sockfd, "hii", 3, 0,
		  p->ai_addr, p->ai_addrlen)) == -1) {
			std::cerr << "talker: sendto" << std::endl;
			return 3;
		}
		std::this_thread::sleep_for((std::chrono::milliseconds)4);
	};
	
	close(sockfd);
	return 0;
}
