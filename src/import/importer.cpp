#include "importer.hpp"
#include "app_state.hpp"
#include "lazy_rester/http_request.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <stack>

namespace lazy_rester {

Importer::Importer(std::shared_ptr<DatabaseStore> db, std::shared_ptr<FileLogger> logger)
    : logger_(logger), db_(db) {}

void Importer::postmanCollectionImport(const std::string &file_path, AppStatePtr state) {
    using namespace nlohmann;
    std::ifstream file(file_path);
    if (!file.is_open()) {
        logger_->error("Could not open the file: " + file_path);
        return;
    }

    nlohmann::json json_data;
    file >> json_data;

    if (!json_data.contains("item") || !json_data["item"].is_array()) {
        logger_->error(
            "Invalid Postman collection format: 'item' field is missing or not an array.");
        return;
    }
    logger_->info("Importing Postman collection: " +
                  json_data.value("info", json::object()).value("name", "Imported Folder"));
    Folder folder{
        .id = 0,
        .name = json_data.value("info", json::object()).value("name", "Imported Folder"),
        .parent_id = 0,
        .sort_order = 0,
    };
    int collection_id = db_->saveFolder(folder);

    std::stack<std::pair<int, json>> item_stack;
    item_stack.push({collection_id, json_data});
    while (!item_stack.empty()) {
        std::pair<int, json> current_item = item_stack.top();
        item_stack.pop();

        if (current_item.second.contains("item") && current_item.second["item"].is_array()) {
            if (current_item.second.contains("name")) {
                Folder sub_folder{
                    .id = 0,
                    .name = current_item.second.value("name", "Unnamed Folder"),
                    .parent_id = current_item.first,
                    .sort_order = 0,
                };
                int sub_folder_id = db_->saveFolder(sub_folder);
                for (const auto &sub_item : current_item.second["item"]) {
                    item_stack.push({sub_folder_id, sub_item});
                }
            } else {
                // It runs only once per collection, because the root collection has no name, but it
                // has items.
                for (const auto &sub_item : current_item.second["item"]) {
                    item_stack.push({current_item.first, sub_item});
                }
            }
        } else if (current_item.second.contains("request")) {
            HttpRequest request;
            request.id_ = 0;
            request.label_name_ = current_item.second.value("name", "Unnamed Request");
            request.method_ =
                httpMethodFromString(current_item.second["request"].value("method", "GET"));
            request.url_ =
                current_item.second["request"].value("url", json::object()).value("raw", "");
            std::map<std::string, std::string> headers;
            for (const auto &header :
                 current_item.second["request"].value("header", json::array())) {
                headers[header.value("key", "")] = header.value("value", "");
            }
            request.headers_ = headers;
            request.body_ =
                current_item.second["request"].value("body", json::object()).value("raw", "");
            request.folder_id_ = current_item.first;
            db_->saveRequest(request);
        }
    }
    AppStatePtr new_state = db_->loadState();
    state->requests_ = new_state->requests_;
    state->folders_ = new_state->folders_;
}
} // namespace lazy_rester
