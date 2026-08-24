#pragma once
#include <functional>
#include <string>
namespace lazy_rester {
struct DialogManager {
    bool show = false;
    std::string title;
    int selected_index = 0;
    std::string placeholder;
    std::string input_value;
    std::vector<std::string> menu_entries = {"..", "/home/ashish"};
    std::function<void(const int &)> on_submit;

    void showInput(const std::string &title, const std::string &placeholder,
                   const std::string &initial_value, std::function<void(const int &)> callback) {
        this->title = title;
        this->placeholder = placeholder;
        input_value = initial_value;
        on_submit = callback;
        show = true;
    }

    void hide() {
        show = false;
        input_value.clear();
        on_submit = nullptr;
    }
};
} // namespace lazy_rester
