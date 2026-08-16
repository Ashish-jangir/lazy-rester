#include "request_explorer.hpp"
#include "app_state.hpp"
#include <ftxui/component/app.hpp>
#include <memory>
namespace lazy_rester {
RequestExplorer::RequestExplorer(AppStatePtr state, int &selected_request_index,
                                 std::shared_ptr<Editor> editor)
    : selected_request_index_(selected_request_index), editor_(editor), state_(state) {}

ftxui::Component RequestExplorer::component() {
    using namespace ftxui;
    for (HttpRequest request : state_->requests_) {
        entries_.push_back(request.label_name_);
    }
    MenuOption menu_option;
    menu_option.entries = &entries_;
    menu_option.selected = &selected_request_index_;
    // TODO add a callback for on_enter
    menu_option.on_enter = [this]() { editor_->updateEditor(selected_request_index_); };
    auto menu = Menu(menu_option);
    auto collection_collapsible = Collapsible("Collection 1", menu);
    return Renderer(collection_collapsible,
                    [collection_collapsible] { return collection_collapsible->Render(); });
}
} // namespace lazy_rester
