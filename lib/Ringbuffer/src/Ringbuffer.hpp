#pragma once
#include <cstdint>

namespace wv::lib {
// A simple ringbuffer implementation
template<std::uint8_t SIZE>
class RingBuffer
{
    static_assert(SIZE > 0, "RingBuffer size must be greater than 0");

  private:
    char mBuffer[SIZE];
    std::uint8_t mBufferStart{0};
    std::uint8_t mBufferSize{0};

  public:
    RingBuffer() = default;

    void reset()
    {
        mBufferStart = 0;
        mBufferSize = 0;
    }
    void put(char c)
    {
        std::uint8_t writeIndex = (mBufferStart + mBufferSize) % SIZE;

        mBuffer[writeIndex] = c;

        if (mBufferSize < SIZE) {
            mBufferSize++;
        } else { // buffer is full
            mBufferStart = (mBufferStart + 1) % SIZE;
        }
    }
    std::uint8_t capacity() const { return SIZE; }

    std::uint8_t size() const { return mBufferSize; }

    char read(std::uint8_t pos) const
    {
        std::uint8_t readIndex = (mBufferStart + pos) % SIZE;
        return mBuffer[readIndex];
    }

    bool operator==(const char* value) const
    {
        const auto maxIdx = size();
        for (std::uint8_t idx = 0; idx < maxIdx; idx++) {
            if (*value == '\0')
                return false;
            if (read(idx) != *value)
                return false;
            value++;
        }
        if (*value != '\0')
            return false;
        return true;
    }
};

} // namespace wv::lib
