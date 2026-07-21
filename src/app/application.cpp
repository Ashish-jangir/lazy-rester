#include "application.hpp"
#include "../logger/file_logger.hpp"
#include "curl/curl.h"
#include <future>
#include <iostream>
#include <memory>
Application::Application() {
    using lazy_rester::MainView;
    using std::make_shared;

    CURLcode curl_result = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (curl_result != CURLE_OK) {
        std::cerr << "Failed to initialize libcurl: " << curl_easy_strerror(curl_result)
                  << std::endl;
        return;
    }

    std::future<std::shared_ptr<FileLogger>> result = std::async(std::launch::async, []() {
        std::shared_ptr<FileLogger> logger = std::make_shared<FileLogger>();
        logger->info("Application started");
        return logger;
    });
    // https://a42a-2405-201-403c-b843-5f43-5e4-baba-94bf.ngrok-free.app
    // lazy_rester::HttpRequest request;
    // request.label_name_ = "Request 1";
    // request.method_ = lazy_rester::HttpMethod::Get;
    // request.url_ = "https://a42a-2405-201-403c-b843-5f43-5e4-baba-94bf.ngrok-free.app";
    // lazy_rester::AppState app_state;
    // app_state.requests_ = std::vector<lazy_rester::HttpRequest>({request, request});
    // app_state.current_open_request_ = request;
    // app_state.selected_request_index_ = 1;
    // app_state.dirty_ = true;
    // lazy_rester::StateStore().saveStateToFile(std::make_shared<lazy_rester::AppState>(app_state));

    lazy_rester::AppStatePtr state = lazy_rester::StateStore().loadStateFromFile();
    std::shared_ptr<FileLogger> logger = result.get();
    auto main_view = std::make_shared<MainView>(state, logger);

    curl_global_cleanup();
}
