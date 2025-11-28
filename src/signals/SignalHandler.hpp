#pragma once

#include <signal.h>

class SignalHandler {
public:
    static void setup();
    static bool shouldQuit();

private:
    static void                     signalCallback(int signum);
    static volatile sig_atomic_t    _quit;
};
