#include "main_view.hpp"
#include "lazy_rester/http_request.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <thread>
#include <vector>
using namespace ftxui;

lazy_rester::MainView::MainView(AppStatePtr state, std::shared_ptr<FileLogger> logger)
    : logger_(logger) {
    client_ = std::make_shared<CurlHttpClient>(logger);
    auto screen = App::Fullscreen();
    int selected_request_index = 0;
    auto response = text("response") | border;
    std::vector<std::string> entries;
    for (HttpRequest request : state->requests_) {
        entries.push_back(request.label_name_);
    }
    // Creating main container for request
    std::string url;
    std::string body;
    Component url_input = Input(&url, "url");
    std::vector<std::string> http_method_entries = {"GET",    "POST",    "PUT", "PATCH",
                                                    "DELETE", "OPTIONS", "HEAD"};
    int http_method_selected = 0;
    Component http_method_dropdown = Dropdown(&http_method_entries, &http_method_selected);

    Component send_button = Button("Send", [&]() {
        logger_->info("Send Button Pressed");
        std::thread([this, &state, &selected_request_index, &response]() {
            auto response_text = client_->sendGetRequest(state->requests_[selected_request_index]);
            response = text(response_text.body) | border;
        }).detach();
    });

    auto container_first_request_line =
        Container::Horizontal({http_method_dropdown, url_input, send_button});
    Component body_input = Input(&body, "body");
    auto container_request = Container::Vertical({container_first_request_line, body_input});
    auto renderer_request = Renderer(container_request, [&] {
        return vbox({hbox({http_method_dropdown->Render(), url_input->Render() | border,
                           send_button->Render()}),
                     separator(), body_input->Render()}) |
               flex | border;
    });
    int main_request_area_size = 20;
    renderer_request = ResizableSplitBottom(Renderer([&response]() { return response; }),
                                            renderer_request, &main_request_area_size);

    // main containder end
    MenuOption menu_option;
    menu_option.entries = &entries;
    menu_option.selected = &selected_request_index;
    menu_option.on_enter = [&]() {
        url = state->requests_[selected_request_index].url_;
        body = state->requests_[selected_request_index].body_;
        http_method_selected = static_cast<int>(state->requests_[selected_request_index].method_);
    };
    int left_size = 20;
    auto menu = Menu(menu_option);
    auto collection_collapsible = Collapsible("Collection 1", menu);
    auto main_window = Renderer(renderer_request, [&] { return renderer_request->Render(); });
    main_window = ResizableSplitLeft(
        Renderer(collection_collapsible, [=] { return collection_collapsible->Render(); }),
        main_window, &left_size);
    auto renderer = Renderer(main_window, [&] { return main_window->Render() | border; });
    screen.Loop(renderer);
}
