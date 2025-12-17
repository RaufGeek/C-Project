#ifndef MODES_H
#define MODES_H

#include "Device.h"
#include <vector>
#include <string>

namespace Modes {
    std::string parseMode(char m);
    std::string parseState(char s);
    void applyMode(std::vector<Device*>& devices, const std::string& mode);
}

#endif
