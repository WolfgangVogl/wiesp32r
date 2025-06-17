#pragma once

namespace wv::wiesp {

class Webserver
{
  private:
    struct Private;
    Private* mImpl{nullptr};

  public:
    Webserver() = default;

    ~Webserver();
    Webserver(const Webserver&) = delete;
    Webserver& operator=(const Webserver&) = delete;

    void run();
};

} // namespace wv::wiesp
