/*
 ** A general description for your program.
 ** Author name and date list here, too.
 */
#include <iostream>
#include <string>
#include "UI.h"
#include "eterm.h"

using namespace std;

int main(int argc, char * argv[]) {
    eterm e;   // server
  //  e.PNR_maintain();  // keep running to remove the PNR with expired ticketing
    UI u(e);   // client
    u.getCommand();  // client side keeps getting commands and then talk to the server
}
