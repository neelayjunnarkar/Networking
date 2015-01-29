#include "UDP_Talker.h"

UDP_Talker::UDP_Talker(std::string ip, std::string port):
	ip{ip},
	port{port}
{
	addrinfo hints, *servinfo;
	int status;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	if ((status = getaddrinfo(ip.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
		std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
	}
	
	for (p = servinfo; p != nullptr; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			std::cerr << "talker: socket" << std::endl;
			continue;
		}
		break;
	}

	if (p == nullptr) {
		std::cerr << "talkr faillled" << std::endl;
	}

	freeaddrinfo(servinfo);
}

UDP_Talker::~UDP_Talker() {
	close(sockfd);
	freeaddrinfo(p);
}

int UDP_Talker::send(std::string msg) {
	int bytesn;
	if ((bytesn = sendto(sockfd, msg.c_str(), msg.length(), 0, p->ai_addr, p->ai_addrlen)) == -1) {
		std::cerr << "talker: sendto" << std::endl;
		return bytesn;
	}
	return bytesn;
}

