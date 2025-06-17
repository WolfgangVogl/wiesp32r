#include "MultiBufferStream.hpp"
#include <cstring>

namespace wv::lib {

namespace {
size_t calculateTotalSize(const std::vector<MultiBufferStream::Buffer>& buffers)
{
    size_t len = 0;
    for (const auto& buf : buffers) {
        len += buf.size;
    }
    return len;
}
} // namespace

MultiBufferStream::Buffer MultiBufferStream::makeBuffer(const char* data, size_t size)
{
    return MultiBufferStream::Buffer{.data = data, .size = size};
}

MultiBufferStream::Buffer MultiBufferStream::makeBuffer(const char* data)
{
    return MultiBufferStream::makeBuffer(data, strlen(data));
}

MultiBufferStream::MultiBufferStream(std::vector<Buffer> buffers)
  : mBuffers(std::move(buffers))
  , mTotalSize(calculateTotalSize(mBuffers))
{
}

MultiBufferStream::~MultiBufferStream() = default;

bool MultiBufferStream::dataAvailable() const
{
    return mTotalRead < mTotalSize;
}

int MultiBufferStream::available() const
{
    return mTotalSize - mTotalRead;
}

int MultiBufferStream::read()
{
    if (!dataAvailable()) {
        return -1;
    }

    auto c = mBuffers[mCurrentBufferIdx].data[mCurrentBufferPos];

    mCurrentBufferPos++;
    mTotalRead++;

    if (mCurrentBufferPos >= mBuffers[mCurrentBufferIdx].size) {
        mCurrentBufferIdx++;
        mCurrentBufferPos = 0;
    }

    return c;
}

int MultiBufferStream::peek() const
{
    if (mCurrentBufferIdx >= mBuffers.size()) {
        return -1;
    }

    return mBuffers[mCurrentBufferIdx].data[mCurrentBufferPos];
}

size_t MultiBufferStream::size() const
{
    return mTotalSize;
}

void MultiBufferStream::reset()
{
    mCurrentBufferIdx = 0;
    mCurrentBufferPos = 0;
    mTotalRead = 0;
}

} // namespace wv::lib
