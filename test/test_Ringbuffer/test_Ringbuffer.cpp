#include <Ringbuffer.hpp>

#include "../unittest.hpp"
#include <string>
#include <string_view>

namespace {
template<std::uint8_t LEN>
class OverwriteTest
{
  private:
    const std::string mDataBuffer;
    const std::string& mData;

    static std::string_view getExpected(const std::string& d)
    {
        // get the last n Bytes
        std::string_view result = d;

        if (result.size() < LEN)
            return result.substr(0);

        return result.substr(result.size() - LEN);
    }

  public:
    OverwriteTest(std::string&& data)
      : mDataBuffer(std::move(data))
      , mData(mDataBuffer)
    {
    }

    OverwriteTest(const std::string& data)
      : mData(data)
    {
    }

    const OverwriteTest& test() const
    {
        wv::lib::RingBuffer<LEN> buffer;
        TEST_ASSERT_EQUAL(LEN, buffer.capacity());
        for (const auto c : mData)
            buffer.put(c);
        /*

        auto expected = getExpected(mData);
        TEST_ASSERT_EQUAL(expected.size(), buffer.size());

        // std::string bufferContent{};
        char bufferContent[LEN + 1];

        for (std::uint8_t idx{0}; idx < buffer.size(); idx++) {
          bufferContent[idx] = buffer.read(idx);
        }
        // bufferContent[buffer.size() - 3] = '\0';
        bufferContent[buffer.size()] = '\0';

        TEST_ASSERT_EQUAL_STRING(std::string{expected}.c_str(), bufferContent);
        */
        return *this;
    }

    std::string_view expected() const
    {
        return getExpected(mData);
    }
};
} // namespace
// https://github.com/platformio/platformio-examples/blob/develop/unit-testing/arduino-mock/test/test_my_service.cpp
// pio run -t clean -e native
// pio test -e native

void setUp() {}
void tearDown() {}

UNITY_MAIN();

namespace wv::test::ringbuffer {

DECLARE_UNITY_TEST(put_and_read)
{
    wv::lib::RingBuffer<5> buffer;
    buffer.put('A');
    buffer.put('B');
    buffer.put('C');

    TEST_ASSERT_EQUAL('A', buffer.read(0));
    TEST_ASSERT_EQUAL('B', buffer.read(1));
    TEST_ASSERT_EQUAL('C', buffer.read(2));
}

DECLARE_UNITY_TEST(put_reset_put_read)
{
    wv::lib::RingBuffer<5> buffer;
    TEST_ASSERT_EQUAL(5, buffer.capacity());

    TEST_ASSERT_EQUAL(0, buffer.size());

    buffer.put('A');
    TEST_ASSERT_EQUAL(1, buffer.size());

    buffer.reset();
    TEST_ASSERT_EQUAL(0, buffer.size());

    buffer.put('B');
    TEST_ASSERT_EQUAL(1, buffer.size());
    TEST_ASSERT_EQUAL('B', buffer.read(0));

    buffer.put('X');
    TEST_ASSERT_EQUAL(2, buffer.size());
    TEST_ASSERT_EQUAL('B', buffer.read(0));
    TEST_ASSERT_EQUAL('X', buffer.read(1));

    TEST_ASSERT_EQUAL(5, buffer.capacity());
}

DECLARE_UNITY_TEST(simple_overwrite)
{
    wv::lib::RingBuffer<3> buffer;
    buffer.put('A');
    buffer.put('B');
    buffer.put('C');
    buffer.put('X'); // overwrites 'A'
    buffer.put('Y'); // overwrites 'Y'

    TEST_ASSERT_EQUAL('C', buffer.read(0));
    TEST_ASSERT_EQUAL('X', buffer.read(1));
    TEST_ASSERT_EQUAL('Y', buffer.read(2));
}

DECLARE_UNITY_TEST(OverwriteTest_itself)
{
    TEST_ASSERT_EQUAL_STRING(std::string{OverwriteTest<7>(std::string{"Foo"}).expected()}.c_str(), "Foo");

    TEST_ASSERT_EQUAL_STRING(std::string{OverwriteTest<3>(std::string{"Foo"}).expected()}.c_str(), "Foo");

    TEST_ASSERT_EQUAL_STRING(std::string{OverwriteTest<2>(std::string{"Bar"}).expected()}.c_str(), "ar");

    TEST_ASSERT_EQUAL_STRING(
      std::string{OverwriteTest<5>(std::string{"this is a string that does not fit into the buffer"}).expected()}
        .c_str(),
      "uffer");

    TEST_ASSERT_EQUAL_STRING(std::string{OverwriteTest<1>(std::string{"abc1"}).expected()}.c_str(), "1");

    TEST_ASSERT_EQUAL_STRING(std::string{OverwriteTest<0>(std::string{"abc1"}).expected()}.c_str(), "");
}

DECLARE_UNITY_TEST(various_overwrites)
{
    const std::string str1{"This is my test string - a lot bigger than the "
                           "buffer!"};

    OverwriteTest<1>(str1).test();
    OverwriteTest<2>(str1).test();
    OverwriteTest<3>(str1).test();
    OverwriteTest<4>(str1).test();
    OverwriteTest<5>(str1).test();
    OverwriteTest<6>(str1).test();
    OverwriteTest<7>(str1).test();

    OverwriteTest<1>(std::string{"ABC123456"}).test();

    OverwriteTest<4>(std::string{"123"}).test();
    OverwriteTest<3>(std::string{"123"}).test();
    OverwriteTest<2>(std::string{"123"}).test();
    OverwriteTest<1>(std::string{"123"}).test();
}

DECLARE_UNITY_TEST(compare_with_cstr)
{
    wv::lib::RingBuffer<5> rb;
    rb.put('A');
    rb.put('B');
    rb.put('C');
    rb.put('D');
    rb.put('E');

    TEST_ASSERT_TRUE(rb == "ABCDE");
    TEST_ASSERT_FALSE(rb == "ABCD");
    TEST_ASSERT_FALSE(rb == "ABCDEF");
    TEST_ASSERT_FALSE(rb == "BCDE");
    TEST_ASSERT_FALSE(rb == " BCDE");
    TEST_ASSERT_FALSE(rb == "ABCDE ");
    TEST_ASSERT_FALSE(rb == "ABCD ");
}

} // namespace wv::test::ringbuffer
