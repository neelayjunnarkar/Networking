#include "Socket.h"

#define MAXBYTES 128
#define CLIENT

int main() {
	//initialize winsock
	WSAData wsaData;
	int status = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(status != 0) {
		printf("WSAStartup failed with error: %d\n", status);
		WSACleanup();
		std::cin.ignore();
		return 1;
	}

	Socket socket{AddressFamily::V4, SocketType::STREAM, ProtocolType::TCP};
#ifdef CLIENT
	socket.connectTo(Endpoint{AddressFamily::V4, "127.0.0.1", 4950});
	char *message = "test sending message";
	int numbytes = socket.send(message, strlen(message));
	std::cout << "number of bytes sent: " << numbytes << std::endl;

	char buf[MAXBYTES];
	numbytes = socket.recv(buf, sizeof buf);
	std::cout << "recieved " << numbytes << " bytes: " << buf <<  std::endl;
#endif 


#ifdef SERVER
	socket.bindTo(Endpoint{AddressFamily::V4, "127.0.0.1", 4950});
	socket.listen();
	
	Socket client = socket.accept();
	char bytes[MAXBYTES];
	int numbytes = client.recv(bytes, sizeof bytes);
	std::cout << "recieved " << numbytes << " bytes: " << bytes <<  std::endl;

	char *send = "return send message";
	numbytes = client.send(send, strlen(send), 0);
	std::cout << "number of bytes sent: " << numbytes << std::endl;
#endif
	std::cin.ignore();
	WSACleanup();
}