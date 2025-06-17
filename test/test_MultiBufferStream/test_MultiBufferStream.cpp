#define USE_ARDUINO_FAKE

#include "../unittest.hpp"
#include <MultiBufferStream.hpp>

// https://github.com/platformio/platformio-examples/blob/develop/unit-testing/arduino-mock/test/test_my_service.cpp
// pio run -t clean -e native
// pio test -e native
// clear; pio test -e native --filter test_MultiBufferStream
// claer; pio test -e native --without-testing --without-uploading

#include <cstring>
#include <string>
#include <string_view>

namespace {
std::string readToString(wv::lib::MultiBufferStream& stream, int max = 128)
{
    std::string result;
    result.reserve(std::min(max, stream.available()));

    for (std::size_t i = 0; i < max; i++) {
        if (!stream.available())
            break;
        result += stream.read();
    }
    return result;
}

wv::lib::MultiBufferStream::Buffer makeBuffer(const char* data)
{
    return wv::lib::MultiBufferStream::makeBuffer(data);
}
} // namespace

void setUp() {}
void tearDown() {}

UNITY_MAIN();

namespace wv::test::multibufferstream {

DECLARE_UNITY_TEST(no_buffer)
{
    wv::lib::MultiBufferStream stream({});

    TEST_ASSERT_EQUAL(0, stream.available());
    TEST_ASSERT_EQUAL_STRING("", readToString(stream).c_str());
    TEST_ASSERT_EQUAL(0, stream.available());

    TEST_ASSERT_EQUAL_STRING("", readToString(stream).c_str());
    TEST_ASSERT_EQUAL(0, stream.available());
}

DECLARE_UNITY_TEST(one_buffer_but_empty)
{
    wv::lib::MultiBufferStream stream({makeBuffer("")});

    TEST_ASSERT_EQUAL(0, stream.available());
    TEST_ASSERT_EQUAL_STRING("", readToString(stream).c_str());
    TEST_ASSERT_EQUAL(0, stream.available());

    TEST_ASSERT_EQUAL_STRING("", readToString(stream).c_str());
    TEST_ASSERT_EQUAL(0, stream.available());
}

DECLARE_UNITY_TEST(one_buffer_single_char)
{
    wv::lib::MultiBufferStream stream({{.data = "A", .size = 1}});

    TEST_ASSERT_EQUAL(1, stream.available());
    TEST_ASSERT_EQUAL_STRING("A", readToString(stream).c_str());
    TEST_ASSERT_EQUAL(0, stream.available());

    TEST_ASSERT_EQUAL_STRING("", readToString(stream).c_str());
    TEST_ASSERT_EQUAL(0, stream.available());
}

DECLARE_UNITY_TEST(multi_buffers)
{
    wv::lib::MultiBufferStream stream({makeBuffer("This"), makeBuffer(" is"), makeBuffer("just A TestCase!")});

    TEST_ASSERT_EQUAL_STRING("This isjust A TestCase!", readToString(stream).c_str());
    TEST_ASSERT_EQUAL(0, stream.available());

    TEST_ASSERT_EQUAL_STRING("", readToString(stream).c_str());
    TEST_ASSERT_EQUAL(0, stream.available());
}

DECLARE_UNITY_TEST(partial_buffer_read_reset)
{
    const auto buff_A = "Just a Question: ";
    const auto buff_B = "How do you know if a C++ developer is qualified? I mean, really!";
    const auto buff_C = "Thats the Answer!";
    const auto buff_D = "Really! You look at their CV.";

    wv::lib::MultiBufferStream stream({{.data = buff_A + 7, .size = 1},
                                       {.data = buff_A + 15, .size = 2},
                                       {.data = buff_B, .size = 48},
                                       {.data = buff_A + 16, .size = 1},
                                       {.data = buff_A + 16, .size = 1},
                                       {.data = buff_C + 10, .size = 1},
                                       {.data = buff_A + 15, .size = 2},
                                       {.data = buff_D + 8, .size = 21}});

    const auto expected = "Q: How do you know if a C++ developer is qualified?  A: You look at their CV.";
    TEST_ASSERT_EQUAL_STRING(expected, readToString(stream).c_str());
    TEST_ASSERT_EQUAL(0, stream.available());

    TEST_ASSERT_EQUAL_STRING("", readToString(stream).c_str());
    TEST_ASSERT_EQUAL(0, stream.available());

    stream.reset();
    TEST_ASSERT_EQUAL_STRING(expected, readToString(stream).c_str());
    TEST_ASSERT_EQUAL(0, stream.available());
}

DECLARE_UNITY_TEST(read_after_end)
{
    wv::lib::MultiBufferStream stream({makeBuffer("just some data"), makeBuffer("... and some more")});

    TEST_ASSERT_EQUAL_STRING("just some data... and some more", readToString(stream).c_str());
    TEST_ASSERT_EQUAL(0, stream.available());

    TEST_ASSERT_EQUAL(-1, stream.read());
    TEST_ASSERT_EQUAL(0, stream.available());
}

} // namespace wv::test::multibufferstream
