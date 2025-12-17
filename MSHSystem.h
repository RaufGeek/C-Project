#ifndef MSHSYSTEM_H
#define MSHSYSTEM_H

#include "Device.h"
#include "Memento.h"
#include <vector>
#include <string>

class MSHSystem {
private:
    std::vector<Device*> _devices;
    StateCaretaker _caretaker;

    std::string _currentMode;
    std::string _currentState;
    int _nextId;

    static MSHSystem* _instance;

    MSHSystem();
    MSHSystem(const MSHSystem&);
    MSHSystem& operator=(const MSHSystem&);

    void addDeviceInternal(Device* d);

    void showStatus();
    void addDeviceUI();
    void removeDeviceUI();
    void powerDeviceUI(bool on);
    void changeModeUI();
    void changeStateUI();
    void showManualFromFile() const;

public:
    static MSHSystem* instance();
    ~MSHSystem();

    void run();
    void displayMenu() const;
};

#endif
