#ifndef IMG_RENAME_LOG_H
#define IMG_RENAME_LOG_H

#include <chrono>
#include <sstream>

enum LoggerLevel
{
    error,
    warning,
    info,
    debug,
    trace,
};

class Log
{
public:
    explicit Log(LoggerLevel level);
    ~Log();

    static void setLoggingLevel(LoggerLevel level);
    static void setLoggingLevel(std::string_view level);
    static LoggerLevel globalLoggingLevel();

    template <typename T>
    Log& operator<< (const T& val)
    {
        if (printing) {
            os << val;
        }
        return *this;
    }

    static std::string_view toString(LoggerLevel level);

private:
    LoggerLevel level {info};
    std::chrono::system_clock::time_point tp;
    std::ostringstream os;
    bool printing {false};
};

#endif //IMG_RENAME_LOG_H
