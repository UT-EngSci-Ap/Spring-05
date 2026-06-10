#include "ComponentState.h"

std::string stateToString(ComponentState s) {
    if (s == ComponentState::PENDING)   return "PENDING";
    if (s == ComponentState::INSTALLED) return "INSTALLED";
    return "FAILED";
}
