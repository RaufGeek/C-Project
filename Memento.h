#ifndef MEMENTO_H
#define MEMENTO_H

#include "Device.h"
#include <map>
#include <stack>
#include <string>
#include <vector>

class HomeState {
private:
    std::string _modeName;
    std::string _stateName;
    std::map<int, bool> _powerById;

public:
    HomeState(const std::string& mode,
              const std::string& state,
              const std::vector<Device*>& devices);

    const std::string& modeName() const;
    const std::string& stateName() const;

    bool powerFor(int id, bool def) const;
};

class StateCaretaker {
private:
    std::stack<HomeState*> _history;

public:
    ~StateCaretaker();
    void save(HomeState* s);
    HomeState* undo();
    bool hasHistory() const;
};

#endif
