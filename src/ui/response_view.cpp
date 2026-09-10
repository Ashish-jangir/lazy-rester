#include "response_view.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace lazy_rester {
ResponseView::ResponseView() {}
ftxui::Component ResponseView::component() {
    using namespace ftxui;
    return Renderer([this](bool focused) {
        Element element =
            paragraph(response_) | vscroll_indicator | hscroll_indicator | frame | yflex;
        if (focused) {
            element |= focus;
            // element |= borderStyled(Color::Green);
        } else {
            // element |= border;
        }
        return element;
    });
}
void ResponseView::updateResponse(const std::string &response) {
    using namespace ftxui;
    response_ = response;
}
} // namespace lazy_rester
