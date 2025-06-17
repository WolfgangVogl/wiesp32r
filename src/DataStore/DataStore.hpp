#pragma once
#include <optional>
#include <string>
#include <string_view>

namespace wv::wiesp {

class DataStore
{
  private:
    struct Private;
    mutable Private* mImpl{nullptr};
    Private* getPrivate() const;

  public:
    DataStore();
    ~DataStore();

    DataStore(const DataStore&) = delete;
    DataStore& operator=(const DataStore&) = delete;

    void store(std::string_view key, std::string_view value);
    void store(std::string_view key, const std::string& value);

    std::optional<std::string> load(std::string_view key) const;

    bool remove(std::string_view key);
};

} // namespace wv::wieacc
