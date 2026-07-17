#include "state_store.hpp"
#include <fstream>

nlohmann::json lazy_rester::StateStore::createRequestJson(lazy_rester::HttpRequest &request) {
    nlohmann::json request_json;
    request_json["url"] = request.url_;
    request_json["method"] = request.method_;
    request_json["body"] = request.body_;
    request_json["label_name"] = request.label_name_;
    request_json["headers"] = nlohmann::json::array();
    for (const auto &[key, value] : request.headers_) {
        nlohmann::json header_json;
        header_json[key] = value;
        request_json["headers"].push_back(header_json);
    }
    return request_json;
}

void lazy_rester::StateStore::saveStateToFile(const AppStatePtr &state) {
    nlohmann::json j;
    j["requests"] = nlohmann::json::array();
    for (lazy_rester::HttpRequest &request : state->requests_) {
        j["requests"].push_back(createRequestJson(request));
        j["selected_request_index"] = state->selected_request_index_;
        j["current_open_request"] = createRequestJson(state->current_open_request_);
        j["dirty"] = state->dirty_;
    }
    std::ofstream file("app_state.json");
    file << j.dump(2);
}

lazy_rester::AppStatePtr lazy_rester::StateStore::loadStateFromFile() {
    std::ifstream file("app_state.json");
    if (!file.is_open()) {
        return std::make_shared<AppState>();
    }
    nlohmann::json j;
    file >> j;
    AppStatePtr state = std::make_shared<AppState>();
    for (const auto &request_json : j["requests"]) {
        lazy_rester::HttpRequest request;
        request.url_ = request_json["url"];
        request.method_ = request_json["method"];
        request.body_ = request_json["body"];
        request.label_name_ = request_json["label_name"];
        for (const auto &header_json : request_json["headers"]) {
            for (auto it = header_json.begin(); it != header_json.end(); ++it) {
                request.headers_[it.key()] = it.value();
            }
        }
        state->requests_.push_back(request);
    }
    state->selected_request_index_ = j["selected_request_index"];
    lazy_rester::HttpRequest current_open_request;
    current_open_request.url_ = j["current_open_request"]["url"];
    current_open_request.method_ = j["current_open_request"]["method"];
    current_open_request.body_ = j["current_open_request"]["body"];
    current_open_request.label_name_ = j["current_open_request"]["label_name"];
    for (const auto &header_json : j["current_open_request"]["headers"]) {
        for (auto it = header_json.begin(); it != header_json.end(); ++it) {
            current_open_request.headers_[it.key()] = it.value();
        }
    }
    state->current_open_request_ = current_open_request;
    state->dirty_ = j["dirty"];
    return state;
}
