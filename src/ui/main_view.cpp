#include "main_view.hpp"
#include "../import/importer.hpp"
#include "../utils/utility.hpp"
#include "dialog_manager.hpp"
#include "editor.hpp"
#include <filesystem>
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <memory>
using namespace ftxui;
namespace lazy_rester {
MainView::MainView(AppStatePtr state, std::shared_ptr<DatabaseStore> db,
                   std::shared_ptr<FileLogger> logger)
    : logger_(logger), db_(db) {
    client_ = std::make_shared<CurlHttpClient>(logger_);
    App screen = App::Fullscreen();
    std::shared_ptr<DialogManager> dialog = std::make_shared<DialogManager>();
    std::shared_ptr<Editor> editor = std::make_shared<Editor>(state, db_, logger_, client_);
    explorer_ = std::make_shared<RequestExplorer>(state, editor);
    int left_size = 20;

    auto main_window = ResizableSplitLeft(
        explorer_->component() | CatchEvent([this, dialog](Event event) {
            if (event == Event::CtrlR) {
                dialog->showInput("Import Postman collection", "json collection file", "~/",
                                  [this](const int input) {
                                      logger_->info("Here in import collection dialog: " + input);
                                  });
                return true;
            }
            return false;
        }),
        editor->component(), &left_size);

    auto renderer = Renderer(main_window, [&] { return main_window->Render() | border; });
    renderer |= Modal(filePickerDialog(dialog, state), &dialog->show);
    screen.Loop(renderer);
}
Component MainView::filePickerDialog(std::shared_ptr<DialogManager> dialog, AppStatePtr state) {
    auto input_field = Input(&dialog->input_value, &dialog->placeholder) | border;
    MenuOption menu_options;
    menu_options.selected = &dialog->selected_index;
    menu_options.on_enter = [this, dialog, state]() {
        logger_->info("Selected index: " + std::to_string(dialog->selected_index));
        int input = dialog->selected_index;
        namespace fs = std::filesystem;
        if (input == 0) {
            fs::path parent_path = fs::path(dialog->input_value).parent_path();
            if (fs::exists(parent_path) && fs::is_directory(parent_path)) {
                dialog->input_value = parent_path.string();
                dialog->menu_entries.erase(dialog->menu_entries.begin() + 1,
                                           dialog->menu_entries.end());
                for (const auto &entry : fs::directory_iterator(parent_path)) {
                    std::string filename = entry.path().filename().string();
                    if (filename[0] == '.' ||
                        (filename.contains(".") && !filename.contains(".json"))) {
                        continue; // Skip hidden files and directories
                    }
                    dialog->menu_entries.push_back(entry.path().string());
                }
                dialog->selected_index = 0;
            }
            return;
        }
        try {
            fs::path file_path = dialog->menu_entries[input];
            if (fs::exists(file_path) && fs::is_regular_file(file_path)) {
                dialog->hide();
                logger_->info("Importing Postman collection from file: " + file_path.string());
                Importer importer(db_, logger_);
                importer.postmanCollectionImport(file_path.string(), state);
                explorer_->refresh();
            } else if (fs::is_directory(file_path)) {
                dialog->input_value = file_path.string();
                dialog->menu_entries.erase(dialog->menu_entries.begin() + 1,
                                           dialog->menu_entries.end());
                for (const auto &entry : fs::directory_iterator(file_path)) {
                    std::string filename = entry.path().filename().string();
                    if (filename[0] == '.' ||
                        (filename.contains(".") && !filename.contains(".json"))) {
                        continue; // Skip hidden files and directories
                    }
                    dialog->menu_entries.push_back(entry.path().string());
                }
                dialog->selected_index = 0;
            }
        } catch (const fs::filesystem_error &e) {
            logger_->error("Error accessing file or directory: " + std::string(e.what()));
        }
    };
    menu_options.entries = &dialog->menu_entries;
    auto menu = Menu(menu_options);
    auto model = Container::Vertical({input_field, menu});
    model |= Renderer([dialog](Element inner) {
        return vbox({text(dialog->title), separator(), inner}) | border |
               size(WIDTH, ftxui::GREATER_THAN, 40);
    });
    model |= CatchEvent([dialog](Event event) {
        if (event == Event::Escape) {
            dialog->hide();
            return true;
        }
        return false;
    });
    return model;
}
} // namespace lazy_rester
