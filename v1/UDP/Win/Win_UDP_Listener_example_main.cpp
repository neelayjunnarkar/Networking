#include "Win_UDP_Listener.h"

#include <iostream>

/**
  * @author Neelay Junnarkar
  *
  * Example program that demonstrates usage of the Win_UDP_Listener class
  *
  * Status: Works, Jan. 31, 2015
  */

int main() {

	Win_UDP_Listener udp_listener{"4950"}; //listens on port 4950

	while (true) {
		std::cout << udp_listener.recv() << std::endl;
	}
}