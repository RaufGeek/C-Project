// --- Concrete Devices ---

class Light : public Device {
private:
    int brightness;
    string color;
public:
    Light() : Device("Generic Light", "Light"), brightness(100), color("White") {}

    // Prototype Clone
    Device* clone() const { return new Light(*this); }

    void configure() {
        cout << "Configuring Light..." << endl;
        cout << "Name: "; cin >> name;
        cout << "Color: "; cin >> color;
        cout << "Brightness (0-100): "; cin >> brightness;
    }

    void printStatus() const {
        Device::printStatus();
        cout << "   -> Color: " << color << ", Brightness: " << brightness << "%" << endl;
    }
};


class Camera : public Sensor {
private:
    int fps;
    bool nightVision;
public:
    Camera() : Sensor("Security Cam", "Camera"), fps(30), nightVision(true) {
        // Cameras are usually always ON in this scenario
        isPowered = true;
    }

    Device* clone() const { return new Camera(*this); }

    void configure() {
        cout << "Configuring Camera..." << endl;
        cout << "Name: "; cin >> name;
        cout << "FPS: "; cin >> fps;
        char nv;
        cout << "Night Vision (y/n): "; cin >> nv;
        nightVision = (nv == 'y' || nv == 'Y');
    }

    // Simulate motion detection
    bool detectMotion() {
        return isActive && isPowered;
    }
};


// --- TV Product Family ---
class TV : public Device {
public:
    TV(string brand) : Device(brand + " TV", "TV") {}
    virtual ~TV() {}
    void configure() {
         cout << "Name: "; cin >> name;
    }
};

class SamsungTV : public TV {
public:
    SamsungTV() : TV("Samsung") {}
    Device* clone() const { return new SamsungTV(*this); }
};

class LGTV : public TV {
public:
    LGTV() : TV("LG") {}
    Device* clone() const { return new LGTV(*this); }
};