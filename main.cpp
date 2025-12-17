#include <iostream>
#include <string>
#include <vector>

using namespace std;


// CORE CLASSES BY MEMBER 3
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

//END OF MEMBER 3 CORE CLASSES


// Singleton System
class MSHSystem {
private:
    static MSHSystem* instance;
    string currentMode;
    int nextId;

    MSHSystem() : currentMode("Normal"), nextId(1) {}

public:
    static MSHSystem* getInstance() {
        if (instance == NULL) instance = new MSHSystem();
        return instance;
    }

    // Menu Logic (Command Inputs)  --> Member 1 (Integrator)
    void displayMenu() {
        cout << "\n=== MY SWEET HOME (MSH) ===" << endl;

        // Core menu (Integrator responsibility)
        cout << "[1] Get Home Status" << endl;
        cout << "[2] Add Device" << endl;

        // --------------------------------------------------
        // Member 7 - Security & Simulation (Chain of Responsibility)
        // NOTE: These options will be activated after full system
        // integration (devices, Logger, Alarm, etc.)
        // --------------------------------------------------
        cout << "[97] SIMULATE: Device Failure (Member 7)" << endl;
        cout << "[98] SIMULATE: Fire / Smoke Detected (Member 7)" << endl;
        cout << "[99] SIMULATE: Motion Detected (Member 7)" << endl;

        cout << "[10] Shutdown" << endl;
        cout << "Select: ";
    }

    void run() {
        int choice;
        while (true) {
            displayMenu();
            if (!(cin >> choice)) break;

            switch (choice) {

                // Core system commands (Integrator)
                case 1:
                    cout << "TODO(Member1): Get Home Status logic" << endl;
                    break;

                case 2:
                    cout << "TODO(Member1/Member2): Add Device logic" << endl;
                    break;

                // --------------------------------------------------
                // Member 7 - Security & Simulation integration points
                // These functions are implemented by Member 7
                // but will be called by the Integrator after
                // full system integration.
                // --------------------------------------------------
                case 99:
                    cout << "TODO(Integrator): call securitySequence()" << endl;
                    // securitySequence();
                    break;

                case 98:
                    cout << "TODO(Integrator): call fireSequence()" << endl;
                    // fireSequence();
                    break;

                case 97:
                    cout << "TODO(Integrator): call simulateFailure()" << endl;
                    // simulateFailure();
                    break;

                case 10:
                    return;

                default:
                    cout << "Invalid option." << endl;
                    break;
            }
        }
    }

    #if 0
    // CORE FUNCTIONS BY MEMBER 7
    // NOTE: Requires integrated system (devices, Logger, Alarm). Keep disabled in skeleton build. 
    // After integration delete "#if 0", "#endif" and SwitchCase 'notes text' (especially if it is function)!

    // --- Scenarios (Req 13, 14, 15, 16) ---

    // Security: Camera Motion -> Alarm -> Light -> Police
    void securitySequence() {
        Logger::getInstance()->log("EVENT: Motion Detected!");
        cout << "\n*** SECURITY ALERT ***" << endl;

        // 1. Alarm triggers
        for (size_t i = 0; i < devices.size(); ++i) {
            if (devices[i]->getType() == "Alarm") ((Alarm*)devices[i])->trigger();
        }

        // Simulate wait (in real app, use sleep)
        cout << "(Alarming for 3 seconds...)" << endl;

        // 2. Lights ON
        cout << "Turning ALL Lights ON..." << endl;
        for (size_t i = 0; i < devices.size(); ++i) {
            if (devices[i]->getType() == "Light") devices[i]->turnOn();
        }

        // 3. Call Police
        cout << "DIALING POLICE..." << endl;
        Logger::getInstance()->log("ACTION: Police Called");
    }

    // Fire: Detector -> Alarm -> Ack? -> Lights Toggle -> Fire Station
    void fireSequence() {
        Logger::getInstance()->log("EVENT: Smoke/Gas Detected!");
        cout << "\n*** FIRE ALERT ***" << endl;

        // 1. Alarm
        for (size_t i = 0; i < devices.size(); ++i) {
            if (devices[i]->getType() == "Alarm") ((Alarm*)devices[i])->trigger();
        }

        // 2. Wait for Ack
        cout << "Press 'k' to acknowledge alarm (you have 3 seconds simulated): ";
        char k; 
        // Simple simulation of user ignoring it:
        // In a real CLI, we can't do non-blocking input easily in portable C++98
        // So we ask the user what they want to simulate
        cout << "\n[SIMULATION] Do you acknowledge? (y/n): ";
        cin >> k;

        if (k == 'y' || k == 'Y') {
            cout << "Alarm acknowledged. Stopping sequence." << endl;
            for (size_t i = 0; i < devices.size(); ++i) {
                if (devices[i]->getType() == "Alarm") ((Alarm*)devices[i])->turnOff();
            }
            return;
        }

        // 3. Not acknowledged: Toggle Lights
        Logger::getInstance()->log("ALERT: User did not Ack. Toggling Lights.");
        cout << "Toggling Lights ON/OFF..." << endl;

        // 4. Call Fire Station
        cout << "DIALING FIRE STATION..." << endl;
        Logger::getInstance()->log("ACTION: Fire Station Called");
    }

    // Failure Simulation (Req 9)
    void simulateFailure() {
        int id;
        cout << "Enter Device ID to break: "; cin >> id;
        for (size_t i = 0; i < devices.size(); ++i) {
            if (devices[i]->getId() == id) {
                devices[i]->setBroken(true);
                Logger::getInstance()->log("FAILURE: Device " + to_string(id) + " failed.");

                // Notification preference (mocked)
                cout << "A SMS is sent (Device " << id << " failure)." << endl;
            }
        }
    }
    #endif
};

MSHSystem* MSHSystem::instance = NULL;

int main() {
    MSHSystem::getInstance()->run();
    return 0;
}