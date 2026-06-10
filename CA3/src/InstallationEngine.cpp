#include <iostream>
#include "InstallationEngine.h"
#include "Module.h"
#include "Package.h"
#include "Helpers.h"
#include "TransactionContext.h"

InstallationEngine::InstallationEngine() {}

InstallationEngine::~InstallationEngine() {
    for (size_t i = 0; i < allComponents.size(); i++)
        delete allComponents[i];
    allComponents.clear();
}

Installable* InstallationEngine::getComponent(const std::string& id) {
    for (size_t i = 0; i < allComponents.size(); i++)
        if (allComponents[i]->getId() == id) return allComponents[i];
    return nullptr;
}

void InstallationEngine::processCommand(std::string line) {
    std::string cmd = getNextToken(line);

    // ------------------------------------------------------------------ ADD
    if (cmd == "ADD") {
        std::string type  = getNextToken(line);
        std::string id    = getNextToken(line);
        std::string title = trimLeading(line);

        if (type.empty() || id.empty() || title.empty() ||
            (type != "MODULE" && type != "PACKAGE")) {
            std::cout << "ERROR: Invalid command\n"; return;
        }
        if (getComponent(id)) {
            std::cout << "ERROR: Component with ID " << id << " already exists\n"; return;
        }

        Installable* newComp = (type == "MODULE")
            ? static_cast<Installable*>(new Module(id, title))
            : static_cast<Installable*>(new Package(id, title));

        newComp->addObserver(&logger);
        allComponents.push_back(newComp);
    }
    // --------------------------------------------------------------- ATTACH
    else if (cmd == "ATTACH") {
        std::string parentId = getNextToken(line);
        std::string childId  = getNextToken(line);
        if (parentId.empty() || childId.empty()) {
            std::cout << "ERROR: Invalid command\n"; return;
        }

        Installable* parent = getComponent(parentId);
        if (!parent) { std::cout << "ERROR: Component " << parentId << " does not exist\n"; return; }

        Installable* child = getComponent(childId);
        if (!child)  { std::cout << "ERROR: Component " << childId  << " does not exist\n"; return; }

        if (!parent->isPackage()) { std::cout << "ERROR: Cannot attach to a module\n"; return; }

        Package* pkgParent = static_cast<Package*>(parent);
        if (pkgParent->hasChild(childId)) {
            std::cout << "ERROR: Component " << childId << " is already attached to " << parentId << "\n"; return;
        }
        if (parent->getState() == ComponentState::INSTALLED) {
            std::cout << "ERROR: Cannot attach to an already installed package\n"; return;
        }

        pkgParent->addChild(child);
    }
    // ------------------------------------------------------------ MOCK_FAIL
    else if (cmd == "MOCK_FAIL") {
        std::string id = getNextToken(line);
        if (id.empty()) { std::cout << "ERROR: Invalid command\n"; return; }

        Installable* comp = getComponent(id);
        if (!comp) { std::cout << "ERROR: Component " << id << " does not exist\n"; return; }
        if (comp->isMockFail()) {
            std::cout << "ERROR: Component " << id << " is already set to fail\n"; return;
        }
        if (comp->getState() == ComponentState::INSTALLED) {
            std::cout << "ERROR: Component " << id << " is already installed\n"; return;
        }
        comp->setMockFail(true);
    }
    // --------------------------------------------------------- RESOLVE_FAIL
    else if (cmd == "RESOLVE_FAIL") {
        std::string id = getNextToken(line);
        if (id.empty()) { std::cout << "ERROR: Invalid command\n"; return; }

        Installable* comp = getComponent(id);
        if (!comp) { std::cout << "ERROR: Component " << id << " does not exist\n"; return; }
        if (!comp->isMockFail()) {
            std::cout << "ERROR: Component " << id << " is not in a mock fail state\n"; return;
        }
        comp->setMockFail(false);
    }
    // -------------------------------------------------------------- INSTALL
    else if (cmd == "INSTALL") {
        std::string id = getNextToken(line);
        if (id.empty()) { std::cout << "ERROR: Invalid command\n"; return; }

        Installable* comp = getComponent(id);
        if (!comp) { std::cout << "ERROR: Component " << id << " does not exist\n"; return; }
        if (comp->getState() == ComponentState::INSTALLED) {
            std::cout << "ERROR: Component " << id << " is already installed\n"; return;
        }

        comp->setExplicit(true);
        TransactionContext tx;
        if (!comp->install(tx))
            comp->setExplicit(false); // Rollback explicit flag on failure
    }
    // ------------------------------------------------------------ UNINSTALL
    else if (cmd == "UNINSTALL") {
        std::string arg = getNextToken(line);
        if (arg.empty()) { std::cout << "ERROR: Invalid command\n"; return; }

        if (arg == "-A") {
            bool any = false;
            for (size_t i = 0; i < allComponents.size(); i++)
                if (allComponents[i]->getState() != ComponentState::PENDING) { any = true; break; }

            if (!any) { std::cout << "ERROR: No installed components to uninstall\n"; return; }

            for (int i = (int)allComponents.size() - 1; i >= 0; i--)
                if (allComponents[i]->getState() != ComponentState::PENDING)
                    allComponents[i]->forcePending();
        } else {
            Installable* comp = getComponent(arg);
            if (!comp) { std::cout << "ERROR: Component " << arg << " does not exist\n"; return; }
            if (comp->getState() != ComponentState::INSTALLED) {
                std::cout << "ERROR: Component " << arg << " is not currently installed\n"; return;
            }
            if (comp->getInstalledParentsCount() > 0) {
                std::cout << "ERROR: Component " << arg << " is required by another package\n"; return;
            }
            comp->uninstall();
        }
    }
    // ---------------------------------------------------- unknown command
    else if (!cmd.empty()) {
        std::cout << "ERROR: Invalid command\n";
    }
}
