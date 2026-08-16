#include "main_view.hpp"
#include "../utils/utility.hpp"
#include "editor.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <memory>
using namespace ftxui;

lazy_rester::MainView::MainView(AppStatePtr state, std::shared_ptr<FileLogger> logger)
    : logger_(logger) {
    client_ = std::make_shared<CurlHttpClient>(logger_);
    App screen = App::Fullscreen();
    std::shared_ptr<Editor> editor = std::make_shared<Editor>(state, logger_, client_);
    int selected_request_index = 0;
    RequestExplorer request_explorer(state, selected_request_index, editor);

    int left_size = 20;

    auto main_window =
        ResizableSplitLeft(request_explorer.component(), editor->component(), &left_size);

    auto renderer = Renderer(main_window, [&] { return main_window->Render() | border; });
    screen.Loop(renderer);
}
