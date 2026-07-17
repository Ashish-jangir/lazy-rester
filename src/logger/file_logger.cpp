#include "file_logger.hpp"
#include <chrono>
#include <filesystem>
#include <source_location>
namespace fs = std::filesystem;

FileLogger::FileLogger() {
    auto now = std::chrono::system_clock::now();
    auto today = floor<std::chrono::days>(now);
    std::chrono::year_month_day ymd{today};

    fs::path filepath = "logs/" + std::to_string(int(ymd.year())) + "-" +
                        std::to_string(unsigned(ymd.month())) + "-" +
                        std::to_string(unsigned(ymd.day())) + ".log";
    // check if file and directory exists, if not create them
    if (!fs::exists(filepath.parent_path())) {
        fs::create_directories(filepath.parent_path());
    }
    fout_.open(filepath, std::ios::app);
}
void FileLogger::info(std::string_view message, const std::source_location &loc) {
    std::lock_guard<std::mutex> lock(log_file_lock_);
    auto now = std::chrono::system_clock::now();
    if (log_level_ > LogLevel::INFO) {
        return;
    }
    fout_ << now << " [INFO] [" << loc.function_name() << "] " << message << std::endl;
}
void FileLogger::debug(std::string_view message, const std::source_location &loc) {
    std::lock_guard<std::mutex> lock(log_file_lock_);
    auto now = std::chrono::system_clock::now();
    if (log_level_ > LogLevel::DEBUG) {
        return;
    }
    fout_ << now << " [DEBUG] [" << loc.function_name() << "] " << message << std::endl;
}
void FileLogger::error(std::string_view message, const std::source_location &loc) {
    std::lock_guard<std::mutex> lock(log_file_lock_);
    auto now = std::chrono::system_clock::now();
    if (log_level_ > LogLevel::ERROR) {
        return;
    }
    fout_ << now << " [ERROR] [" << loc.function_name() << "] " << message << std::endl;
}
void FileLogger::fatal(std::string_view message, const std::source_location &loc) {
    std::lock_guard<std::mutex> lock(log_file_lock_);
    auto now = std::chrono::system_clock::now();
    if (log_level_ > LogLevel::FATAL) {
        return;
    }
    fout_ << now << " [FATAL] [" << loc.function_name() << "] " << message << std::endl;
}
void FileLogger::warn(std::string_view message, const std::source_location &loc) {
    std::lock_guard<std::mutex> lock(log_file_lock_);
    auto now = std::chrono::system_clock::now();
    if (log_level_ > LogLevel::WARN) {
        return;
    }
    fout_ << now << " [WARN] [" << loc.function_name() << "] " << message << std::endl;
}
