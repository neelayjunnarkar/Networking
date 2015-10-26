#include "Endpoint.h"

#if defined(_WIN32) && !defined(_MSC_VER)
int inet_pton(int af, const char *src, void *dst) {
  struct sockaddr_storage ss;
  int size = sizeof(ss);
  char src_copy[INET6_ADDRSTRLEN+1];

  ZeroMemory(&ss, sizeof(ss));
  /* stupid non-const API */
  strncpy (src_copy, src, INET6_ADDRSTRLEN+1);
  src_copy[INET6_ADDRSTRLEN] = 0;

  if (WSAStringToAddress(src_copy, af, NULL, (struct sockaddr *)&ss, &size) == 0) {
    switch(af) {
      case AF_INET:
    *(struct in_addr *)dst = ((struct sockaddr_in *)&ss)->sin_addr;
    return 1;
      case AF_INET6:
    *(struct in6_addr *)dst = ((struct sockaddr_in6 *)&ss)->sin6_addr;
    return 1;
    }
  }
  return 0;
}
#endif

Endpoint::Endpoint(AddressFamily addressFamily, const char *ipAddress, u_short port):
	_addressFamily{addressFamily},
	_ipAddress{ipAddress},
	_port{port}
{
	if(_addressFamily == AddressFamily::V4) {

		_data.v4.sin_family = (int)AddressFamily::V4;
		_data.v4.sin_port = htons(_port);
		inet_pton((int)AddressFamily::V4, _ipAddress, &_data.v4.sin_addr.s_addr);

	} else if(_addressFamily == AddressFamily::V6) {

		_data.v6.sin6_family = (int)AddressFamily::V6;
		_data.v6.sin6_port = htons(_port);
		int status = inet_pton((int)AddressFamily::V6, _ipAddress, &(_data.v6.sin6_addr));
		if(status != 1) {
#ifdef _WIN32
			std::cerr << "pton error: " << status << " " << WSAGetLastError() << std::endl;
#elif defined(__linux__) //end _WIN32
			std::cerr << "pton error: " << status << std::endl;
#endif
		}
	}
}

