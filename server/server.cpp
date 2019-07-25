#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include <thread>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "eterm.h"

using namespace std;


// Driver function
int main() {
		eterm e;
 		std::thread th1(&eterm::clientHandler, &e);
		std::thread th2(&eterm::PNR_maintain, &e);

		th1.join();
 		th2.join();
	//	e.clientHandler();
		return 0;
}
