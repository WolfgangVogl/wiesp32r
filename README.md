# Wiesp32r

**Wiesp32r** is an access control system using a Wiegand reader and an ESP32 microcontroller.

WORK IN PROGRESS!

Stay tuned for updates and additional features in future versions!  

## Features  
- Supports Wiegand readers for access control (using the Wiegand Protocal Library for Arduino (matjack1))
- Powered by the ESP32 microcontroller
- Asynchronous web server for configuration and monitoring (esp32async/EspAsyncWebserver and AsyncTCP)

## Getting Started  
### Requirements  
- ESP32 development board.  
- Compatible Wiegand reader (e.g., RFID).  
- Basic understanding of Arduino development.  

### Setup  
1. Clone this repository.  
2. Install the required libraries via PlatformIO or your preferred development environment (see [platformio.ini](./platformio.ini))
3. Run Tests
   ```plaintext
   pio run -e native
4. Check the configuration (Pins, ...) (see [config.hpp](./src/config.hpp))
5. Build and upload to your esp32

## Licensing Information

This project is licensed under the MIT License.f

### Third-Party Dependencies
The following libraries are used in this project and are licensed under the GNU Lesser General Public License (LGPL):
- ESPAsyncWebServer (LGPL-3.0)
- AsyncTCP (LGPL-3.0-or-later)
- Wiegand Protocol Library for Arduino (LGPL-2.1-or-later)

The following libraries are used in this project and are licensed under the MIT License (MIT):
- fabiobatsilva/ArduinoFake@^0.4.0

For more details, see the [LICENSE](./LICENSE) file and [THIRD_PARTY_LICENSES](./THIRD_PARTY_LICENSES).
