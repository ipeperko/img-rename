#include "Log.h"
#include <iomanip>
#include <iostream>

namespace {
LoggerLevel global_logging_level = info;

std::string timeStamp(std::chrono::system_clock::time_point tp)
{
    std::ostringstream ss;
    std::time_t tp_c = std::chrono::system_clock::to_time_t(tp);
    double tp_ms = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
    tp_ms -= std::milli::den * tp_c;
    struct tm tm{};
    gmtime_r(&tp_c, &tm);
    ss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << tp_ms << std::setfill(' ') << "Z";
    return ss.str();
}
} // namespace

Log::Log(LoggerLevel level)
    : level(level)
{
    if (level <= global_logging_level) {
        printing = true;
        tp = std::chrono::system_clock::now();
    }
}

Log::~Log()
{
    if (printing) {
        std::ostringstream out;
        out << "[" << timeStamp(tp) << "]";
        out << " [" << toString(level) << "]";
        out << " " << os.str();

        std::string s = out.str();
        std::string el;
        if (s.empty() || s.back() != '\n') {
            el = "\n";
        }

        if (level <= warning) {
            std::cout.flush();
            std::cerr << out.str() << el;
        }
        else {
            std::cout << out.str() << el;
        }
    }
}

void Log::setLoggingLevel(LoggerLevel level)
{
    global_logging_level = level;
}

void Log::setLoggingLevel(std::string_view level)
{
    if (level == "info") {
        setLoggingLevel(info);
    }
    else if (level == "debug") {
        Log::setLoggingLevel(debug);
    }
    else if (level == "trace") {
        Log::setLoggingLevel(trace);
    }
    else if (level == "warning") {
        Log::setLoggingLevel(warning);
    }
    else if (level == "error") {
        Log::setLoggingLevel(error);
    }
    else {
        throw std::domain_error("Log level '" + std::string(level) + "' not valid");
    }
}

LoggerLevel Log::globalLoggingLevel()
{
    return global_logging_level;
}

std::string_view Log::toString(LoggerLevel level)
{
    switch (level) {
        case error : return "error";
        case warning : return "warning";
        case info : return "info";
        case debug : return "debug";
        case trace : return "trace";
        default: return "unknown";
    }
}
