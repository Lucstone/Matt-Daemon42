#pragma once

#include <string>

class LockFile {
private:
    static const std::string    LOCK_FILE;
    int                         _fd;
    bool                        _locked;

public:
    LockFile();
    LockFile(const LockFile& other);
    LockFile& operator=(const LockFile& other);
    ~LockFile();

    bool create();
    void remove();
    bool isLocked() const;
};
