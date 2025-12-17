#include "MSHSystem.h"

#include "Logger.h"
#include "Devices.h"
#include "DetectorFactory.h"
#include "Modes.h"
#include "Scenarios.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

static std::string to_string_int(int v) {
    std::ostringstream os;
    os << v;
    return os.str();
}

MSHSystem* MSHSystem::_instance = 0;

MSHSystem::MSHSystem()
    : _currentMode("Normal"),
      _currentState("Normal"),
      _nextId(1) {
    addDeviceInternal(new Alarm());
}

MSHSystem::~MSHSystem() {
    for (size_t i = 0; i < _devices.size(); ++i) delete _devices[i];
    _devices.clear();
    Logger::instance().close();
}

MSHSystem* MSHSystem::instance() {
    if (_instance == 0) _instance = new MSHSystem();
    return _instance;
}

void MSHSystem::addDeviceInternal(Device* d) {
    d->setId(_nextId++);
    _devices.push_back(d);
    Logger::instance().log("Device added: " + d->getName() + " (ID: " + to_string_int(d->getId()) + ")");
}

void MSHSystem::showManualFromFile() const {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif

    std::ifstream f("manual.txt");
    if (!f.is_open()) {
        std::cout << "manual.txt not found.\n";
        return;
    }
    std::string line;
    while (std::getline(f, line)) std::cout << line << "\n";
}

void MSHSystem::displayMenu() const {
    std::cout << "\n=== MY SWEET HOME (MSH) ===\n";
    std::cout << "[1] Get Home Status\n";
    std::cout << "[2] Add Device\n";
    std::cout << "[3] Remove Device\n";
    std::cout << "[4] Power ON Device\n";
    std::cout << "[5] Power OFF Device\n";
    std::cout << "[6] Change Mode (Normal/Evening/Party/Cinema)\n";
    std::cout << "[7] Change State (Normal/Sleep/High Perf/Previous)\n";
    std::cout << "[8] Manual\n";
    std::cout << "[9] About\n";
    std::cout << "[10] Shutdown\n";
    std::cout << "[99] SIMULATE: Motion Detected\n";
    std::cout << "[98] SIMULATE: Smoke/Gas Detected\n";
    std::cout << "[97] SIMULATE: Device Failure\n";
    std::cout << "Select: ";
}

void MSHSystem::run() {
    int choice;
    while (true) {
        displayMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        switch (choice) {
            case 1: showStatus(); break;
            case 2: addDeviceUI(); break;
            case 3: removeDeviceUI(); break;
            case 4: powerDeviceUI(true); break;
            case 5: powerDeviceUI(false); break;
            case 6: changeModeUI(); break;
            case 7: changeStateUI(); break;
            case 8: showManualFromFile(); break;
            case 9: std::cout << "\n--- ABOUT ---\nMSH v1.0\n"; break;
            case 10: return;
            case 99: Scenarios::securitySequence(_devices); break;
            case 98: Scenarios::fireSequence(_devices); break;
            case 97: Scenarios::simulateFailure(_devices); break;
            default: std::cout << "Invalid option.\n";
        }
    }
}

void MSHSystem::showStatus() {
    std::cout << "\n--- HOME STATUS ---\n";
    std::cout << "Mode: " << _currentMode << " | State: " << _currentState << "\n";
    for (size_t i = 0; i < _devices.size(); ++i) _devices[i]->printStatus();
}

void MSHSystem::addDeviceUI() {
    std::cout << "Type? (L)ight, (C)amera, (T)V, (D)etector Pair, (S)ound: ";
    char t; std::cin >> t;
    int qty;
    std::cout << "Quantity: "; std::cin >> qty;
    if (qty <= 0) return;

    if (t == 'L' || t == 'l') {
        Light proto; proto.configure();
        for (int i = 0; i < qty; ++i) addDeviceInternal(proto.clone());
        return;
    }
    if (t == 'C' || t == 'c') {
        Camera proto; proto.configure();
        for (int i = 0; i < qty; ++i) addDeviceInternal(proto.clone());
        return;
    }
    if (t == 'S' || t == 's') {
        SoundSystem proto; proto.configure();
        for (int i = 0; i < qty; ++i) addDeviceInternal(proto.clone());
        return;
    }
    if (t == 'T' || t == 't') {
        std::cout << "Brand? (1) Samsung, (2) LG: ";
        int b; std::cin >> b;
        Device* tv = (b == 1) ? (Device*)new SamsungTV() : (Device*)new LGTV();
        tv->configure();
        addDeviceInternal(tv);
        for (int i = 1; i < qty; ++i) addDeviceInternal(tv->clone());
        return;
    }
    if (t == 'D' || t == 'd') {
        SmokeDetector smokeProto;
        smokeProto.configure();
        for (int i = 0; i < qty; ++i) {
            std::vector<Device*> pair = DetectorFactory::createDetectorPair(smokeProto);
            for (size_t k = 0; k < pair.size(); ++k) addDeviceInternal(pair[k]);
        }
        return;
    }
}

void MSHSystem::removeDeviceUI() {
    int id;
    std::cout << "Enter ID: ";
    std::cin >> id;

    for (std::vector<Device*>::iterator it = _devices.begin(); it != _devices.end(); ++it) {
        if ((*it)->getId() == id) {
            if ((*it)->getType() == "Alarm") {
                std::cout << "Cannot remove Alarm.\n";
                return;
            }
            delete *it;
            _devices.erase(it);
            std::cout << "Removed.\n";
            return;
        }
    }
    std::cout << "Not found.\n";
}

void MSHSystem::powerDeviceUI(bool on) {
    int id;
    std::cout << "Enter ID: ";
    std::cin >> id;

    for (size_t i = 0; i < _devices.size(); ++i) {
        if (_devices[i]->getId() == id) {
            if (!on && !_devices[i]->isUserPowerOffAllowed()) {
                _devices[i]->turnOff();
                return;
            }
            if (on) _devices[i]->turnOn();
            else _devices[i]->turnOff();
            return;
        }
    }
    std::cout << "Not found.\n";
}

void MSHSystem::changeModeUI() {
    std::cout << "Mode: (N)ormal (E)vening (P)arty (C)inema: ";
    char m; std::cin >> m;

    std::string newMode = Modes::parseMode(m);
    if (newMode.empty()) return;

    _currentMode = newMode;
    Logger::instance().log("Mode changed to " + newMode);
    Modes::applyMode(_devices, newMode);
}

void MSHSystem::changeStateUI() {
    std::cout << "State: (N)ormal (S)leep (H)ighPerf (P)revious: ";
    char s; std::cin >> s;

    if (s == 'P' || s == 'p') {
        HomeState* prev = _caretaker.undo();
        if (!prev) { std::cout << "No previous state!\n"; return; }

        _currentState = prev->stateName();
        _currentMode  = prev->modeName();

        for (size_t i = 0; i < _devices.size(); ++i) {
            bool desired = prev->powerFor(_devices[i]->getId(), _devices[i]->isPowered());
            if (!_devices[i]->isUserPowerOffAllowed()) _devices[i]->turnOn();
            else { if (desired) _devices[i]->turnOn(); else _devices[i]->turnOff(); }
        }
        delete prev;
        return;
    }

    _caretaker.save(new HomeState(_currentMode, _currentState, _devices));
    _currentState = Modes::parseState(s);
    Logger::instance().log("State changed to " + _currentState);
}
