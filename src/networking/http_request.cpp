#include <../utils/utility.hpp>
#include <lazy_rester/http_request.hpp>
#include <sstream>
namespace lazy_rester {}
std::string lazy_rester::HttpRequest::toString() const {
    std::string result =
        "HttpRequest { id: " + std::to_string(id_) + ", folder_id: " + std::to_string(folder_id_) +
        ", label_name: " + label_name_ + ", url: " + url_ +
        ", method: " + std::string(lazy_rester::toString(method_)) + ", headers: {";
    for (const auto &[key, value] : headers_) {
        result += key + ": " + value + ", ";
    }
    result += "}, body: " + body_ + " }";
    return result;
}
std::string lazy_rester::HttpRequest::getHeadersAsString() const {
    std::string result;
    for (const auto &[key, value] : headers_) {
        result += key + ": " + value + "\n";
    }
    return result;
}
void lazy_rester::HttpRequest::setHeadersFromString(const std::string &headers_str) {
    headers_.clear();
    std::istringstream headers_stream(headers_str);
    std::string header_line;
    while (std::getline(headers_stream, header_line)) {
        auto [key, value] = lazy_rester::Utility::parseHeaderLine(header_line);
        headers_[key] = value;
    }
}
