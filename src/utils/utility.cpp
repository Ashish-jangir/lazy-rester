#include "utility.hpp"

std::pair<std::string, std::string>
lazy_rester::Utility::parseHeaderLine(const std::string &header_line) {
    size_t colon_pos = header_line.find(':');
    if (colon_pos != std::string::npos) {
        std::string key = header_line.substr(0, colon_pos);
        std::string value = header_line.substr(colon_pos + 1);
        // trim whitespace from key and value
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);
        return {key, value};
    }
    return {"", ""};
}
