#include "LockFile.hpp"
#include "TintinReporter.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <errno.h>
#include <cstring>
#include <iostream>

const std::string LockFile::LOCK_FILE = "/var/lock/matt_daemon.lock";

LockFile::LockFile() : _fd(-1), _locked(false) {}

LockFile::LockFile(const LockFile& other) {
    (void)other;
}

LockFile& LockFile::operator=(const LockFile& other) {
    if (this != &other) {}
    return *this;
}

LockFile::~LockFile() {
    remove();
}

bool LockFile::create() {
    _fd = open(LOCK_FILE.c_str(), O_CREAT | O_WRONLY, 0644);

    if (_fd == -1) {
        std::cerr << "Error creating lock file: " << strerror(errno) << std::endl;
        TintinReporter::getInstance().log(TintinReporter::ERROR, "Matt_daemon: Cannot create lock file.");
        return false;
    }

    if (flock(_fd, LOCK_EX | LOCK_NB) == -1) {
        if (errno == EWOULDBLOCK) {
            std::cerr << "Can't open :" << LOCK_FILE << std::endl;
            TintinReporter::getInstance().log(TintinReporter::ERROR, "Matt_daemon: Error file locked.");
        } else {
            std::cerr << "Error locking file: " << strerror(errno) << std::endl;
            TintinReporter::getInstance().log(TintinReporter::ERROR, "Matt_daemon: Cannot lock file.");
        }
        close(_fd);
        _fd = -1;
        return false;
    }
    
    _locked = true;
    
    std::string pid_str = std::to_string(getpid()) + "\n";
    write(_fd, pid_str.c_str(), pid_str.length());
    

    return true;
}

void LockFile::remove() {
    if (_fd != -1) {
        if (_locked) {
            flock(_fd, LOCK_UN);
            _locked = false;
        }

        close(_fd);
        unlink(LOCK_FILE.c_str());
        _fd = -1;
    }
}

bool LockFile::isLocked() const {
    return _locked;
}
