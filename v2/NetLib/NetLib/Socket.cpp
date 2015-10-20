#include "Socket.h"

Socket::Socket(AddressFamily addressFamily, SocketType socketT, ProtocolType protocolT) :
	_addressFamily{addressFamily},
	_socketT{socketT},
	_protocolT{protocolT} {

	_fileDescriptor = socket((int)_addressFamily, (int)_socketT, (int)_protocolT);

}

int Socket::connect(const Socket &socket, Endpoint endpoint) {
	int status = ::connect(socket._fileDescriptor, endpoint.data(), sizeof *endpoint.data());
	if(status == SOCKET_ERROR) {
		std::cerr << "error at connect(): " << WSAGetLastError() << std::endl;
		return status;
	}
	return status;
}

int Socket::connectTo(const Endpoint &endpoint) {
	return Socket::connect(*this, endpoint);
}

int Socket::bind(const Socket &socket, Endpoint endpoint) {
	int status = SOCKET_ERROR;
	status = ::bind(socket._fileDescriptor, endpoint.data(), sizeof *endpoint.data());
	if(status == SOCKET_ERROR) {
		std::cerr << "error at bind(): " << WSAGetLastError() << std::endl;
		return status;
	}
	return status;
}

int Socket::bindTo(const Endpoint &endpoint) {
	return Socket::bind(*this, endpoint);
}

Socket Socket::accept(Endpoint *const endpoint) { //should base properties of socket based on properties of endpoint
	Socket client{_addressFamily, _socketT, _protocolT};
	client._fileDescriptor = ::accept(_fileDescriptor, endpoint->data(), nullptr);

	if(client._fileDescriptor == SOCKET_ERROR)
		std::cerr << "error at accept: " << WSAGetLastError() << std::endl;
	return client;
}

int Socket::listen(unsigned maxBacklog) {
	int status = ::listen(_fileDescriptor, maxBacklog);
	if(status == SOCKET_ERROR) {
		std::cerr << "error at listen(): " << WSAGetLastError() << std::endl;
		return status;
	}
	return status;
}


int Socket::recv(char *bytes, unsigned byteslen, int flags) {
	int numbytes = ::recv(_fileDescriptor, bytes, byteslen, 0);
	if(numbytes < 0)
		std::cerr << "error at recv(): " << WSAGetLastError() << std::endl;
	bytes[numbytes] = '\0';
	return numbytes;
}

int Socket::send(char *bytes, unsigned byteslen, int flags) {
	int numbytes = ::send(_fileDescriptor, bytes, byteslen, flags);
	if(numbytes < 0)
		std::cerr << "error at send(): " << WSAGetLastError() << std::endl;
	return numbytes;
}
