#pragma once
#include <map>
#include <string>

namespace lazy_rester {
class HttpResponse {
  public:
    std::string body;
    std::map<std::string, std::string> headers;
};
} // namespace lazy_rester
