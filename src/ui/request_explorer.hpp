#include "app_state.hpp"
#include "editor.hpp"
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>
#include <vector>
namespace lazy_rester {
class RequestExplorer {
  public:
    RequestExplorer(AppStatePtr state, int &selected_request_index, std::shared_ptr<Editor> editor);
    ftxui::Component component();

  private:
    std::vector<std::string> entries_;
    int selected_request_index_;
    std::shared_ptr<Editor> editor_;
    AppStatePtr state_;
};
} // namespace lazy_rester
