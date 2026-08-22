#pragma once
#include <map>
#include <string>
namespace lazy_rester {

enum class HttpMethod { Get, Post, Put, Patch, Delete, Head, Options };
constexpr std::string_view toString(HttpMethod method) {
    switch (method) {
    case HttpMethod::Get:
        return "GET";
    case HttpMethod::Post:
        return "POST";
    case HttpMethod::Put:
        return "PUT";
    case HttpMethod::Patch:
        return "PATCH";
    case HttpMethod::Delete:
        return "DELETE";
    case HttpMethod::Head:
        return "HEAD";
    case HttpMethod::Options:
        return "OPTIONS";
    }
    return "UNKNOWN";
}
class HttpRequest {
  public:
    int id_;
    int folder_id_;
    std::string label_name_;
    std::string url_;
    HttpMethod method_;
    std::map<std::string, std::string> headers_;
    std::string body_;
    std::string toString() const;
    std::string getHeadersAsString() const;
    void setHeadersFromString(const std::string &headers_str);
};
} // namespace lazy_rester
