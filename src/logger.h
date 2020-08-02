#ifndef LOGGER
#define LOGGER
#include <string>
#include <iostream>

using namespace std;

class Logger {

    private:
        //TODO create file for logging
    public:
        void logInfo(string msg);
        void logWarning(string msg);
        void logError(string msg);
        void clearLog();
};

#endif