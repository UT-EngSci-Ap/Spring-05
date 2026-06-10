#pragma once
#include <string>

enum class ComponentState {
    PENDING,
    INSTALLED,
    FAILED
};

std::string stateToString(ComponentState s);
