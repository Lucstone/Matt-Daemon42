#pragma once

#include "Server.hpp"
#include "LockFile.hpp"

class Daemon {
public:
    Daemon();
    ~Daemon();

    bool init();
    void run();

private:
    bool daemonize();
    bool checkRoot();

    Server*     _server;
    LockFile*   _lockFile;
};
