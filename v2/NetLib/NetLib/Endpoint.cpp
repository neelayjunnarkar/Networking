#include "Endpoint.h"

Endpoint::Endpoint(AddressFamily addressFamily, const char *ipAddress, u_short port) :
	_addressFamily{addressFamily},
	_ipAddress{ipAddress},
	_port{port} {
	if(_addressFamily == AddressFamily::V4) {

		_data.v4.sin_family = (int)AddressFamily::V4;
		_data.v4.sin_port = htons(_port);
		if(strcmp(ipAddress, "127.0.0.1") == 0) {
			_data.v4.sin_addr.s_addr = INADDR_ANY;
		} else {
			InetPton((int)AddressFamily::V4, _ipAddress, &_data.v4.sin_addr.s_addr);
		}
	} else if(_addressFamily == AddressFamily::V6) {

		_data.v6.sin6_family = (int)AddressFamily::V6;
		_data.v6.sin6_port = htons(_port);
		// _data.v6.sin6_addr = in6addr_any;
		int status = InetPton((int)AddressFamily::V6, _ipAddress, &(_data.v6.sin6_addr));
		if(status != 1) {
#ifdef _WIN32
			std::cerr << "pton error: " << status << " " << WSAGetLastError() << std::endl;
#elif defined(__linux__) //end _WIN32
			std::cerr << "pton error: " << status << std::endl;
#endif
		}
	}
}