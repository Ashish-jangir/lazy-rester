#include "curl_http_client.hpp"
#include "curl/curl.h"

lazy_rester::HttpResponse
lazy_rester::CurlHttpClient::sendGetRequest(lazy_rester::HttpRequest &request) {
    CURL *curl = curl_easy_init();
    lazy_rester::HttpResponse http_response;
    curl_easy_setopt(curl, CURLOPT_URL, request.url_.c_str());
    curl_easy_perform(curl);

    if (curl) {
        curl_easy_cleanup(curl);
    }
    return http_response;
}
