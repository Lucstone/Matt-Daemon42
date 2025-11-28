#pragma once

#include <string>

class LockFile {
private:
    static const std::string    LOCK_FILE;
    int                         _fd;

public:
    LockFile();
    ~LockFile();

    bool create();
    void remove();
};
