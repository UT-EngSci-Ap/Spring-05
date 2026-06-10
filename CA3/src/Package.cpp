#include "Package.h"

Package::Package(std::string id, std::string title)
    : Installable(id, title)
{}

bool Package::isPackage() const { return true; }

void Package::addChild(Installable* child) {
    children.push_back(child);
}

bool Package::hasChild(const std::string& childId) const {
    for (size_t i = 0; i < children.size(); i++)
        if (children[i]->getId() == childId) return true;
    return false;
}

bool Package::install(TransactionContext& tx) {
    if (state == ComponentState::INSTALLED) return true;

    if (mockFail) {
        setState(ComponentState::FAILED);
        return false;
    }

    size_t initialChangedSize = tx.stateChangedNodes.size();
    size_t initialCountSize   = tx.countIncreasedNodes.size();
    bool   success            = true;

    for (size_t i = 0; i < children.size(); i++) {
        Installable* child = children[i];

        if (child->install(tx)) {
            child->incParents();
            tx.countIncreasedNodes.push_back(child);
        } else {
            success = false;
            break;
        }
    }

    if (success) {
        setState(ComponentState::INSTALLED);
        tx.stateChangedNodes.push_back(this);
        return true;
    }

    // Roll back parent-count increments.
    while (tx.countIncreasedNodes.size() > initialCountSize) {
        tx.countIncreasedNodes.back()->decParents();
        tx.countIncreasedNodes.pop_back();
    }
    // Roll back state changes.
    while (tx.stateChangedNodes.size() > initialChangedSize) {
        tx.stateChangedNodes.back()->setState(ComponentState::PENDING);
        tx.stateChangedNodes.pop_back();
    }

    setState(ComponentState::FAILED);
    return false;
}

void Package::uninstall() {
    if (state != ComponentState::PENDING) {
        setState(ComponentState::PENDING);
        isExplicitlyInstalled = false;
    }
    // Cascade uninstall in LIFO order.
    for (int i = (int)children.size() - 1; i >= 0; i--) {
        children[i]->decParents();
        if (children[i]->getInstalledParentsCount() == 0 &&
            !children[i]->getExplicit()) {
            children[i]->uninstall();
        }
    }
}
