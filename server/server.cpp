#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <netdb.h>
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
		e.clientHandler();
}
