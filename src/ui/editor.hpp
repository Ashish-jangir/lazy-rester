#pragma once
#include "../logger/file_logger.hpp"
#include "../networking/curl_http_client.hpp"
#include "app_state.hpp"
#include <ftxui/component/app.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>
namespace lazy_rester {
class Editor {
  public:
    Editor(AppStatePtr state, std::shared_ptr<FileLogger> logger,
           std::shared_ptr<CurlHttpClient> client);
    ftxui::Component component();
    void updateEditor(int selected_request_index);

  private:
    std::string url_;
    std::string body_;
    std::string headers_;
    std::string params_;
    std::string authorization_;
    std::vector<std::string> http_method_entries_;
    int http_method_selected_ = 0;
    std::vector<std::string> tabs_;
    int selected_tab_ = 0;
    int main_request_area_size_ = 20;
    std::shared_ptr<FileLogger> logger_;
    std::shared_ptr<CurlHttpClient> client_;
    ftxui::Element response_;
    AppStatePtr state_;
};
} // namespace lazy_rester
