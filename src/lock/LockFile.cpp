#include "LockFile.hpp"
#include "TintinReporter.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <iostream>

const std::string LockFile::LOCK_FILE = "/var/lock/matt_daemon.lock";

LockFile::LockFile() : _fd(-1) {}

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
    _fd = open(LOCK_FILE.c_str(), O_CREAT | O_EXCL | O_WRONLY, 0644);

    if (_fd == -1) {
        if (errno == EEXIST) {
            std::cerr << "Can't open :" << LOCK_FILE << std::endl;
            TintinReporter::getInstance().log(TintinReporter::ERROR, "Matt_daemon: Error file locked.");
        } else {
            std::cerr << "Error creating lock file: " << strerror(errno) << std::endl;
            TintinReporter::getInstance().log(TintinReporter::ERROR, "Matt_daemon: Cannot create lock file.");
        }
        return false;
    }

    return true;
}

void LockFile::remove() {
    if (_fd != -1) {
        close(_fd);
        unlink(LOCK_FILE.c_str());
        _fd = -1;
    }
}
