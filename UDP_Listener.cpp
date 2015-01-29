#include "UDP_Listener.h"

UDP_Listener::UDP_Listener(std::string port) :
	port(port)
{
	int status;
	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(NULL, port.c_str(), &hints, &servinfo)) != 0) {
		fprintf(stderr, "cRIO: getaddrinfo: %s\n", gai_strerror(status));
	}

	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			std::cerr << "cRIO: socket" << std::endl;
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			std::cerr << "cRIO: bind" << std::endl;
			continue;
		}
		break;
	}

	if (p == NULL) {
		fprintf(stderr, "cRIO: failed to bind\n");
	}
	freeaddrinfo(servinfo);

	int bytesn;
	std::string conn_msg = "cRIO: connection achieved";
	if ((bytesn = sendto((int)sockfd, (char *)conn_msg.c_str(), (int)conn_msg.length(), 0,
		(struct sockaddr *)p->ai_addr, (int)p->ai_addrlen)) == -1) {
		std::cerr << "cRIO: failure at \"connection achieved sendto\"" << std::endl;
	}

}

UDP_Listener::~UDP_Listener() {
	close(sockfd);
}

std::string  UDP_Listener::recv() {
	msg.clear();
	char buf[BYTESMAX];
	int bytesn;
	addr_len = sizeof(their_addr);

	if ((bytesn = recvfrom(sockfd, buf, BYTESMAX - 1, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
		std::cerr << "recvfrom" << std::endl;
	}
	buf[bytesn] = '\0';

	msg.resize(bytesn);
	msg = buf;

	return msg;
}
