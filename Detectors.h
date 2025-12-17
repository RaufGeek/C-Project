// --- Detectors (MEMBER 4) ---
class Detector : public Sensor {
public:
    Detector(string typeName) : Sensor(typeName + " Detector", "Detector") {
        // Detectors cannot be powered off easily
        isPowered = true;
    }

    // Detectors cannot be powered off by user, override turnOff
    void turnOff() {
        cout << "WARNING: Cannot power off a detector manually!" << endl;
    }
};

class SmokeDetector : public Detector {
public:
    SmokeDetector() : Detector("Smoke") {}
    Device* clone() const { return new SmokeDetector(*this); }
};

class GasDetector : public Detector {
public:
    GasDetector() : Detector("Gas") {}
    Device* clone() const { return new GasDetector(*this); }
};