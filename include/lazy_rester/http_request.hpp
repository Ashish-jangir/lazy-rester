#pragma once
#include <map>
#include <stdexcept>
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
constexpr HttpMethod httpMethodFromString(const std::string &method_str) {
    if (method_str == "GET") {
        return HttpMethod::Get;
    } else if (method_str == "POST") {
        return HttpMethod::Post;
    } else if (method_str == "PUT") {
        return HttpMethod::Put;
    } else if (method_str == "PATCH") {
        return HttpMethod::Patch;
    } else if (method_str == "DELETE") {
        return HttpMethod::Delete;
    } else if (method_str == "HEAD") {
        return HttpMethod::Head;
    } else if (method_str == "OPTIONS") {
        return HttpMethod::Options;
    }
    throw std::invalid_argument("Invalid HTTP method string: " + method_str);
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
