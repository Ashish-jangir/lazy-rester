#include "../logger/file_logger.hpp"
#include "../storage/database_store.hpp"
#include <memory>
#include <string>
namespace lazy_rester {
class Importer {
  private:
    std::shared_ptr<FileLogger> logger_;
    std::shared_ptr<DatabaseStore> db_;

  public:
    Importer(std::shared_ptr<DatabaseStore> db, std::shared_ptr<FileLogger> logger);
    void postmanCollectionImport(const std::string &file_path, AppStatePtr state);
};
} // namespace lazy_rester
