#include "Module.h"

Module::Module(std::string id, std::string title)
    : Installable(id, title)
{}

bool Module::install(TransactionContext& tx) {
    if (state == ComponentState::INSTALLED) return true;

    if (mockFail) {
        setState(ComponentState::FAILED);
        return false;
    }

    setState(ComponentState::INSTALLED);
    tx.stateChangedNodes.push_back(this);
    return true;
}

void Module::uninstall() {
    if (state != ComponentState::PENDING) {
        setState(ComponentState::PENDING);
        isExplicitlyInstalled = false;
    }
}
