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
    eterm e;
    UI u(e);
    u.getCommand();
}
