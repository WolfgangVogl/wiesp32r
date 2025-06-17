#pragma once

#include <cstddef>
#include <vector>

namespace wv::lib {

class MultiBufferStream
{
  public:
    struct Buffer
    {
        const char* data;
        size_t size;
    };

    static Buffer makeBuffer(const char* data, size_t size);
    static Buffer makeBuffer(const char* data);

  private:
    std::vector<Buffer> mBuffers;
    size_t mCurrentBufferIdx{0};
    size_t mCurrentBufferPos{0};
    size_t mTotalSize{0};
    size_t mTotalRead{0};

    inline bool dataAvailable() const;

  public:
    MultiBufferStream(std::vector<Buffer> buffers);
    ~MultiBufferStream();

    int available() const;
    int read();
    int peek() const;
    size_t size() const;
    void reset();
};

} // namespace wv::lib
