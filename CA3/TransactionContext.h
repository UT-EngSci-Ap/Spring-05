#pragma once
#include <vector>

// Forward declaration – avoids pulling in the full Installable header here.
class Installable;

struct TransactionContext {
    std::vector<Installable*> stateChangedNodes;
    std::vector<Installable*> countIncreasedNodes;
};
