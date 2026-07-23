#include <string>
#include <utility>
namespace lazy_rester {
class Utility {
  public:
    Utility() = default;
    Utility(Utility &&) = default;
    Utility(const Utility &) = default;
    Utility &operator=(Utility &&) = default;
    Utility &operator=(const Utility &) = default;
    ~Utility() = default;
    static std::pair<std::string, std::string> parseHeaderLine(const std::string &header_line);

  private:
};
} // namespace lazy_rester
