#ifndef _WGP_CONFIG_HPP_
#define _WGP_CONFIG_HPP_

#define WGP_WIEGAND_GPIO_D0    18
#define WGP_WIEGAND_GPIO_D1    19

#define WGP_WLAN_ON_GPIO_PIN   34
#define WGP_RELAIS_GPIO_PIN    32
#define WGP_RELAIS_ON_TIME_MS  250

#define WGP_MAX_ACCESS_KEY_LEN 34

// WIFI configuration (values (optional) from config.local.hpp)
#undef DEF_WIFI_PASSWORD
#undef DEF_WIFI_SSID

#ifdef __has_include
#if __has_include("config.local.hpp")
#include "config.local.hpp"
#endif

#else
#warning "__has_include not supported"
#endif

#endif