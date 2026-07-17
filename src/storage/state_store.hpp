#include "app_state.hpp"
#include <lazy_rester/http_request.hpp>
#include <nlohmann/json.hpp>
namespace lazy_rester {
/*
 * @brief Responsible for saving the application state to a file.
 *
 */
class StateStore {
  public:
    void saveStateToFile(const AppStatePtr &state);
    AppStatePtr loadStateFromFile();

  private:
    nlohmann::json createRequestJson(lazy_rester::HttpRequest &request);
};
} // namespace lazy_rester
