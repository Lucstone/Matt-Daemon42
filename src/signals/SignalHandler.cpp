#include "SignalHandler.hpp"
#include "TintinReporter.hpp"

volatile sig_atomic_t SignalHandler::_quit = 0;

SignalHandler::SignalHandler() {}

SignalHandler::SignalHandler(const SignalHandler& other) {
    (void)other;
}

SignalHandler& SignalHandler::operator=(const SignalHandler& other) {
    (void)other;
    return *this;
}

SignalHandler::~SignalHandler() {}

void SignalHandler::setup() {
    struct sigaction    sa;

    sa.sa_handler = signalCallback;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGHUP, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
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
