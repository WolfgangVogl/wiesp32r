#include "DataStore.hpp"
#include <Preferences.h>

namespace wv::wiesp {
struct DataStore::Private
{
  private:
    Preferences* mPref{nullptr};

  public:
    Preferences& storage()
    {
        if (!mPref)
            mPref = new Preferences();
        return *mPref;
    }
    ~Private()
    {
        if (mPref)
            delete mPref;
    }
};

DataStore::DataStore() = default;
DataStore::~DataStore()
{
    if (mImpl)
        delete mImpl;
}
DataStore::Private* DataStore::getPrivate() const
{
    if (!mImpl)
        mImpl = new Private();
    return mImpl;
}

void DataStore::store(std::string_view key, std::string_view value)
{
    getPrivate()->storage().putString(key.data(), value.data());
}
void DataStore::store(std::string_view key, const std::string& value)
{
    store(key, std::string_view{value});
}

std::optional<std::string> DataStore::load(std::string_view key) const
{
    auto& store = getPrivate()->storage();
    if (!store.isKey(key.data()))
        return std::nullopt;

    std::size_t maxLen = store.getBytesLength(key.data());
    if (maxLen == 0) // Exists but no data
        return std::string{};

    std::string buffer(maxLen, '\0');
    auto len = store.getString(key.data(), buffer.data(), maxLen);
    if (len == 0) // failure
        return std::nullopt;

    buffer.resize(len);
    return buffer;
}

bool DataStore::remove(std::string_view key)
{
    return getPrivate()->storage().remove(key.data());
}

} // namespace wv::wiesp
