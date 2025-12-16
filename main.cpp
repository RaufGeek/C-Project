/*
 * CENG-464 Term Project: My Sweet Home (MSH)
 * Technologies: C++98, Design Patterns
 * * Design Patterns Used:
 * 1. Singleton (MSHSystem, Logger)
 * 2. Prototype (Device Cloning for batch addition)
 * 3. Memento (State History/Undo)
 * 4. Factory Method (Implied in addDevice logic)
 * * Note: This code uses C++98 standards (NULL instead of nullptr, 
 * verbose iterators, manual memory management).
 */

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <stack>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <sstream>

using namespace std;

// --- Helper for C++98 String Conversion ---
template <typename T>
string to_string(T value) {
    ostringstream os;
    os << value;
    return os.str();
}

// ==========================================
// 1. LOGGER (Singleton)
// ==========================================
class Logger {
private:
    static Logger* instance;
    ofstream logFile;

    Logger() {
        logFile.open("msh_log.txt", ios::app);
        log("--- System Started ---");
    }

public:
    static Logger* getInstance() {
        if (instance == NULL) {
            instance = new Logger();
        }
        return instance;
    }

    void log(string message) {
        time_t now = time(0);
        char* dt = ctime(&now);
        string timeStr(dt);
        // Remove newline from ctime
        timeStr = timeStr.substr(0, timeStr.length() - 1);

        string logEntry = "[" + timeStr + "] " + message;
        if (logFile.is_open()) {
            logFile << logEntry << endl;
        }
        // Also print to console for user visibility
        // cout << "(LOG: " << message << ")" << endl; 
    }

    ~Logger() {
        if (logFile.is_open()) {
            log("--- System Shutdown ---");
            logFile.close();
        }
    }
};

Logger* Logger::instance = NULL;


// ==========================================
// 2. DEVICE HIERARCHY (Prototype Pattern)
// ==========================================

class Device {
protected:
    int id;
    string name;
    string type;
    bool isPowered;
    bool isActive; // true = working, false = broken

public:
    Device(string n, string t) : id(0), name(n), type(t), isPowered(false), isActive(true) {}
    virtual ~Device() {}

    // Prototype Pattern: Pure virtual clone
    virtual Device* clone() const = 0;

    void setId(int newId) { id = newId; }
    int getId() const { return id; }
    string getName() const { return name; }
    string getType() const { return type; }
    bool getPower() const { return isPowered; }
    bool getActive() const { return isActive; }

    virtual void turnOn() { 
        if (!isActive) {
            cout << "Cannot power on " << name << " (Broken/Inactive)." << endl;
            return;
        }
        isPowered = true; 
    }

    virtual void turnOff() { isPowered = false; }

    void setBroken(bool broken) { isActive = !broken; }

    virtual void printStatus() const {
        cout << "[" << type << "] ID: " << id << " | Name: " << name 
             << " | Power: " << (isPowered ? "ON" : "OFF") 
             << " | Status: " << (isActive ? "OK" : "BROKEN") << endl;
    }

    // Allow manual configuration update (for cloning source)
    virtual void configure() {
        cout << "Enter name for " << type << ": ";
        cin >> name;
    }
};

// --- Sensor Interface ---
class Sensor : public Device {
public:
    Sensor(string n, string t) : Device(n, t) {}
    virtual ~Sensor() {}
};

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

// --- Detectors ---
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

// --- Alarm (Singleton-like actuator, but we can have multiple) ---
class Alarm : public Device {
public:
    Alarm() : Device("General Alarm", "Alarm") {
        isPowered = true; 
    }

    Device* clone() const {
        // Alarms might not be cloneable in this scenario, but we implement it for safety
        return new Alarm(*this);
    }

    void turnOff() { cout << "Alarm silenced." << endl; isPowered = false; }
    void trigger() { 
        cout << "!!! ALARM TRIGGERED !!!" << endl; 
        isPowered = true;
    }
};


// ==========================================
// 3. MEMENTO PATTERN (State Management)
// ==========================================

class HomeState {
private:
    string modeName;
    string stateName;
    map<int, bool> devicePowerStates;

public:
    HomeState(string m, string s, const vector<Device*>& devices) {
        modeName = m;
        stateName = s;
        // Save power state of every device by ID
        for (size_t i = 0; i < devices.size(); ++i) {
            if (devices[i]) {
                devicePowerStates[devices[i]->getId()] = devices[i]->getPower();
            }
        }
    }

    string getModeName() const { return modeName; }
    string getStateName() const { return stateName; }
    bool getPowerState(int id) {
        if (devicePowerStates.find(id) != devicePowerStates.end()) {
            return devicePowerStates[id];
        }
        return false;
    }
};

class StateCaretaker {
private:
    stack<HomeState*> history;

public:
    void saveState(HomeState* state) {
        history.push(state);
    }

    HomeState* undo() {
        if (history.empty()) return NULL;
        HomeState* s = history.top();
        history.pop();
        return s;
    }

    bool hasHistory() { return !history.empty(); }
};


// ==========================================
// 4. MSH SYSTEM (Singleton & Facade)
// ==========================================

class MSHSystem {
private:
    static MSHSystem* instance;
    vector<Device*> devices;
    StateCaretaker caretaker;
    string currentMode;
    string currentState;
    int nextId;

    MSHSystem() {
        nextId = 1;
        currentMode = "Normal";
        currentState = "Normal";
        // Initialize with default Alarm
        addDeviceInternal(new Alarm());
    }

    void addDeviceInternal(Device* d) {
        d->setId(nextId++);
        devices.push_back(d);
        Logger::getInstance()->log("Device added: " + d->getName() + " (ID: " + to_string(d->getId()) + ")");
    }

public:
    static MSHSystem* getInstance() {
        if (instance == NULL) instance = new MSHSystem();
        return instance;
    }

    ~MSHSystem() {
        for (size_t i = 0; i < devices.size(); ++i) {
            delete devices[i];
        }
        devices.clear();
    }

    // --- Menu & Input ---
    void displayMenu() {
        cout << "\n=== MY SWEET HOME (MSH) ===" << endl;
        cout << "[1] Get Home Status" << endl;
        cout << "[2] Add Device" << endl;
        cout << "[3] Remove Device" << endl;
        cout << "[4] Power ON Device" << endl;
        cout << "[5] Power OFF Device" << endl;
        cout << "[6] Change Mode (Normal, Evening, Party, Cinema)" << endl;
        cout << "[7] Change State (Normal, Sleep, High Perf, Previous)" << endl;
        cout << "[8] Manual" << endl;
        cout << "[9] About" << endl;
        cout << "[10] Shutdown" << endl;
        cout << "[99] SIMULATE: Motion Detected" << endl;
        cout << "[98] SIMULATE: Smoke Detected" << endl;
        cout << "[97] SIMULATE: Device Failure" << endl;
        cout << "Select: ";
    }

    void run() {
        int choice;
        while (true) {
            displayMenu();
            if (!(cin >> choice)) {
                cin.clear(); cin.ignore(1000, '\n');
                continue;
            }

            Logger::getInstance()->log("User Input: " + to_string(choice));

            switch (choice) {
                case 1: showStatus(); break;
                case 2: addDeviceUI(); break;
                case 3: removeDeviceUI(); break;
                case 4: powerDeviceUI(true); break;
                case 5: powerDeviceUI(false); break;
                case 6: changeModeUI(); break;
                case 7: changeStateUI(); break;
                case 8: cout << "\n--- MANUAL ---\nUse numbers to navigate.\nDetectors cannot be turned off.\n"; break;
                case 9: cout << "\n--- ABOUT ---\nMSH v1.0\nDevelopers: Team 1\n"; break;
                case 10: return;
                case 99: securitySequence(); break;
                case 98: fireSequence(); break;
                case 97: simulateFailure(); break;
                default: cout << "Invalid option." << endl;
            }
        }
    }

    // --- Device Management (Req 5, 8, 10) ---
    void addDeviceUI() {
        cout << "Type? (L)ight, (C)amera, (T)V, (D)etector: ";
        char type; cin >> type;
        int qty;
        cout << "Quantity: "; cin >> qty;

        Device* prototype = NULL;
        bool isPair = false; // For Smoke & Gas

        if (type == 'L' || type == 'l') prototype = new Light();
        else if (type == 'C' || type == 'c') prototype = new Camera();
        else if (type == 'T' || type == 't') {
            cout << "Brand? (1) Samsung, (2) LG: ";
            int b; cin >> b;
            if (b == 1) prototype = new SamsungTV();
            else prototype = new LGTV();
        } 
        else if (type == 'D' || type == 'd') {
            // Detectors are sold together
            isPair = true;
            // We'll use Smoke as primary prototype to config name
            prototype = new SmokeDetector();
        } 
        else {
            cout << "Unknown type." << endl;
            return;
        }

        // Configure the first one
        prototype->configure();

        // Add the first one (or pair)
        if (isPair) {
             addDeviceInternal(prototype); // Smoke
             addDeviceInternal(new GasDetector()); // Gas (default config)
             qty--; // Decrement because we added the prototype
        } else {
             addDeviceInternal(prototype);
             qty--;
        }

        // Clone the rest
        for (int i = 0; i < qty; ++i) {
            Device* clone = prototype->clone();
            addDeviceInternal(clone);
            if (isPair) addDeviceInternal(new GasDetector()); // Add pair for clones too
        }

        cout << "Devices added." << endl;
    }

    void removeDeviceUI() {
        int id;
        cout << "Enter ID to remove: "; cin >> id;
        for (vector<Device*>::iterator it = devices.begin(); it != devices.end(); ++it) {
            if ((*it)->getId() == id) {
                Logger::getInstance()->log("Removing device " + to_string(id));
                delete *it;
                devices.erase(it);
                cout << "Device removed." << endl;
                return;
            }
        }
        cout << "ID not found." << endl;
    }

    // --- Power (Req 6) ---
    void powerDeviceUI(bool on) {
        int id;
        cout << "Enter Device ID: "; cin >> id;
        for (size_t i = 0; i < devices.size(); ++i) {
            if (devices[i]->getId() == id) {
                if (on) devices[i]->turnOn();
                else devices[i]->turnOff();
                Logger::getInstance()->log("Device " + to_string(id) + (on ? " Powered ON" : " Powered OFF"));
                return;
            }
        }
    }

    // --- Status (Req 1) ---
    void showStatus() {
        cout << "\n--- HOME STATUS ---" << endl;
        cout << "Mode: " << currentMode << " | State: " << currentState << endl;
        for (size_t i = 0; i < devices.size(); ++i) {
            devices[i]->printStatus();
        }
    }

    // --- Modes (Req 7) ---
    void changeModeUI() {
        cout << "Select Mode: (N)ormal, (E)vening, (P)arty, (C)inema: ";
        char m; cin >> m;
        string newMode;

        // Save State before changing (implied, though usually done in Change State)

        switch(m) {
            case 'N': case 'n': newMode = "Normal"; break;
            case 'E': case 'e': newMode = "Evening"; break;
            case 'P': case 'p': newMode = "Party"; break;
            case 'C': case 'c': newMode = "Cinema"; break;
            default: return;
        }

        currentMode = newMode;
        Logger::getInstance()->log("Mode changed to " + newMode);
        applyModeLogic(newMode);
    }

    void applyModeLogic(string mode) {
        // Strategy Logic embedded here for simplicity
        for (size_t i = 0; i < devices.size(); ++i) {
            string type = devices[i]->getType();
            if (type == "Detector" || type == "Camera" || type == "Alarm") continue; // Always ON

            if (mode == "Normal") {
                if (type == "Light") devices[i]->turnOn();
                if (type == "TV") devices[i]->turnOff();
            } else if (mode == "Evening") {
                if (type == "Light") devices[i]->turnOff();
                if (type == "TV") devices[i]->turnOff();
            } else if (mode == "Party") {
                if (type == "Light") devices[i]->turnOn();
                if (type == "TV") devices[i]->turnOff();
                // Music ON (not implemented in class list, implied)
            } else if (mode == "Cinema") {
                if (type == "Light") devices[i]->turnOff();
                if (type == "TV") devices[i]->turnOn();
            }
        }
    }

    // --- States (Req 11, 12) ---
    void changeStateUI() {
        cout << "Select State: (N)ormal, (H)igh Perf, (S)leep, (P)revious: ";
        char s; cin >> s;

        if (s == 'P' || s == 'p') {
            // Restore Memento
            HomeState* prev = caretaker.undo();
            if (prev) {
                currentState = prev->getStateName();
                currentMode = prev->getModeName();
                // Restore power states
                for (size_t i = 0; i < devices.size(); ++i) {
                    bool p = prev->getPowerState(devices[i]->getId());
                    if (p) devices[i]->turnOn(); else devices[i]->turnOff();
                }
                Logger::getInstance()->log("State restored to " + currentState);
                delete prev;
            } else {
                cout << "No previous state!" << endl;
            }
        } else {
            // Save current state
            caretaker.saveState(new HomeState(currentMode, currentState, devices));

            if (s == 'S' || s == 's') currentState = "Sleep";
            else if (s == 'H' || s == 'h') currentState = "High Performance";
            else currentState = "Normal";

            Logger::getInstance()->log("State changed to " + currentState);
        }
    }

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
};

MSHSystem* MSHSystem::instance = NULL;

// ==========================================
// MAIN
// ==========================================
int main() {
    MSHSystem* app = MSHSystem::getInstance();
    app->run();
    return 0;
}