#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include <iostream>

class Device {
protected:
    int _id;
    std::string _name;
    std::string _type;
    bool _powered;
    bool _active;

public:
    Device(const std::string& name, const std::string& type)
        : _id(0), _name(name), _type(type), _powered(false), _active(true) {}

    virtual ~Device() {}
    virtual Device* clone() const = 0;

    void setId(int id) { _id = id; }
    int getId() const { return _id; }

    const std::string& getName() const { return _name; }
    const std::string& getType() const { return _type; }

    bool isPowered() const { return _powered; }
    bool isActive() const { return _active; }

    virtual void turnOn() {
        if (!_active) {
            std::cout << "Cannot power on " << _name << " (Broken/Inactive).\n";
            return;
        }
        _powered = true;
    }

    virtual void turnOff() { _powered = false; }
    virtual bool isUserPowerOffAllowed() const { return true; }

    void setBroken(bool broken) { _active = !broken; }

    virtual void configure() {
        std::cout << "Enter name for " << _type << ": ";
        std::cin >> _name;
    }

    virtual void printStatus() const {
        std::cout << "[" << _type << "] ID: " << _id
                  << " | Name: " << _name
                  << " | Power: " << (_powered ? "ON" : "OFF")
                  << " | Status: " << (_active ? "OK" : "BROKEN")
                  << "\n";
    }
};

class Sensor : public Device {
public:
    Sensor(const std::string& name, const std::string& type)
        : Device(name, type) {}
    virtual ~Sensor() {}
};

#endif
