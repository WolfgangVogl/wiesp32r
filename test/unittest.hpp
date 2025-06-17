#pragma once

#ifdef USE_ARDUINO_FAKE
#include <ArduinoFake.h>
#endif

#include <unity.h>
#ifdef USE_ARDUINO_FAKE
using namespace fakeit;
#endif
#include <vector>

#ifdef DECLARE_TEST
#undef DECLARE_TEST
#endif

typedef void (*TestFunction)();
static std::vector<TestFunction> test_list;

#define DECLARE_UNITY_TEST(test_name)                                                                                  \
    void test_##test_name();                                                                                           \
    void test_exec_##test_name();                                                                                      \
    static struct test_##test_name##_registrar                                                                         \
    {                                                                                                                  \
        test_##test_name##_registrar()                                                                                 \
        {                                                                                                              \
            test_list.push_back(test_exec_##test_name);                                                                \
        }                                                                                                              \
    } test_##test_name##_instance;                                                                                     \
    void test_exec_##test_name()                                                                                       \
    {                                                                                                                  \
        RUN_TEST(test_##test_name);                                                                                    \
    }                                                                                                                  \
    void test_##test_name()

#define UNITY_RUN_TESTS                                                                                                \
    for (TestFunction test : test_list) {                                                                              \
        test();                                                                                                        \
    }

#define UNITY_MAIN()                                                                                                   \
    int main(int argc, char** argv)                                                                                    \
    {                                                                                                                  \
        UNITY_BEGIN();                                                                                                 \
        UNITY_RUN_TESTS;                                                                                               \
        UNITY_END();                                                                                                   \
    }
