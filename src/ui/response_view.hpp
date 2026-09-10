#include <ftxui/component/app.hpp>
namespace lazy_rester {
class ResponseView {
  private:
    std::string response_ = "response";

  public:
    ResponseView();
    ftxui::Component component();
    ftxui::Component response_component_;
    void updateResponse(const std::string &response);
};
} // namespace lazy_rester
