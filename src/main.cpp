#include "Daemon.hpp"
#include "TintinReporter.hpp"
#include <iostream>

int main() {
    Daemon daemon;
    
    if (!daemon.init()) {
        return 1;
    }
    
    daemon.run();
    
    return 0;
}