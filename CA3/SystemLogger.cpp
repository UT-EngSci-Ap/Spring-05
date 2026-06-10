#include <iostream>
#include "SystemLogger.h"
#include "Installable.h"

void SystemLogger::onStateChanged(const Installable* comp,
                                  ComponentState oldState,
                                  ComponentState newState) {
    std::cout << "[OBSERVER] Component " << comp->getId()
              << " changed from " << stateToString(oldState)
              << " to "           << stateToString(newState) << std::endl;
}
