#include "uml/managers/umlServer.h"
#include <iostream>
#include <unistd.h>

/**
 * This is the main file for the executable uml-server. by running the executable it will start a server
 * to handle uml clients being invoked elsewhere.
 * There are some commandline options:
 *  --port, -p : specify the port that the server will listen on, default 8652
 *  --mount-path, -m : specify the path that the server will be run on, default the path ran from
 *  --location, -l : load from and save to the path specified
 **/

int main(int argc, char* argv[]) {
    int i = 0;
    int port = 8652;
    std::string path = ".";
    std::string location;
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
            dashDash = (char*)realloc(dashDash, 11);
            memcpy(dashDash, &argv[i][0], 10);
            dashDash[10] = '\0';
            if (strcmp(dashDash, "--location") == 0) {
                free(dashDash);
                path = &argv[i][11];
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
    UML::UmlServer server(port);
    if (!location.empty()) {
        server.open(location);
    }
    server.mount(path);
    server.waitTillShutDown();
    server.save(location);
    server.shutdown();
}