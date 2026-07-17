#pragma once
#include "log_level.hpp"
#include "logger.hpp"
#include <fstream>
#include <mutex>
#include <source_location>
namespace fs = std::filesystem;
class FileLogger : public Logger {
  private:
    std::mutex log_file_lock_;
    std::ofstream fout_;
#ifdef LAZY_RESTER_LEVEL_DEBUG
    LogLevel log_level_ = LogLevel::DEBUG;
#endif
#ifdef LAZY_RESTER_LEVEL_INFO
    LogLevel log_level_ = LogLevel::INFO;
#endif
#ifdef LAZY_RESTER_LEVEL_WARN
    LogLevel log_level_ = LogLevel::WARN;
#endif
#ifdef LAZY_RESTER_LEVEL_ERROR
    LogLevel log_level_ = LogLevel::ERROR;
#endif
#ifdef LAZY_RESTER_LEVEL_FATAL
    LogLevel log_level_ = LogLevel::FATAL;
#endif
  public:
    FileLogger();
    void info(std::string_view message,
              const std::source_location &loc = std::source_location::current()) override;
    void debug(std::string_view message,
               const std::source_location &loc = std::source_location::current()) override;
    void error(std::string_view message,
               const std::source_location &loc = std::source_location::current()) override;
    void fatal(std::string_view message,
               const std::source_location &loc = std::source_location::current()) override;
    void warn(std::string_view message,
              const std::source_location &loc = std::source_location::current()) override;
};
