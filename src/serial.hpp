#pragma once

#include "arduino.hpp"
#include <type_traits>

#ifdef DEBUG
#define LDPrint(...)   Serial.print(__VA_ARGS__)
#define LDPrintln(...) Serial.println(__VA_ARGS__)
#else
#define LDPrint(...)
#define LDPrintln(...)
#endif

namespace wv::lib::log::numeric {
template<typename T>
struct Format
{
    T value;
    int format{10};
};

template<typename T>
struct is_format : std::false_type
{};

template<typename T>
struct is_format<Format<T>> : std::true_type
{};
} // namespace wv::lib::log::numeric

namespace wv::lib::log {

template<typename T>
auto hex(T value)
{
    return numeric::Format<T>{.value = value, .format = HEX};
}

class Logger
{
  private:
    template<typename T>
    void printImpl(const T& value)
    {
        LDPrint(value);
    }
    template<typename T>
    void printImpl(numeric::Format<T> data)
    {
        LDPrint(data.value, data.format);
    }

  public:
    template<typename... T>
    void print(const T&... t)
    {
        if constexpr (sizeof...(t) > 0) {
            (printImpl(t), ...);
        }
    }

    template<typename... T>
    void println(const T&... t)
    {
        (printImpl(t), ...);
        printImpl('\n');
    }

    void hr1()
    {
        printImpl("=======================================\n");
    }
    void hr2()
    {
        printImpl("-----\n");
    }
};
} // namespace wv::lib::log

static wv::lib::log::Logger logger{};
