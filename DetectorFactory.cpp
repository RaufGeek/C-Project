#include "DetectorFactory.h"

Detector::Detector(const std::string& kind) : Sensor(kind + " Detector", "Detector") { _powered = true; }
bool Detector::isUserPowerOffAllowed() const { return false; }
void Detector::turnOff() { _powered = true; }

SmokeDetector::SmokeDetector() : Detector("Smoke") {}
Device* SmokeDetector::clone() const { return new SmokeDetector(*this); }

GasDetector::GasDetector() : Detector("Gas") {}
Device* GasDetector::clone() const { return new GasDetector(*this); }

std::vector<Device*> DetectorFactory::createDetectorPair(const SmokeDetector& smokePrototype) {
    std::vector<Device*> out;
    out.push_back(smokePrototype.clone());
    out.push_back(new GasDetector());
    return out;
}
