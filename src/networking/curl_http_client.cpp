#include "curl_http_client.hpp"
#include "curl/curl.h"

lazy_rester::HttpResponse
lazy_rester::CurlHttpClient::sendRequest(lazy_rester::HttpRequest &request) {
    CURL *curl = curl_easy_init();
    struct curl_slist *list = nullptr;
    for (const auto &header : request.headers_) {
        std::string header_string = header.first + ": " + header.second;
        list = curl_slist_append(list, header_string.c_str());
    }
    lazy_rester::HttpResponse http_response;
    std::string response_string;
    curl_easy_setopt(curl, CURLOPT_URL, request.url_.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, toString(request.method_).data());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.body_.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &http_response.body);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &http_response.headers);
    CURLcode result = curl_easy_perform(curl);

    if (result == CURLE_OK) {
        long response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        http_response.status_code = static_cast<int>(response_code);
    } else {
        logger_->error("curl_easy_perform() failed: " +
                       std::to_string(*curl_easy_strerror(result)));
    }
    if (curl) {
        curl_easy_cleanup(curl);
    }
    if (list) {
        curl_slist_free_all(list);
    }
    return http_response;
}
size_t lazy_rester::CurlHttpClient::writeCallback(void *contents, size_t size, size_t nmemb,
                                                  void *userp) {
    static_cast<std::string *>(userp)->append(static_cast<char *>(contents), size * nmemb);
    return size * nmemb;
}
size_t lazy_rester::CurlHttpClient::headerCallback(char *buffer, size_t size, size_t nitems,
                                                   void *userdata) {
    std::string header(buffer, size * nitems);
    std::map<std::string, std::string> *headers =
        static_cast<std::map<std::string, std::string> *>(userdata);
    header.erase(header.find_last_not_of("\r\n") + 1);
    size_t colon_pos = header.find(':');
    if (colon_pos != std::string::npos) {
        std::string key = header.substr(0, colon_pos);
        std::string value = header.substr(colon_pos + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        (*headers)[key] = value;
    }
    return size * nitems;
}
