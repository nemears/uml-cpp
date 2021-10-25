#include "uml/managers/umlServer.h"
#include <iostream>
#include <unistd.h>

int main(int argc, char* argv[]) {
    UML::UmlServer server;
    int min = 0, hour = 0;
    while(1) {
        std::cout << hour << ":" << (min < 10 ? "0" + std::to_string(min) : std::to_string(min)) << std::endl;
        sleep(60);
        min ++;
        if (min == 60) {
            min = 0;
            hour++;
        }
    }
}