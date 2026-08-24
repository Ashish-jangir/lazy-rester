#include "importer.hpp"
#include "app_state.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

namespace lazy_rester {

Importer::Importer(std::shared_ptr<DatabaseStore> db, std::shared_ptr<FileLogger> logger)
    : logger_(logger), db_(db) {}

void Importer::postmanCollectionImport(const std::string &file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Could not open the file: " << file_path << std::endl;
        return;
    }

    nlohmann::json json_data;
    file >> json_data;

    if (!json_data.contains("item") || !json_data["item"].is_array()) {
        std::cerr << "Invalid Postman collection format: 'item' field is missing or not an array."
                  << std::endl;
        return;
    }
    Folder folder{
        .id = 0,
        .name = json_data.value("info", nlohmann::json::object()).value("name", "Imported Folder"),
        .parent_id = 0,
        .sort_order = 0,
    };
    int collection_id = db_->saveFolder(folder);

    for (const auto &item : json_data["item"]) {
        if (!item.contains("request")) {
            std::cerr << "Invalid Postman collection format: 'request' field is missing."
                      << std::endl;
        }
    }
}
} // namespace lazy_rester
