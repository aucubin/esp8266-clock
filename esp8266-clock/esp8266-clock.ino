#include <TM1637Display.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const int clk_pin = D6;
const int dio_pin = D5;

const char *ssid = "ssid";
const char *password = "password";

WiFiUDP ntpUdp;

NTPClient timeClient(ntpUdp);

TM1637Display display(clk_pin, dio_pin);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }

  timeClient.begin();

  uint8_t data[] = {0xff, 0xff, 0xff, 0xff};
  display.setBrightness(0x0f);
  display.setSegments(data);
}

void loop() {
  timeClient.update();

  int hour = timeClient.getHours();
  int minutes = timeClient.getMinutes();
  uint8 data[] = {0xff, 0xff, 0xff, 0xff};

  if(hour >= 10){
    int firstDigit = hour / 10;
    int secondDigit = hour % 10;
    data[0] = display.encodeDigit(static_cast<uint8_t>(firstDigit));
    data[1] = display.encodeDigit(static_cast<uint8_t>(secondDigit));
  }
  else{
    data[0] = display.encodeDigit(0);
    data[1] = display.encodeDigit(static_cast<uint8_t>(hour));
  }

  if(minutes >= 10){
    int firstDigit = minutes / 10;
    int secondDigit = minutes % 10;    
    data[2] = display.encodeDigit(static_cast<uint8_t>(firstDigit));
    data[3] = display.encodeDigit(static_cast<uint8_t>(secondDigit));
  }else{
    data[2] = display.encodeDigit(0);
    data[3] = display.encodeDigit(static_cast<uint8_t>(minutes));
  }

  display.setSegments(data);
  
  delay(1000);
}
