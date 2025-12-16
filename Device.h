#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

/* =========================
   ABSTRACT BASE DEVICE
   ========================= */
class Device {
protected:
    int id;
    string name;
    string type;
    bool isPowered;
    bool isActive;

public:
    Device(int _id, const string& _name, const string& _type)
        : id(_id),
          name(_name),
          type(_type),
          isPowered(false),
          isActive(true) {}

    virtual ~Device() {}

    // Prototype hook (some devices may override)
    virtual Device* clone() const = 0;

    // Device operations
    virtual void turnOn() {
        if (isActive)
            isPowered = true;
    }

    virtual void turnOff() {
        isPowered = false;
    }

    // TEMPLATE METHOD
    virtual void printStatus() const {
        cout << "ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Type: " << type << endl;
        cout << "Power: " << (isPowered ? "ON" : "OFF") << endl;
        printSpecificStatus();
    }

protected:
    // Hook for subclasses
    virtual void printSpecificStatus() const {}
};

/* =========================
   ABSTRACT SENSOR
   ========================= */
class Sensor : public Device {
protected:
    bool triggered;

public:
    Sensor(int _id, const string& _name)
        : Device(_id, _name, "Sensor"),
          triggered(false) {}

    virtual ~Sensor() {}

    virtual void detect() = 0;
};

/* =========================
   ALARM (NON-CLONEABLE)
   ========================= */
class Alarm : public Device {
public:
    Alarm(int _id)
        : Device(_id, "Alarm", "Security") {}

    // Non-cloneable device
    Device* clone() const {
        return NULL;
    }

    void turnOff() {
        cout << "WARNING: Alarm cannot be turned off manually!" << endl;
    }

protected:
    void printSpecificStatus() const {
        cout << "Alarm status: READY" << endl;
    }
};

/* =========================
   FACTORY METHOD (BASE)
   ========================= */
class DeviceFactory {
public:
    static Device* createDevice(const string& type, int id) {
        if (type == "Alarm")
            return new Alarm(id);

        // Future devices added by other members
        return NULL;
    }
};

#endif
