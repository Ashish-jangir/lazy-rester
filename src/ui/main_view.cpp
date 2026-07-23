#include "main_view.hpp"
#include "../utils/utility.hpp"
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
    std::string headers;
    Component url_input = Input(&url, "url");
    std::vector<std::string> http_method_entries = {"GET",    "POST", "PUT",    "PATCH",
                                                    "DELETE", "HEAD", "OPTIONS"};
    int http_method_selected = 0;
    Component http_method_dropdown = Dropdown(&http_method_entries, &http_method_selected);

    Component send_button = Button("Send", [&]() {
        logger_->info("Send Button Pressed");
        std::thread([&]() {
            HttpRequest request;
            request.method_ = static_cast<HttpMethod>(http_method_selected);
            request.url_ = url;
            request.body_ = body;
            logger_->info("Headers: " + headers);
            // parse new line seprated headers from string to map
            std::map<std::string, std::string> headers_map;
            std::istringstream headers_stream(headers);
            std::string header_line;
            while (std::getline(headers_stream, header_line)) {
                auto [key, value] = lazy_rester::Utility::parseHeaderLine(header_line);
                headers_map[key] = value;
            }
            request.headers_ = headers_map;
            auto response_text = client_->sendRequest(request);
            response = text(response_text.body) | border;
            screen.PostEvent(Event::Custom);
        }).detach();
    });

    auto container_first_request_line =
        Container::Horizontal({http_method_dropdown, url_input, send_button});
    InputOption input_option = InputOption::Default();
    // keep background color black for input field, so that it looks like a terminal
    input_option.transform = [](InputState state) {
        state.element |= bgcolor(Color::Black);
        return state.element;
    };
    Component body_input = Input(&body, "body", input_option);
    Component headers_input = Input(
        &headers, "rows are separated by new line, key and value are separated by :", input_option);
    std::vector<std::string> tabs = {"Body", "Headers"};
    int selected_tab = 0;
    Component tab_toggle = Toggle(&tabs, &selected_tab);
    Component tab_container = Container::Tab({body_input, headers_input}, &selected_tab);

    Component container_request =
        Container::Vertical({container_first_request_line, tab_toggle, tab_container});

    Component renderer_request = Renderer(container_request, [&] {
        return vbox(
            {hbox({http_method_dropdown->Render(),
                   url_input->Render() | border | bgcolor(Color::Black), send_button->Render()}),
             separatorEmpty(), tab_toggle->Render(),
             // headers_input->Render() | flex | border,
             //          body_input->Render()}) |
             //    flex | border;
             separatorEmpty(), tab_container->Render() | flex | border | bgcolor(Color::Black)});
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
        headers = "";
        for (const auto &header : state->requests_[selected_request_index].headers_) {
            headers += header.first + ": " + header.second + "\n";
        }
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
