#ifndef BASE_DEVICE_H
#define BASE_DEVICE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ==========================================
// BASE DEVICE CLASS (Template Method Pattern)
// ==========================================
class Device {
protected:
    int id;
    string name;
    string type;
    bool isPowered;
    bool isActive; // Tracks if the device is operational or "Broken"

public:
    // Constructor
    Device(string n, string t) : id(0), name(n), type(t), isPowered(false), isActive(true) {}
    
    // Virtual Destructor for safe inheritance
    virtual ~Device() {}

    // Pure Virtual Function: Prototype Link (To be implemented by Member 3)
    virtual Device* clone() const = 0;

    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    string getType() const { return type; }
    bool getPower() const { return isPowered; }
    bool getActive() const { return isActive; }

    // Setters
    void setId(int newId) { id = newId; }
    void setBroken(bool broken) { isActive = !broken; }

    // Core Logic: Power Management
    virtual void turnOn() { 
        if (!isActive) {
            cout << "Error: " << name << " is broken and cannot be turned on." << endl;
            return;
        }
        isPowered = true; 
    }

    virtual void turnOff() { 
        isPowered = false; 
    }

    // Template Method: Defines the structure of status reporting
    virtual void printStatus() const {
        cout << "[" << type << "] ID: " << id 
             << " | Name: " << name 
             << " | Power: " << (isPowered ? "ON" : "OFF") 
             << " | Status: " << (isActive ? "OK" : "BROKEN");
        
        // Custom data for specific devices will be printed after this in children
        cout << endl;
    }

    // Pure Virtual: Configuration Hook
    virtual void configure() = 0;
};

// ==========================================
// BASE SENSOR CLASS
// ==========================================
class Sensor : public Device {
public:
    Sensor(string n, string t) : Device(n, t) {}
    virtual ~Sensor() {}
    
    // Sensors might have specific shared behavior (like constant power)
    virtual void turnOff() {
        cout << "Note: " << name << " is a security sensor. Powering off is restricted." << endl;
        Device::turnOff();
    }
};

#endif