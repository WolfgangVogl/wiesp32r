# Wiesp32r  
**Version 0.0.0a**  

**Wiesp32r** is an access control system leveraging a Wiegand reader and an ESP32 microcontroller to deliver flexible and customizable access management.  

🚧 **WORK IN PROGRESS! NOT PRODUCTION READY** 🚧  


## Features  
### Implemented  
- **Wiegand Reader Support**: Utilizes the Wiegand Protocol Library for Arduino ([matjack1](https://github.com/matjack1/WiegandProtocol)).  
- **ESP32-Powered**: Leverages the capabilities of the ESP32 microcontroller.  
- **Asynchronous Web Server**: Configuration and monitoring using `ESPAsyncWebServer` and `AsyncTCP`.  
- **Hotspot Mode**: Automatically creates a Wi-Fi hotspot if no known network is available.  

### Planned Features  
- **Web Interface Enhancements**:  
  - Discover and connect to Wi-Fi networks with credential storage.  
  - Set a custom password for the web interface.  
- **Wi-Fi Opt-In via Jumper**: Allow manual activation of Wi-Fi capabilities using a hardware jumper.  
- **User Account Management**:  
  - Create and manage user accounts with access methods:  
    - PIN-based access.  
    - RFID-based access.  
    - Combined PIN + RFID access.  
  - Support for one-time access PIN codes.  

### Potential Features (Under Consideration)  
- **Access Event Notifications**:  
  - Trigger external endpoints when access is granted.  
  - Supported protocols:  
    - HTTP requests (e.g., JSON payloads).  
    - MQTT messaging.  
- **Time-Based Access Control**:  
  - Set time restrictions for user accounts and one-time PINs.  
- **Advanced PIN Features**:  
  - Use PIN codes for time-limited or conditional access.  

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

## Licensing  

### Project License  
This project is licensed under the **MIT License**. See the [LICENSE](./LICENSE) file for full details.  

### Third-Party Licenses  
This project includes the following third-party libraries:  
- **[ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)**  
  - Licensed under LGPL-3.0.
- **[AsyncTCP](https://github.com/me-no-dev/AsyncTCP)**  
  - Licensed under LGPL-3.0-or-later.
- **[Wiegand Protocol Library for Arduino](https://github.com/matjack1/WiegandProtocol)**  
  - Licensed under LGPL-2.1-or-later.
- **[ArduinoFake](https://github.com/FabioBatSilva/ArduinoFake)**  
  - Licensed under MIT

See [THIRD_PARTY_LICENSES](./THIRD_PARTY_LICENSES).
Refer to the respective repositories for more details on these licenses.  
