#ifndef SCENARIOS_H
#define SCENARIOS_H

#include "Device.h"
#include <vector>

namespace Scenarios {
    void securitySequence(std::vector<Device*>& devs);
    void fireSequence(std::vector<Device*>& devs);
    void simulateFailure(std::vector<Device*>& devs);
}

#endif
