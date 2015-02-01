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

/**
  * @author Neelay Junnarkar
  *
  * The UDP_Talker class
  *
  * Status: Works, Jan. 31, 2015
  */

class UDP_Talker {
	private:
		std::string ip; //ip sending to
		std::string port; //port sending to

		int sockfd;
		struct addrinfo *p;
	public:
		UDP_Talker(std::string ip, std::string port = "4950");
		~UDP_Talker();

		inline std::string getIP() const { return ip; }
		inline std::string getPort() const { return port; }
		
		///returns results of sendto: -1 is error, other result is number of bytes sent
		int send(std::string msg);
};
