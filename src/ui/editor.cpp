#include "editor.hpp"
#include "../utils/utility.hpp"
#include "app_state.hpp"
#include "lazy_rester/http_request.hpp"
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <memory>
#include <thread>
namespace lazy_rester {

Editor::Editor(AppStatePtr state, std::shared_ptr<DatabaseStore> db,
               std::shared_ptr<FileLogger> logger, std::shared_ptr<CurlHttpClient> client)
    : logger_(logger), client_(client), db_(db), state_(state) {};

ftxui::Component Editor::component() {
    using namespace ftxui;
    response_ = text("response") | border;
    Component url_input = Input(&(request_.url_), "url");
    http_method_entries_ = {"GET", "POST", "PUT", "PATCH", "DELETE", "HEAD", "OPTIONS"};
    Component http_method_dropdown = Dropdown(&http_method_entries_, &http_method_selected_);

    Component send_button = Button("Send", [this]() { send(); });

    auto container_first_request_line =
        Container::Horizontal({http_method_dropdown, url_input, send_button});
    InputOption input_option = InputOption::Default();
    // keep background color black for input field, so that it looks like a terminal
    input_option.transform = [](InputState state) {
        state.element |= bgcolor(Color::Black);
        return state.element;
    };
    Component body_input = Input(&(request_.body_), "body", input_option);
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

    Component renderer_request =
        Renderer(container_request,
                 [http_method_dropdown, url_input, tab_toggle, send_button, tab_container] {
                     return vbox({hbox({http_method_dropdown->Render(),
                                        url_input->Render() | border | bgcolor(Color::Black),
                                        send_button->Render()}),
                                  separatorEmpty(), tab_toggle->Render(), separatorEmpty(),
                                  tab_container->Render() | flex | border | bgcolor(Color::Black)});
                 });

    renderer_request = ResizableSplitBottom(Renderer([this]() { return response_; }),
                                            renderer_request, &main_request_area_size_);

    Component editor_component =
        Renderer(renderer_request, [renderer_request] { return renderer_request->Render(); });
    return editor_component | CatchEvent([this](Event event) {
               if (event == Event::CtrlS) {
                   saveRequestToDatabase();
                   return true;
               }
               return false;
           });
}

void Editor::updateEditor(int selected_request_index) {
    HttpRequest request = db_->getRequest(selected_request_index);
    logger_->info("Updating Editor with request index: " + request.toString());
    request_ = request;
    http_method_selected_ = static_cast<int>(request.method_);
    headers_ = request.getHeadersAsString();
}

void Editor::saveRequestToDatabase() {
    HttpRequest request = request_;
    request.method_ = static_cast<HttpMethod>(http_method_selected_);
    request.setHeadersFromString(headers_);
    logger_->info("Saving Request to Database: " + request.toString());
    db_->saveRequest(request);
}

void Editor::send() {

    logger_->info("Send Button Pressed");
    std::thread([this]() {
        HttpRequest request = request_;
        request.method_ = static_cast<HttpMethod>(http_method_selected_);
        logger_->info("Headers: " + headers_);
        request.setHeadersFromString(headers_);
        auto response_text = client_->sendRequest(request);
        response_ = ftxui::text(response_text.body) | ftxui::border;
        // app.PostEvent(Event::Custom);
    }).detach();
}
} // namespace lazy_rester
