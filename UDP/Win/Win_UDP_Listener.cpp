#include "Win_UDP_Listener.h"


Win_UDP_Listener::Win_UDP_Listener(const std::string &port) :
port(port) {

	bool error = false;

	int status;
	addrinfo hints, *servinfo, *p;

	WSADATA wsaData;
	status = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (status != 0) {
		printf("WSAStartup failed with error: %d\n", status);
		error = true;
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(NULL, port.c_str(), &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		error = true;
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
		error = true;
	}

	if (error) {
		closesocket(sockfd);
		WSACleanup();
	}


}


Win_UDP_Listener::~Win_UDP_Listener() {
	closesocket(sockfd);
}

std::string Win_UDP_Listener::recv() {
	msg.clear();
	char buf[BYTESMAX];
	int bytesn;
	addr_len = sizeof(their_addr);
	if ((bytesn = recvfrom(sockfd, buf, BYTESMAX - 1, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
		std::cerr << "recfrom" << std::endl;
		msg = "error";
	} else {
		buf[bytesn] = '\0';
		msg.resize(bytesn);
		msg = buf;
	}
	return msg;

}