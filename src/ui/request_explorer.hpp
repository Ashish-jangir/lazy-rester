#include "app_state.hpp"
#include "editor.hpp"
#include "lazy_rester/http_request.hpp"
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>
#include <variant>
#include <vector>
namespace lazy_rester {
struct FolderMenuState {
    int selected = 0;
    std::vector<std::string> entries;
    std::vector<int> db_ids; // parallel vector to entries to store the corresponding database IDs
};
class RequestExplorer {
  public:
    RequestExplorer(AppStatePtr state, std::shared_ptr<Editor> editor);
    ftxui::Component component();

  private:
    ftxui::Component empty();
    ftxui::Component getChildern(int folder_id);
    std::map<int, FolderMenuState> folder_states_; // folder id -> state
    std::vector<std::string> entries_;
    std::map<int, std::vector<std::variant<Folder, HttpRequest>>> request_map_;
    int selected_request_index_;
    std::shared_ptr<Editor> editor_;
    AppStatePtr state_;
};
} // namespace lazy_rester
