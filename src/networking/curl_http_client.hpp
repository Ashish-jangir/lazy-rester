#pragma once
#include "../logger/file_logger.hpp"
#include <lazy_rester/http_request.hpp>
#include <lazy_rester/http_response.hpp>
#include <memory>

namespace lazy_rester {

/**
 * @brief This is http client to make http request
 */
class CurlHttpClient {
  private:
    std::shared_ptr<FileLogger> logger_;

  public:
    CurlHttpClient(std::shared_ptr<FileLogger> logger) : logger_(logger) {}
    /**
     * @brief make get request
     *
     * This method is used for making get request
     *
     * @param request is all the parameter like url, headers and query parameters.
     * @return HttpResponse that contains response body, status codes and headers.
     */
    HttpResponse sendRequest(HttpRequest &request);

    // Callback function for writing response data
    static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp);
    static size_t headerCallback(char *buffer, size_t size, size_t nitems, void *userdata);
};
} // namespace lazy_rester
