#include "arduino.hpp"
#include "config.hpp"
#include "global.hpp"
#include "serial.hpp"
#include "wlanAllowModeCheck.hpp"

#include <WiFi.h>
#include <Wiegand.h>

///////////////////////////////////////////////////////////////////////////////
// GLOBAL
namespace app = wv::wiesp;

app::Global global;

///////////////////////////////////////////////////////////////////////////////
// MAIN LOOP
void loop()
{
    namespace log = wv::lib::log;
    if (global.wiegand.available()) {
        const auto code = global.wiegand.getCode();

        logger.println("Wiegand HEX = ",
                       log::hex(code),
                       " DECIMAL = ",
                       code,
                       " CHAR = ",
                       static_cast<char>(code),
                       " UCHAR = ",
                       static_cast<unsigned char>(code),
                       " Type W",
                       global.wiegand.getWiegandType(),
                       "...");

        const auto isEnter = code == 13;
        const auto isEscape = code == 27;

        auto& ac = global.accessControl;

        if (isEscape) {
            ac.resetInput();
        } else if (isEnter) {
            if (ac.check()) {
                logger.println("HIT\n");
            } else {
                logger.println("MISS\n");
                logger.println("DATA: '", ac.data().c_str(), "'");
            }
            ac.resetInput();
        } else {
            ac.addInput(code);
        }
    }

    app::checkWlanAllowPin(global);
}

///////////////////////////////////////////////////////////////////////////////
// INTERRUPT ROUTINES
void IRAM_ATTR isrModePinChanged()
{
    global.checkForModify = millis() + 50;
}

///////////////////////////////////////////////////////////////////////////////
// SETUP Stuff
inline void setupOutPinLow(uint8_t pin)
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

inline void setupAttachInterrupt(uint8_t pin, void (*isrFct)(void), int mode)
{
    pinMode(pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(pin), isrFct, mode);
}

bool connectToWifi(const std::string& ssid, const std::optional<std::string>& password)
{
    auto timeExpired = [expire = millis() + (90 * 1000), &ssid]() {
        if (millis() > expire) {
            logger.println("WiFi - Timeout while trying to connect to SSID '", ssid.c_str(), "'");
            return true;
        }
        return false;
    };

    logger.println("WiFi - Connection to SSID '", ssid.c_str(), "', use password: ", (password ? "YES" : "NO"));

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    WiFi.setSortMethod(WIFI_CONNECT_AP_BY_SIGNAL);
    WiFi.setScanMethod(WIFI_ALL_CHANNEL_SCAN);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);

    // scan networks and output them
    while (true) {
        logger.println("WiFi scan start..");
        const auto scanAsync{false};
        const auto scanHidden{true};
        auto wifiNetworks = WiFi.scanNetworks(scanAsync, scanHidden);
        logger.println("Scan done, found ", wifiNetworks, " networks");

        for (int i = 0; i < wifiNetworks; ++i) {
            logger.println("#", i, ": ", WiFi.SSID(i), " (", WiFi.RSSI(i), ")");
            logger.println(static_cast<int>(WiFi.encryptionType(i)));
            if (WiFi.SSID(i).c_str() == ssid) {
                logger.println("Found SSID '", ssid.c_str(), "' - that's the one we're looking for");
            }
            delay(10);
        }
        if (wifiNetworks > 0) {
            break;
        }
    }

    if (timeExpired())
        return false;

    if (password) {
        WiFi.begin(ssid.c_str(), password->c_str());
    } else {
        WiFi.begin(ssid.c_str());
    }
    auto start = millis();

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        logger.print(".");

        if (timeExpired())
            return false;
    }

    logger.hr2();
    logger.println("WiFi connected");
    logger.println("IP address: ", WiFi.localIP());
    return true;
}

void setupWifi()
{
    auto loader = [](std::string_view key) -> std::optional<std::string> {
    // return global.dateStore.load(key);

#ifdef DEF_WIFI_SSID
        if (key == "wifi_ssid") {
            return DEF_WIFI_SSID;
        }
#endif
#ifdef DEF_WIFI_PASSWORD
        if (key == "wifi_password") {
            return DEF_WIFI_PASSWORD;
        }
#endif
        return std::nullopt;
    };

    logger.hr1();

    auto hasWifi{false};

    if (auto ssid = loader("wifi_ssid")) {
        hasWifi = connectToWifi(*ssid, loader("wifi_password"));
    }

    if (!hasWifi) {
        logger.println("Setting up access Point");
        logger.hr2();
        WiFi.mode(WIFI_AP);
        WiFi.softAP("garage", "123456789");
        logger.println("AP IP address: ", WiFi.softAPIP());
    }

    logger.hr1();
}

void setup()
{
    Serial.begin(115200);

    global.wiegand.begin(WGP_WIEGAND_GPIO_D0, WGP_WIEGAND_GPIO_D1);
    setupOutPinLow(WGP_RELAIS_GPIO_PIN);
    attachInterrupt(WGP_WLAN_ON_GPIO_PIN, isrModePinChanged, CHANGE);

    setupWifi();
    global.webserver.run();
}
