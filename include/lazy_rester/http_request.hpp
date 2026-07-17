#pragma once
#include <map>
#include <string>
namespace lazy_rester {

enum class HttpMethod { Get, Post, Put, Patch, Delete, Option };

class HttpRequest {
  public:
    std::string label_name_;
    std::string url_;
    HttpMethod method_;
    std::map<std::string, std::string> headers_;
    std::string body_;
};
} // namespace lazy_rester
