#include "Daemon.hpp"
#include "TintinReporter.hpp"
#include "SignalHandler.hpp"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>

Daemon::Daemon() : _server(NULL), _lockFile(NULL) {}

Daemon::Daemon(const Daemon& other) {
    (void)other;
}

Daemon& Daemon::operator=(const Daemon& other) {
    if (this != &other) {}
    return *this;
}

Daemon::~Daemon() {
    delete _server;
    delete _lockFile;
}

bool Daemon::checkRoot() {
    if (getuid() != 0) {
        std::cerr << "Error: This daemon must be run as root." << std::endl;
        return false;
    }
    return true;
}

bool Daemon::init() {
    if (!checkRoot()) {
        return false;
    }

    if (!TintinReporter::getInstance().init()) {
        return false;
    }

    TintinReporter::getInstance().log(TintinReporter::INFO, "Matt_daemon: Started.");

    _lockFile = new LockFile();
    if (!_lockFile->create()) {
        TintinReporter::getInstance().log(TintinReporter::INFO, "Matt_daemon: Quitting.");
        return false;
    }

    _server = new Server(4242);
    if (!_server->init()) {
        return false;
    }

    SignalHandler::setup();

    if (!daemonize()) {
        return false;
    }

    return true;
}

bool Daemon::daemonize() {
    TintinReporter::getInstance().log(TintinReporter::INFO, "Matt_daemon: Entering Daemon mode.");

    pid_t pid = fork();
    if (pid < 0) {
        TintinReporter::getInstance().log(TintinReporter::ERROR, "Matt_daemon: Fork failed.");
        return false;
    }

    if (pid > 0) {
        exit(0);
    }

    if (setsid() < 0) {
        TintinReporter::getInstance().log(TintinReporter::ERROR, "Matt_daemon: setsid failed.");
        return false;
    }

    pid = fork();
    if (pid < 0) {
        TintinReporter::getInstance().log(TintinReporter::ERROR, "Matt_daemon: Second fork failed.");
        return false;
    }

    if (pid > 0) {
        exit(0);
    }

    chdir("/");
    umask(0);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    int fd = open("/dev/null", O_RDWR);
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    if (fd > 2) {
        close(fd);
    }

    std::ostringstream oss;
    oss << "Matt_daemon: started. PID: " << getpid() << ".";
    TintinReporter::getInstance().log(TintinReporter::INFO, oss.str());

    return true;
}

void Daemon::run() {
    if (_server) {
        _server->run();
    }

    TintinReporter::getInstance().log(TintinReporter::INFO, "Matt_daemon: Quitting.");
}
