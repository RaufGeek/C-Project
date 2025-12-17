#include "Memento.h"

HomeState::HomeState(const std::string& mode,
                     const std::string& state,
                     const std::vector<Device*>& devices)
    : _modeName(mode), _stateName(state) {
    for (size_t i = 0; i < devices.size(); ++i)
        _powerById[devices[i]->getId()] = devices[i]->isPowered();
}

const std::string& HomeState::modeName() const { return _modeName; }
const std::string& HomeState::stateName() const { return _stateName; }

bool HomeState::powerFor(int id, bool def) const {
    std::map<int,bool>::const_iterator it = _powerById.find(id);
    if (it == _powerById.end()) return def;
    return it->second;
}

StateCaretaker::~StateCaretaker() {
    while (!_history.empty()) {
        delete _history.top();
        _history.pop();
    }
}

void StateCaretaker::save(HomeState* s) { _history.push(s); }

HomeState* StateCaretaker::undo() {
    if (_history.empty()) return 0;
    HomeState* s = _history.top();
    _history.pop();
    return s;
}

bool StateCaretaker::hasHistory() const { return !_history.empty(); }
