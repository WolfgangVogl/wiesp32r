#ifndef _MODE_PIN_HPP_
#define _MODE_PIN_HPP_

#include "arduino.hpp"
#include "global.hpp"
#include "serial.hpp"

namespace wv::wiesp {
void checkWlanAllowPin(Global& global)
{
    if (global.checkForModify > 0 && global.checkForModify < millis()) {
        global.checkForModify = 0;
        bool isHigh = (digitalRead(WGP_WLAN_ON_GPIO_PIN) == HIGH);
        if (global.allowModify != isHigh) {
            global.allowModify = isHigh;
            logger.print(millis());
            logger.print(" MODE has changed: ");
            logger.println(global.allowModify);
        }
    }
}
} // namespace wv::wiesp

#endif
