#include "Logger.h"
#include <ctime>

Logger::Logger() {
    _file.open("msh_log.txt", std::ios::app);
    log("--- System Started ---");
}

Logger::~Logger() {
    close();
}

Logger& Logger::instance() {
    static Logger inst;
    return inst;
}

std::string Logger::nowString() const {
    std::time_t t = std::time(0);
    char* dt = std::ctime(&t);
    std::string s(dt ? dt : "");
    if (!s.empty() && s[s.size() - 1] == '\n') s.erase(s.size() - 1);
    return s;
}

void Logger::log(const std::string& msg) {
    if (_file.is_open()) _file << "[" << nowString() << "] " << msg << "\n";
}

void Logger::close() {
    if (_file.is_open()) {
        log("--- System Shutdown ---");
        _file.close();
    }
}
