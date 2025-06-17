#pragma once
#include "Ringbuffer.hpp"
#include <cstdint> // Für std::uint8_t erforderlich
#include <functional>
#include <string>

namespace wv::wiesp {
class AccessControl
{
  private:
    std::function<void(void)> mOnSuccess;
    ::wv::lib::RingBuffer<36> mBuffer;

  public:
    AccessControl() = default;
    AccessControl(std::function<void(void)> onSuccess)
      : mOnSuccess(std::move(onSuccess))
    {
    }

    void addInput(char c) { mBuffer.put(c); }
    void addInput(unsigned long c) { mBuffer.put('0' + static_cast<char>(c)); }

    void resetInput() { mBuffer.reset(); }
    bool check() const
    {
        // dummy check
        return mBuffer == "1234";
    }

    std::string data() const
    {
        std::string result;
        result.reserve(mBuffer.size());
        for (std::uint8_t idx = 0; idx < mBuffer.size(); idx++) {
            result += mBuffer.read(idx);
        }
        return result;
    }
};
} // namespace wv::wiesp
