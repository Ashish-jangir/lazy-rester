#pragma once
#include "app_state.hpp"
#include "sqlite3.h"
#include <lazy_rester/http_request.hpp>
#include <nlohmann/json.hpp>
namespace lazy_rester {
/*
 * @brief Responsible for saving the application state to database.
 *
 */
class DatabaseStore {

  public:
    DatabaseStore();
    void saveState(const AppStatePtr &state);
    AppStatePtr loadState();
    void createFolder(std::string name, int parent_id);
    void renameFolder(int id, std::string name);
    void deleteFolder(int id);

    HttpRequest getRequest(int id);
    int saveRequest(HttpRequest request);
    void updateRequest(HttpRequest request);
    void deleteRequest(int id);
    void moveRequest(int id, int new_folder_id);

  private:
    sqlite3 *db_;
};
} // namespace lazy_rester
