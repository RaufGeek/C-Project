#ifndef DEVICES_H
#define DEVICES_H

#include "Device.h"
#include <string>

class Light : public Device {
private:
    int _brightness;
    std::string _color;

public:
    Light();
    virtual Device* clone() const;
    virtual void configure();
    virtual void printStatus() const;
};

class Camera : public Sensor {
private:
    int _fps;
    bool _nightVision;

public:
    Camera();
    virtual Device* clone() const;
    virtual void configure();
    virtual bool isUserPowerOffAllowed() const;
    bool detectMotion() const;
};

class SoundSystem : public Device {
private:
    int _volume;

public:
    SoundSystem();
    virtual Device* clone() const;
    virtual void configure();
    virtual void printStatus() const;
};

class TV : public Device {
public:
    TV(const std::string& brand);
    virtual ~TV();
    virtual void configure();
};

class SamsungTV : public TV {
public:
    SamsungTV();
    virtual Device* clone() const;
};

class LGTV : public TV {
public:
    LGTV();
    virtual Device* clone() const;
};

class Alarm : public Device {
public:
    Alarm();
    virtual Device* clone() const;
    virtual bool isUserPowerOffAllowed() const;
    virtual void turnOff();
    void trigger();
};

#endif
