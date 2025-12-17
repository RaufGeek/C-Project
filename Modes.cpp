#include "Modes.h"

std::string Modes::parseMode(char m) {
    switch (m) {
        case 'N': case 'n': return "Normal";
        case 'E': case 'e': return "Evening";
        case 'P': case 'p': return "Party";
        case 'C': case 'c': return "Cinema";
        default: return "";
    }
}

std::string Modes::parseState(char s) {
    switch (s) {
        case 'N': case 'n': return "Normal";
        case 'S': case 's': return "Sleep";
        case 'H': case 'h': return "High Performance";
        default: return "Normal";
    }
}

void Modes::applyMode(std::vector<Device*>& devices, const std::string& mode) {
    for (size_t i = 0; i < devices.size(); ++i) {
        const std::string& type = devices[i]->getType();
        if (type == "Detector" || type == "Camera" || type == "Alarm") {
            devices[i]->turnOn();
            continue;
        }

        if (mode == "Normal") {
            if (type == "Light") devices[i]->turnOn();
            if (type == "TV") devices[i]->turnOff();
            if (type == "Sound") devices[i]->turnOff();
        } else if (mode == "Evening") {
            if (type == "Light") devices[i]->turnOff();
            if (type == "TV") devices[i]->turnOff();
            if (type == "Sound") devices[i]->turnOff();
        } else if (mode == "Party") {
            if (type == "Light") devices[i]->turnOn();
            if (type == "TV") devices[i]->turnOff();
            if (type == "Sound") devices[i]->turnOn();
        } else if (mode == "Cinema") {
            if (type == "Light") devices[i]->turnOff();
            if (type == "TV") devices[i]->turnOn();
            if (type == "Sound") devices[i]->turnOff();
        }
    }
}
