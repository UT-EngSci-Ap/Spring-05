#pragma once
#include "ComponentState.h"

// Forward declaration.
class Installable;

class Observer {
public:
    virtual void onStateChanged(const Installable* comp,
                                ComponentState oldState,
                                ComponentState newState) = 0;
    virtual ~Observer() {}
};
