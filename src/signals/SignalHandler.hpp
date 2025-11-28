#pragma once

#include <signal.h>

class SignalHandler {
private:
    static void                     signalCallback(int signum);
    static volatile sig_atomic_t    _quit;

public:
    SignalHandler();
    SignalHandler(const SignalHandler& other);
    SignalHandler& operator=(const SignalHandler& other);
    ~SignalHandler();

    static void setup();
    static bool shouldQuit();
};
