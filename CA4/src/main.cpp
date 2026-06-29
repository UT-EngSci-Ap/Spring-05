#include <iostream>

#include "Application.h"

int main() {
    try {
        Application app;
        if (!app.init()) {
            return 1;
        }
        app.run();
        app.shutdown();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
