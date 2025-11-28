#include "SignalHandler.hpp"
#include "TintinReporter.hpp"
#include <sstream>

volatile sig_atomic_t SignalHandler::_quit = 0;

void SignalHandler::setup() {
    struct sigaction sa;
    sa.sa_handler = signalCallback;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    // Intercepter les signaux principaux
    sigaction(SIGINT, &sa, NULL);   // Ctrl+C
    sigaction(SIGTERM, &sa, NULL);  // kill
    sigaction(SIGHUP, &sa, NULL);   // Terminal fermé
    sigaction(SIGQUIT, &sa, NULL);  // Ctrl+\
}

void SignalHandler::signalCallback(int signum) {
    std::string signalName;
    
    switch (signum) {
        case SIGINT:
            signalName = "SIGINT";
            break;
        case SIGTERM:
            signalName = "SIGTERM";
            break;
        case SIGHUP:
            signalName = "SIGHUP";
            break;
        case SIGQUIT:
            signalName = "SIGQUIT";
            break;
        default:
            signalName = "UNKNOWN";
            break;
    }
    
    std::string msg = "Matt_daemon: Signal handler (" + signalName + ").";
    TintinReporter::getInstance().log(TintinReporter::INFO, msg);
    _quit = 1;
}

bool SignalHandler::shouldQuit() {
    return _quit != 0;
}