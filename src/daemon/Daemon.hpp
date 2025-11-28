#pragma once

#include "Server.hpp"
#include "LockFile.hpp"

class Daemon {
private:
    Server*     _server;
    LockFile*   _lockFile;

    bool daemonize();
    bool checkRoot();

public:
    Daemon();
    ~Daemon();

    bool init();
    void run();
};
