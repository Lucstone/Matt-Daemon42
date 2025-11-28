#pragma once

#include <string>

class LockFile {
public:
    LockFile();
    ~LockFile();

    bool create();
    void remove();
    bool exists() const;

private:
    static const std::string    LOCK_FILE;
    int                         _fd;
};
