#pragma once

#include <signal.h>

class SignalHandler {
private:
    static void                     signalCallback(int signum);
    static volatile sig_atomic_t    _quit;

public:
    static void setup();
    static bool shouldQuit();
};
