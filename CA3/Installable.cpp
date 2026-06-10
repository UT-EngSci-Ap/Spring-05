#include "Installable.h"

Installable::Installable(std::string id, std::string title)
    : id(id), title(title),
      state(ComponentState::PENDING),
      mockFail(false),
      installedParentsCount(0),
      isExplicitlyInstalled(false)
{}

void Installable::setState(ComponentState newState) {
    if (state != newState) {
        ComponentState oldState = state;
        state = newState;
        for (size_t i = 0; i < observers.size(); i++)
            observers[i]->onStateChanged(this, oldState, newState);
    }
}

std::string    Installable::getId()    const { return id; }
std::string    Installable::getTitle() const { return title; }
ComponentState Installable::getState() const { return state; }

bool Installable::isMockFail() const  { return mockFail; }
void Installable::setMockFail(bool v) { mockFail = v; }

int  Installable::getInstalledParentsCount() const { return installedParentsCount; }
void Installable::incParents() { installedParentsCount++; }
void Installable::decParents() { if (installedParentsCount > 0) installedParentsCount--; }

bool Installable::getExplicit() const  { return isExplicitlyInstalled; }
void Installable::setExplicit(bool v)  { isExplicitlyInstalled = v; }

void Installable::addObserver(Observer* obs) { observers.push_back(obs); }

bool Installable::isPackage() const { return false; }

void Installable::forcePending() {
    setState(ComponentState::PENDING);
    installedParentsCount = 0;
    isExplicitlyInstalled = false;
    mockFail = false;
}
