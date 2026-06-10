#pragma once
#include <string>
#include <vector>
#include "Installable.h"
#include "SystemLogger.h"

class InstallationEngine {
private:
    std::vector<Installable*> allComponents;
    SystemLogger logger;

    Installable* getComponent(const std::string& id);

public:
    InstallationEngine();
    ~InstallationEngine();

    // Non-copyable
    InstallationEngine(const InstallationEngine&)            = delete;
    InstallationEngine& operator=(const InstallationEngine&) = delete;

    void processCommand(std::string line);
};
