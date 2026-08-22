#include "database_store.hpp"
#include "../utils/utility.hpp"
#include <iostream>
#include <memory>
#include <sqlite3.h>
namespace lazy_rester {

DatabaseStore::DatabaseStore() {

    sqlite3_open("app_state.db", &db_);
    const char *sql = R"(
        CREATE TABLE IF NOT EXISTS folders (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            parent_id INTEGER,
            sort_order INTEGER DEFAULT 0,
            FOREIGN KEY(parent_id) REFERENCES folders(id) ON DELETE CASCADE
        );
        CREATE TABLE IF NOT EXISTS requests (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            folder_id INTEGER,
            url TEXT NOT NULL DEFAULT '',
            method INTEGER NOT NULL DEFAULT 0,
            body TEXT DEFAULT '',
            headers TEXT DEFAULT '',
            label_name TEXT NOT NULL DEFAULT '',
            sort_order INTEGER DEFAULT 0,
            FOREIGN KEY(folder_id) REFERENCES folders(id) ON DELETE CASCADE
        );
    )";
    char *errmsg;
    sqlite3_exec(db_, sql, nullptr, nullptr, &errmsg);
    if (errmsg) {
        std::cerr << "Error creating tables: " << errmsg << std::endl;
        sqlite3_free(errmsg);
    }
}
void DatabaseStore::saveState(const AppStatePtr &state) {}

AppStatePtr DatabaseStore::loadState() {
    AppStatePtr state = std::make_shared<AppState>();
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db_, "SELECT id, folder_id, url, method,label_name FROM requests", -1, &stmt,
                       nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        int folder_id = sqlite3_column_int(stmt, 1);
        std::string url = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        int method = sqlite3_column_int(stmt, 3);
        std::string label_name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        // ... populate HttpRequest
        state->requests_.emplace_back(HttpRequest{id, folder_id, label_name, url,
                                                  static_cast<HttpMethod>(method),
                                                  std::map<std::string, std::string>(), ""});
    }
    sqlite3_finalize(stmt);
    sqlite3_prepare_v2(db_, "SELECT id, name, parent_id, sort_order FROM folders", -1, &stmt,
                       nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        int parent_id = sqlite3_column_int(stmt, 2);
        int sort_order = sqlite3_column_int(stmt, 3);
        // ... populate Folder
        state->folders_.emplace_back(Folder{id, name, parent_id, sort_order});
    }
    sqlite3_finalize(stmt);
    return state;
}

void DatabaseStore::createFolder(std::string name, int parent_id) {}
void DatabaseStore::renameFolder(int id, std::string name) {}
void DatabaseStore::deleteFolder(int id) {}
HttpRequest DatabaseStore::getRequest(int id) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(
        db_,
        "SELECT id, folder_id, url, method, body, headers, label_name FROM requests WHERE id = ?",
        -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    HttpRequest request;
    std::string headers;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        request.id_ = sqlite3_column_int(stmt, 0);
        request.folder_id_ = sqlite3_column_int(stmt, 1);
        request.url_ = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        request.method_ = static_cast<HttpMethod>(sqlite3_column_int(stmt, 3));
        request.body_ = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        headers = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        request.label_name_ = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));
    }
    sqlite3_finalize(stmt);
    // get headers for the request

    std::map<std::string, std::string> headers_map;
    std::istringstream headers_stream(headers);
    std::string header_line;
    while (std::getline(headers_stream, header_line)) {
        auto [key, value] = lazy_rester::Utility::parseHeaderLine(header_line);
        headers_map[key] = value;
    }
    request.headers_ = headers_map;
    return request;
}
int DatabaseStore::saveRequest(HttpRequest request) {
    sqlite3_stmt *stmt;
    if (request.id_ > 0) {

        sqlite3_prepare_v2(db_,
                           "UPDATE requests SET folder_id = ?, url = ?, method = ?, body = ?, "
                           "label_name = ?, headers = ? WHERE id = ?",
                           -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 7, request.id_);
    } else {

        sqlite3_prepare_v2(db_,
                           "INSERT INTO requests (folder_id, url, method, body, label_name, "
                           "headers) VALUES (?, ?, ?, ?, ?)",
                           -1, &stmt, nullptr);
    }
    sqlite3_bind_int(stmt, 1, static_cast<int>(request.folder_id_));
    sqlite3_bind_text(stmt, 2, request.url_.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, static_cast<int>(request.method_));
    sqlite3_bind_text(stmt, 4, request.body_.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, request.label_name_.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, request.getHeadersAsString().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    int id = request.id_ > 0 ? request.id_ : static_cast<int>(sqlite3_last_insert_rowid(db_));
    sqlite3_finalize(stmt);
    return id;
}
void DatabaseStore::updateRequest(HttpRequest request) {}
void DatabaseStore::deleteRequest(int id) {}
void DatabaseStore::moveRequest(int id, int new_folder_id) {}
} // namespace lazy_rester
