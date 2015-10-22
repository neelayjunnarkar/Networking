#include "Socket.h"

Socket::Socket(AddressFamily addressFamily, SocketType socketT, ProtocolType protocolT) :
	_addressFamily{addressFamily},
	_socketT{socketT},
	_protocolT{protocolT} {

	_fileDescriptor = socket((int)_addressFamily, (int)_socketT, (int)_protocolT);

	//error checking
	if(_fileDescriptor == SocketError::FAILED_SOCKET) {
#ifdef _WIN32
		std::cerr << "error at socket(): " << WSAGetLastError() << std::endl;
#elif defined(__linux__) //end _WIN32
		std::cerr << "error at client(); file descriptor = " << _fileDescriptor << std::endl;
#endif //end __linux__
	} //end error checking


}

int Socket::connect(const Socket &socket, Endpoint endpoint) {
	int status = ::connect(socket._fileDescriptor, endpoint.data(), sizeof *endpoint.data());

	//error checking
	if(status == SocketError::GENERAL) {
#ifdef _WIN32
		std::cerr << "error at connect(): " << WSAGetLastError() << std::endl;
#elif defined(__linux__) //end _WIN32
		std::cerr << "error at connect(): " << status << std::endl;
#endif //end __linux__
	} //end error checking

	return status;
}

int Socket::connectTo(const Endpoint &endpoint) {
	return Socket::connect(*this, endpoint);
}

int Socket::bind(const Socket &socket, Endpoint endpoint) {

	int status = SocketError::GENERAL;

	status = ::bind(socket._fileDescriptor, endpoint.data(), sizeof *endpoint.data());

	//error checking
	if(status == SocketError::GENERAL) {
#ifdef _WIN32
		std::cerr << "error at bind(): " << WSAGetLastError() << std::endl;
#elif defined(__linux__) //end _WIN32
		std::cerr << "error at bind(): " << status << std::endl;
#endif //end __linux__
	} //end error checking

	return status;
}

int Socket::bindTo(const Endpoint &endpoint) {
	return Socket::bind(*this, endpoint);
}

Socket Socket::accept(Endpoint *const endpoint) { //should base properties of socket based on properties of endpoint
	Socket client{_addressFamily, _socketT, _protocolT};
	client._fileDescriptor = ::accept(_fileDescriptor, endpoint->data(), nullptr);

	//error checking
	if(client._fileDescriptor == SocketError::FAILED_SOCKET) {
#ifdef _WIN32
		std::cerr << "error at accept: " << WSAGetLastError() << std::endl;
#elif defined(__linux__) //end _WIN32
		std::cerr << "error at accept(); file descriptor = " << client._fileDescriptor << std::endl;
#endif //end __linux__
	} //end error checking
	
	return client;
}

int Socket::listen(unsigned maxBacklog) {
	int status = ::listen(_fileDescriptor, maxBacklog);

	//error checkin
	if(status == SocketError::GENERAL) {
#ifdef _WIN32
		std::cerr << "error at listen(): " << WSAGetLastError() << std::endl;
#elif defined(__linux__) //end __WIN32
		std::cerr << "error at listen(): " << status << std::endl;
#endif //end __linux__
	} //end error checking

	return status;
}


int Socket::recv(char *bytes, unsigned byteslen, int flags) {
	int numbytes = ::recv(_fileDescriptor, bytes, byteslen, 0);

	//error checking
#ifdef _WIN32
	if(numbytes < 0) {
		std::cerr << "error at recv(): " << WSAGetLastError() << std::endl;
		return numbytes;
	}
#elif defined(__linux__) //end _WIN32
	if(numbytes == SocketError::GENERAL) {
		std::cerr << "error at recv(): " << numbytes << std::endl;
		return numbytes;
	}
#endif //end __linux__

	bytes[numbytes] = '\0';
	return numbytes;
}

int Socket::send(char *bytes, unsigned byteslen, int flags) {
	int numbytes = ::send(_fileDescriptor, bytes, byteslen, flags);

	//error checking
#ifdef _WIN32
	if(numbytes < 0)
		std::cerr << "error at send(): " << WSAGetLastError() << std::endl;

#elif defined(__linux__) //end _WIN32
	if(numbytes < 0)
		std::cerr << "error at send(): " << numbytes << std::endl;
#endif //end __linux__

	return numbytes;
}
