# ESP8266 Clock
A simple clock built from an ESP8266 and a TM1637 Display

# Parts needed
- ESP8266
- TM1637 4 Segment display

# Building

- Install PlatformIO for Visual Studio Code
- Copy the creds.h.template to creds.h and enter the credentials of your WiFi
- Check if the pins for clk and dio in the code match the pins on the ESP8266 where the display is connected
- Build and flash it to your ESP8266