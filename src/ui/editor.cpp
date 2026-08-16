#include "editor.hpp"
#include "../utils/utility.hpp"
#include "app_state.hpp"
#include "lazy_rester/http_request.hpp"
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <memory>
#include <thread>
namespace lazy_rester {

Editor::Editor(AppStatePtr state, std::shared_ptr<FileLogger> logger,
               std::shared_ptr<CurlHttpClient> client)
    : logger_(logger), client_(client), state_(state) {};

ftxui::Component Editor::component() {
    using namespace ftxui;
    response_ = text("response") | border;
    Component url_input = Input(&url_, "url");
    http_method_entries_ = {"GET", "POST", "PUT", "PATCH", "DELETE", "HEAD", "OPTIONS"};
    Component http_method_dropdown = Dropdown(&http_method_entries_, &http_method_selected_);

    Component send_button = Button("Send", [this]() {
        logger_->info("Send Button Pressed");
        std::thread([&]() {
            HttpRequest request;
            request.method_ = static_cast<HttpMethod>(http_method_selected_);
            request.url_ = url_;
            request.body_ = body_;
            logger_->info("Headers: " + headers_);
            // parse new line seprated headers from string to map
            std::map<std::string, std::string> headers_map;
            std::istringstream headers_stream(headers_);
            std::string header_line;
            while (std::getline(headers_stream, header_line)) {
                auto [key, value] = lazy_rester::Utility::parseHeaderLine(header_line);
                headers_map[key] = value;
            }
            request.headers_ = headers_map;
            auto response_text = client_->sendRequest(request);
            response_ = text(response_text.body) | border;
            // app.PostEvent(Event::Custom);
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
    Component body_input = Input(&body_, "body", input_option);
    Component headers_input =
        Input(&headers_,
              "rows are separated by new line, key and value are separated by :", input_option);
    Component params_input = Input(&params_, "query parameter", input_option);
    Component authorization_input =
        Input(&authorization_, "authorization placeholder", input_option);

    tabs_ = {"Params", "Authorization", "Headers", "Body"};
    selected_tab_ = 0;
    Component tab_toggle = Toggle(&tabs_, &selected_tab_);
    Component tab_container = Container::Tab(
        {params_input, authorization_input, headers_input, body_input}, &selected_tab_);

    Component container_request =
        Container::Vertical({container_first_request_line, tab_toggle, tab_container});

    Component renderer_request = Renderer(container_request, [http_method_dropdown, url_input,
                                                              tab_toggle, send_button,
                                                              tab_container] {
        return vbox(
            {hbox({http_method_dropdown->Render(),
                   url_input->Render() | border | bgcolor(Color::Black), send_button->Render()}),
             separatorEmpty(), tab_toggle->Render(),
             // headers_input->Render() | flex | border,
             //          body_input->Render()}) |
             //    flex | border;
             separatorEmpty(), tab_container->Render() | flex | border | bgcolor(Color::Black)});
    });

    renderer_request = ResizableSplitBottom(Renderer([this]() { return response_; }),
                                            renderer_request, &main_request_area_size_);
    return Renderer(renderer_request, [renderer_request] { return renderer_request->Render(); });
}

void Editor::updateEditor(int selected_request_index) {

    url_ = state_->requests_[selected_request_index].url_;
    body_ = state_->requests_[selected_request_index].body_;
    http_method_selected_ = static_cast<int>(state_->requests_[selected_request_index].method_);
    for (const auto &header : state_->requests_[selected_request_index].headers_) {
        headers_ += header.first + ": " + header.second + "\n";
    }
}
} // namespace lazy_rester
