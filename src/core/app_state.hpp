#pragma once
#include "folder.hpp"
#include <lazy_rester/http_request.hpp>
#include <memory>
#include <vector>
namespace lazy_rester {
/*
 * @brief The AppState class holds the state of application.
 *
 * Manages State such as the list of collections, the selected collection, and the list of requests
 * in the selected collection. It also provides methods to manipulate this state, such as adding a
 * new collection, selecting a collection, and adding a new request to the selected collection.
 */
class AppState {
  public:
    std::vector<HttpRequest> requests_;
    std::vector<Folder> folders_;
    int selected_request_index_ = -1;
    int selected_folder_id_ = -1;
    HttpRequest current_open_request_;
    bool dirty_ = false;
};
using AppStatePtr = std::shared_ptr<AppState>;
} // namespace lazy_rester
