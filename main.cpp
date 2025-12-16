#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Singleton System
class MSHSystem {
private:
    static MSHSystem* instance;
    string currentMode;
    int nextId;

    MSHSystem() : currentMode("Normal"), nextId(1) {}

public:
    static MSHSystem* getInstance() {
        if (instance == NULL) instance = new MSHSystem();
        return instance;
    }

    // Menu Logic (Command Inputs)
    void displayMenu() {
        cout << "\n=== MY SWEET HOME (MSH) ===" << endl;
        cout << "[1] Get Home Status\n[2] Add Device\n[10] Shutdown" << endl;
        cout << "Select: ";
    }

    void run() {
        int choice;
        while (true) {
            displayMenu();
            if (!(cin >> choice)) break;
            if (choice == 10) return; 
            cout << "Executing Command: " << choice << endl;
        }
    }
};

MSHSystem* MSHSystem::instance = NULL;

int main() {
    MSHSystem::getInstance()->run();
    return 0;
}