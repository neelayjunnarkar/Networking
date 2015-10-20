#include <iostream>

#include "UDP_Talker.h"

/**
  * @author Neelay Junnarkar
  *
  * Example program that demonstrates usage of the UDP_Talker class
  *
  * Status: Working, Jan. 31, 2015
  */

int main() {
	UDP_Talker talker{"192.168.2.184", "4950"}; //sends to <ip> <port>
	while (true) {
		std::cout << talker.send("hii") << std::endl;
	}
	return 0;
}
