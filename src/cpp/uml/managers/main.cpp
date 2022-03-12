#include "uml/managers/umlServer.h"
#include <iostream>
#ifndef WIN32
#include <unistd.h>
#else
#include "winsock2.h"
#include <ws2tcpip.h>
#include <stdio.h>
#endif

/**
 * This is the main file for the executable uml-server. by running the executable it will start a server
 * to handle uml clients being invoked elsewhere.
 * There are some commandline options:
 *  --port, -p : specify the port that the server will listen on, default 8652
 *  --mount-path, -m : specify the path that the server will be run on, default the path ran from
 *  --location, -l : load from and save to the path specified
 *  --duration, -d : run for specified duration in ms
 *  --num-els, -n : max number of elements in memory before releasing
 **/

int main(int argc, char* argv[]) {
    int i = 0;
    int port = 8652;
    std::string path = ".";
    std::string location;
    int duration = -1;
    int numEls = UML_SERVER_NUM_ELS;
    srand(time(0));
    while (i < argc) {
        if (strcmp(argv[i], "-p") == 0) {
            port = atoi(argv[i+1]);
            i+= 2;
            continue;
        }
        if (strcmp(argv[i], "-m") == 0) {
            path = argv[i+1];
            i+= 2;
            continue;
        }
        if (strcmp(argv[i], "-l") == 0) {
            location = argv[i+1];
            i+=2;
            continue;
        }
        if (strcmp(argv[i], "-d") == 0) {
            duration = atoi(argv[i+1]);
            i += 2;
            continue;
        }
        if (strcmp(argv[i], "-n") == 0) {
            numEls = atoi(argv[i+1]);
            i += 2;
            continue;
        }
        char* dashDash = (char*) malloc(3);
        memcpy(dashDash, &argv[i][0], 2);
        dashDash[2] = '\0';
        if (strcmp(dashDash, "--") == 0) {
            dashDash = (char*)realloc(dashDash, 7);
            memcpy(dashDash, &argv[i][0], 6);
            dashDash[6] = '\0';
            if (strcmp(dashDash, "--port") == 0) {
                free(dashDash);
                port = atoi(&argv[i][7]);
                i++;
                continue;
            }
            dashDash = (char*)realloc(dashDash, 10);
            memcpy(dashDash, &argv[i][0], 9);
            dashDash[9] = '\0';
            if (strcmp(dashDash, "--num-els") == 0) {
                free(dashDash);
                numEls = atoi(&argv[i][10]);
                i++;
                continue;
            }
            dashDash = (char*)realloc(dashDash, 11);
            memcpy(dashDash, &argv[i][0], 10);
            dashDash[10] = '\0';
            if (strcmp(dashDash, "--location") == 0) {
                free(dashDash);
                path = &argv[i][11];
                i++;
                continue;
            } else if (strcmp(dashDash, "--duration") == 0) {
                free(dashDash);
                duration = atoi(&argv[i][11]);
                i++;
                continue;
            }
            dashDash = (char*)realloc(dashDash, 13);
            memcpy(dashDash, &argv[i][0], 12);
            dashDash[12] = '\0';
            if (strcmp(dashDash, "--mount-path") == 0) {
                free(dashDash);
                path = &argv[i][13];
                i++;
                continue;
            }
        }
        free(dashDash);
        if (i > 0) {
            std::cerr << "invalid arguments! " << argv[i];
            exit(-1);
        }
        i++;
    }
    try {
        UML::UmlServer server(port);
        if (!location.empty()) {
            server.open(location);
        }
        server.setMaxEls(numEls);
        server.mount(path);
        std::cout << "server running" << std::endl;
        if (duration < 0) {
            server.waitTillShutDown();
        } else {
            server.waitTillShutDown(duration);
        }
        if (!location.empty()) {
            server.save(location);
        }
        server.log("server has " + std::to_string(server.getNumElsInMemory()) + " of " + std::to_string(server.getMaxEls()) + " elements in memory before shutdown");
        server.shutdownServer();
        exit(0);
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        exit(1);
    }
}