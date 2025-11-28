#pragma once

#include <string>
#include <fstream>

class TintinReporter {
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

private:
    std::ofstream               _logFile;
    static const std::string    LOG_DIR;
    static const std::string    LOG_FILE;

    TintinReporter();
    TintinReporter(const TintinReporter& other);
    TintinReporter& operator=(const TintinReporter& other);
    ~TintinReporter();

    std::string             getTimestamp() const;
    std::string             getLevelString(LogLevel level) const;
};
