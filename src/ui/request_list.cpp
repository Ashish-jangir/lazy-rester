#include "request_list.hpp"
lazy_rester::RequestList::RequestList(std::vector<HttpRequest> &requests) {
    ftxui::Component component = ftxui::Container::Vertical({});
    std::vector<std::string> request_entries;
    for (HttpRequest req : requests) {
        request_entries.push_back(req.label_name_);
    }
    int selected = 0;
    ftxui::MenuOption option;
    option.on_enter = [this] {};
    std::vector<std::string> collection_entries = {
        "Collection 1",
        "Collection 2",
        "Collection 3",
    };
    for (std::string collection_title : collection_entries) {
        
    }
}

ftxui::Component Inner(std::vector<ftxui::Component> children) {
    ftxui::Component vlist = ftxui::Container::Vertical(std::move(children));
    return Renderer(vlist, [vlist] {
        return ftxui::hbox({
            ftxui::text(" "),
            vlist->Render(),
        });
    });
}
