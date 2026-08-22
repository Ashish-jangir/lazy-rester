#include "request_explorer.hpp"
#include "app_state.hpp"
#include "lazy_rester/http_request.hpp"
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <memory>
#include <variant>
namespace lazy_rester {
RequestExplorer::RequestExplorer(AppStatePtr state, std::shared_ptr<Editor> editor)
    : editor_(editor), state_(state) {

    for (const Folder &folder : state_->folders_) {
        request_map_[folder.parent_id].push_back(folder);
    }
    for (const HttpRequest &request : state_->requests_) {
        request_map_[request.folder_id_].push_back(request);
    }
}

ftxui::Component RequestExplorer::component() {
    using namespace ftxui;
    Component collection_container = getChildern(0);
    return Renderer(collection_container,
                    [collection_container] { return collection_container->Render(); });
}
ftxui::Component RequestExplorer::getChildern(int folder_id) {
    using namespace ftxui;
    Component collection_container = Container::Vertical({});
    MenuOption menu_option;
    for (const std::variant<Folder, HttpRequest> &item : request_map_[folder_id]) {
        if (std::holds_alternative<Folder>(item)) {
            const Folder &folder = std::get<Folder>(item);
            // Create a collapsible for this folder
            auto folder_collapsible = Collapsible(folder.name, getChildern(folder.id));
            // Add the collapsible to the collection container
            collection_container->Add(folder_collapsible);
        } else if (std::holds_alternative<HttpRequest>(item)) {
            const HttpRequest &request = std::get<HttpRequest>(item);
            if (folder_states_.contains(folder_id)) {
                folder_states_[folder_id].entries.push_back(request.label_name_);
                folder_states_[folder_id].db_ids.push_back(request.id_);
            } else {
                folder_states_[folder_id] =
                    FolderMenuState{0, {request.label_name_}, {request.id_}};
            }
        }
    }
    menu_option.entries = &(folder_states_[folder_id].entries);
    menu_option.selected = &(folder_states_[folder_id].selected);
    FolderMenuState &fs = folder_states_[folder_id];
    menu_option.on_enter = [&fs, this]() {
        // update the editor with the selected request
        editor_->updateEditor(fs.db_ids[fs.selected]);
    };
    auto menu = Menu(menu_option);
    collection_container->Add(menu);
    return Renderer(collection_container, [collection_container] {
        return hbox({text(" "), collection_container->Render()});
    });
}
} // namespace lazy_rester
