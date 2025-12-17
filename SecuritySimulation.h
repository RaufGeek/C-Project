#pragma once

// Member 7 - Security & Simulation
// Pattern: Chain of Responsibility (Sequential Events)
// This file contains the emergency automation sequences
// extracted from main.cpp as individual responsibility.

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
