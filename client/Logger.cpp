#include <ctime>
#include "objects.h"

using namespace G;

vector<string> Logger::queue = {};

void Logger::add(string log) {
    std::time_t result = std::time(nullptr);
    log.insert(0, std::string(std::asctime(std::localtime(&result))) + ": $$");
    queue.push_back(log);
};

void Logger::clear() {
    queue.clear();
};

vector<string> Logger::getLogs() {
    return queue;
};