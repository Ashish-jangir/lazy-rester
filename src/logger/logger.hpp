#pragma once
#include <source_location>
#include <string_view>
class Logger {
  public:
    Logger() = default;
    virtual ~Logger() = default;
    virtual void debug(std::string_view message,
                       const std::source_location &loc = std::source_location::current()) = 0;

    virtual void info(std::string_view message,
                      const std::source_location &loc = std::source_location::current()) = 0;

    virtual void warn(std::string_view message,
                      const std::source_location &loc = std::source_location::current()) = 0;

    virtual void error(std::string_view message,
                       const std::source_location &loc = std::source_location::current()) = 0;

    virtual void fatal(std::string_view message,
                       const std::source_location &loc = std::source_location::current()) = 0;
};
