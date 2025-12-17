#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>

class Logger {
private:
    std::ofstream _file;

    Logger();
    Logger(const Logger&);
    Logger& operator=(const Logger&);

    std::string nowString() const;

public:
    static Logger& instance();
    void log(const std::string& msg);
    void close();
    ~Logger();
};

#endif
