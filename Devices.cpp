#include "Devices.h"
#include <iostream>

Light::Light() : Device("Generic Light", "Light"), _brightness(100), _color("White") {}
Device* Light::clone() const { return new Light(*this); }
void Light::configure() {
    std::cout << "Configuring Light...\n";
    std::cout << "Name: "; std::cin >> _name;
    std::cout << "Color: "; std::cin >> _color;
    std::cout << "Brightness (0-100): "; std::cin >> _brightness;
}
void Light::printStatus() const {
    Device::printStatus();
    std::cout << "   -> Color: " << _color << ", Brightness: " << _brightness << "%\n";
}

Camera::Camera() : Sensor("Security Cam", "Camera"), _fps(30), _nightVision(true) { _powered = true; }
Device* Camera::clone() const { return new Camera(*this); }
void Camera::configure() {
    std::cout << "Configuring Camera...\n";
    std::cout << "Name: "; std::cin >> _name;
    std::cout << "FPS: "; std::cin >> _fps;
    char nv; std::cout << "Night Vision (y/n): "; std::cin >> nv;
    _nightVision = (nv == 'y' || nv == 'Y');
}
bool Camera::isUserPowerOffAllowed() const { return false; }
bool Camera::detectMotion() const { return _active && _powered; }

SoundSystem::SoundSystem() : Device("Sound System", "Sound"), _volume(50) {}
Device* SoundSystem::clone() const { return new SoundSystem(*this); }
void SoundSystem::configure() {
    std::cout << "Configuring Sound...\n";
    std::cout << "Name: "; std::cin >> _name;
    std::cout << "Volume: "; std::cin >> _volume;
}
void SoundSystem::printStatus() const {
    Device::printStatus();
    std::cout << "   -> Volume: " << _volume << "\n";
}

TV::TV(const std::string& brand) : Device(brand + " TV", "TV") {}
TV::~TV() {}
void TV::configure() { std::cout << "Name: "; std::cin >> _name; }

SamsungTV::SamsungTV() : TV("Samsung") {}
Device* SamsungTV::clone() const { return new SamsungTV(*this); }

LGTV::LGTV() : TV("LG") {}
Device* LGTV::clone() const { return new LGTV(*this); }

Alarm::Alarm() : Device("General Alarm", "Alarm") { _powered = true; }
Device* Alarm::clone() const { return new Alarm(*this); }
bool Alarm::isUserPowerOffAllowed() const { return false; }
void Alarm::turnOff() { std::cout << "WARNING: Alarm cannot be powered off.\n"; _powered = true; }
void Alarm::trigger() { std::cout << "!!! ALARM TRIGGERED !!!\n"; _powered = true; }
