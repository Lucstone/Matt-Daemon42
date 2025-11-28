#include "TintinReporter.hpp"
#include <iostream>
#include <sys/stat.h>
#include <ctime>
#include <iomanip>
#include <sstream>

const std::string TintinReporter::LOG_DIR = "/var/log/matt_daemon/";
const std::string TintinReporter::LOG_FILE = "/var/log/matt_daemon/matt_daemon.log";

TintinReporter::TintinReporter() {}

TintinReporter::~TintinReporter() {
    close();
}

TintinReporter& TintinReporter::getInstance() {
    static TintinReporter   instance;

    return instance;
}

bool TintinReporter::init() {
    mkdir(LOG_DIR.c_str(), 0755);

    _logFile.open(LOG_FILE.c_str(), std::ios::app);

    if (!_logFile.is_open()) {
        std::cerr << "Error: Cannot open log file: " << LOG_FILE << std::endl;
        return false;
    }

    return true;
}

void TintinReporter::close() {
    if (_logFile.is_open()) {
        _logFile.close();
    }
}

std::string TintinReporter::getTimestamp() const {
    time_t              now = time(0);
    struct tm           *timeinfo = localtime(&now);
    std::ostringstream  oss;

    oss << std::setfill('0') 
        << std::setw(2) << timeinfo->tm_mday << "/"
        << std::setw(2) << (timeinfo->tm_mon + 1) << "/"
        << (timeinfo->tm_year + 1900) << "-"
        << std::setw(2) << timeinfo->tm_hour << ":"
        << std::setw(2) << timeinfo->tm_min << ":"
        << std::setw(2) << timeinfo->tm_sec;

    return oss.str();
}

std::string TintinReporter::getLevelString(LogLevel level) const {
    switch (level) {
        case INFO: return "INFO";
        case LOG: return "LOG";
        case ERROR: return "ERROR";
        case WARNING: return "WARNING";
        default: return "UNKNOWN";
    }
}

void TintinReporter::log(LogLevel level, const std::string& message) {
    if (!_logFile.is_open()) {
        return;
    }

    _logFile << "[" << getTimestamp() << "] [ " 
             << getLevelString(level) << " ] - " 
             << message << std::endl;
    _logFile.flush();
}
