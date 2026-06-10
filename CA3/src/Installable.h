#pragma once
#include <string>
#include <vector>
#include "ComponentState.h"
#include "Observer.h"
#include "TransactionContext.h"

class Installable {
protected:
    std::string       id;
    std::string       title;
    ComponentState    state;
    bool              mockFail;
    int               installedParentsCount;
    bool              isExplicitlyInstalled;
    std::vector<Observer*> observers;

public:
    Installable(std::string id, std::string title);
    virtual ~Installable() {}

    // Core accessors
    std::string    getId()    const;
    std::string    getTitle() const;
    ComponentState getState() const;

    // Mock-fail flag
    bool isMockFail() const;
    void setMockFail(bool val);

    // Parent-count tracking
    int  getInstalledParentsCount() const;
    void incParents();
    void decParents();

    // Explicit-install flag
    bool getExplicit() const;
    void setExplicit(bool val);

    // Observer registration
    void addObserver(Observer* obs);

    // Type query
    virtual bool isPackage() const;

    // Lifecycle
    virtual bool install(TransactionContext& tx) = 0;
    virtual void uninstall() = 0;
    virtual void forcePending();

    // State mutation (notifies observers)
    void setState(ComponentState newState);
};
