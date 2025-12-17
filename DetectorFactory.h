#ifndef DETECTORFACTORY_H
#define DETECTORFACTORY_H

#include "Device.h"
#include <vector>
#include <string>

class Detector : public Sensor {
public:
    Detector(const std::string& kind);
    virtual bool isUserPowerOffAllowed() const;
    virtual void turnOff();
};

class SmokeDetector : public Detector {
public:
    SmokeDetector();
    virtual Device* clone() const;
};

class GasDetector : public Detector {
public:
    GasDetector();
    virtual Device* clone() const;
};

class DetectorFactory {
public:
    static std::vector<Device*> createDetectorPair(const SmokeDetector& smokePrototype);
};

#endif
