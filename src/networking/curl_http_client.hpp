#include <lazy_rester/http_request.hpp>
#include <lazy_rester/http_response.hpp>

namespace lazy_rester {

/**
 * @brief This is http client to make http request
 */
class CurlHttpClient {
  public:
    /**
     * @brief make get request
     *
     * This method is used for making get request
     *
     * @param request is all the parameter like url, headers and query parameters.
     * @return HttpResponse that contains response body, status codes and headers.
     */
    HttpResponse sendGetRequest(HttpRequest &request);
};
} // namespace lazy_rester
