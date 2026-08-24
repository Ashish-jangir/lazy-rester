#include "../logger/file_logger.hpp"
#include "../networking/curl_http_client.hpp"
#include "../storage/database_store.hpp"
#include "app_state.hpp"
#include "dialog_manager.hpp"
#include "request_explorer.hpp"
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>
#include <lazy_rester/http_request.hpp>
#include <memory>

namespace lazy_rester {

static std::string popup_message;
/**
 * @brief Show main ui of the application
 */
class MainView {
  private:
    std::shared_ptr<CurlHttpClient> client_;
    std::shared_ptr<FileLogger> logger_;
    std::shared_ptr<DatabaseStore> db_;

  public:
    /**
     * @brief This method is used for initializing the main ui of the application
     *
     * This method intilizes all the component of the application and start rendering those on the
     * ui
     *
     * @return it returns nothing
     */
    MainView(AppStatePtr state, std::shared_ptr<DatabaseStore> db,
             std::shared_ptr<FileLogger> logger);
    ftxui::Component filePickerDialog(std::shared_ptr<DialogManager>);
};
} // namespace lazy_rester
