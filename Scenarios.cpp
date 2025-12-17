#include "Scenarios.h"
#include "Devices.h"
#include "Logger.h"
#include <iostream>

#ifdef _WIN32
  #include <windows.h>
  static void sleepSeconds(int s){ Sleep(s*1000); }
#else
  #include <unistd.h>
  static void sleepSeconds(int s){ sleep(s); }
#endif

namespace Scenarios {

static void triggerAlarms(std::vector<Device*>& devs){
    for(size_t i=0;i<devs.size();++i)
        if(devs[i]->getType()=="Alarm") ((Alarm*)devs[i])->trigger();
}
static void lights(std::vector<Device*>& devs,bool on){
    for(size_t i=0;i<devs.size();++i)
        if(devs[i]->getType()=="Light") { if(on) devs[i]->turnOn(); else devs[i]->turnOff(); }
}

void securitySequence(std::vector<Device*>& devs){
    Logger::instance().log("EVENT: Motion Detected");
    std::cout<<"\n*** SECURITY ALERT ***\n";
    triggerAlarms(devs);
    sleepSeconds(2);
    std::cout<<"Turning lights ON...\n";
    lights(devs,true);
    std::cout<<"CALLING POLICE...\n";
}

void fireSequence(std::vector<Device*>& devs){
    Logger::instance().log("EVENT: Smoke/Gas Detected");
    std::cout<<"\n*** FIRE ALERT ***\n";
    triggerAlarms(devs);
    std::cout<<"Acknowledge? (y/n): ";
    char a; std::cin>>a;
    if(a=='y'||a=='Y'){ std::cout<<"Acknowledged.\n"; return; }

    for(int i=0;i<6;++i){
        lights(devs,(i%2==0));
        sleepSeconds(1);
    }
    std::cout<<"CALLING FIRE STATION...\n";
}

void simulateFailure(std::vector<Device*>& devs){
    int id;
    std::cout<<"Device ID to break: ";
    std::cin>>id;
    for(size_t i=0;i<devs.size();++i){
        if(devs[i]->getId()==id){
            devs[i]->setBroken(true);
            std::cout<<"SMS sent for device "<<id<<"\n";
            return;
        }
    }
    std::cout<<"Not found.\n";
}

}
