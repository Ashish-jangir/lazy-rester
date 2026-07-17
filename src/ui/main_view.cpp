#include "main_view.hpp"
#include "lazy_rester/http_request.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <vector>
using namespace ftxui;

lazy_rester::MainView::MainView(AppStatePtr state, std::shared_ptr<FileLogger> logger) {
    auto screen = App::TerminalOutput();
    // HttpRequest request_selected;
    int selected_request_index = 0;
    // auto request_vertical = Container::Vertical({}, &selected_request_index);
    std::vector<std::string> entries;
    for (HttpRequest request : state->requests_) {
        // auto select_request = [&]() { request_selected = request; };
        // Component button = Button(request.label_name_, select_request);
        // request_vertical->Add(button | flex);
        logger->info("Adding request to menu: " + request.label_name_);
        entries.push_back(request.label_name_);
    }
    MenuOption menu_option;
    menu_option.entries = &entries;
    menu_option.selected = &selected_request_index;
    auto menu = Menu(menu_option);
    auto collection_collapsible = Collapsible("Collection 1", menu);
    screen.Loop(collection_collapsible);
}
