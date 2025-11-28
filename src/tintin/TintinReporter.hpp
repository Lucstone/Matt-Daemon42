#pragma once

#include <string>
#include <fstream>

class TintinReporter {
private:
    std::ofstream               _logFile;
    static const std::string    LOG_DIR;
    static const std::string    LOG_FILE;

    TintinReporter();
    ~TintinReporter();
    TintinReporter(const TintinReporter&) = delete;
    TintinReporter& operator=(const TintinReporter&) = delete;

    std::string             getTimestamp() const;
    std::string             getLevelString(LogLevel level) const;

public:
    enum LogLevel {
        INFO,
        LOG,
        ERROR,
        WARNING
    };

    static TintinReporter   &getInstance();

    void                    log(LogLevel level, const std::string& message);
    bool                    init();
    void                    close();
};
