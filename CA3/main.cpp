#include <iostream>
#include <string>
#include "InstallationEngine.h"
#include "Helpers.h"

int main() {
    InstallationEngine engine;
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::string lineCopy = line;
        if (line == "END" || getNextToken(lineCopy) == "END") break;
        engine.processCommand(line);
    }

    return 0;
}
