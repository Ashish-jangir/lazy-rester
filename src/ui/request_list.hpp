#include "lazy_rester/http_request.hpp"
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <functional>
#include <vector>
namespace lazy_rester {
class RequestList {
  public:
    RequestList(std::vector<HttpRequest> &requests);
    ftxui::Component inner(std::vector<ftxui::Component> children);
};
} // namespace lazy_rester
