#include "logger.h"

void Logger::logInfo(string msg) {
    cout << "[Info] " << msg << '\n';
}

void Logger::logWarning(string msg) {
    cout << "[Warning] " << msg << '\n';
}

void Logger::logError(string msg) {
    cerr << "[Error] " << msg << '\n';
}

void Logger::clearLog() {
    // TODO
}