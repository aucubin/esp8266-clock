#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <TZ.h>
#include <PolledTimeout.h>
#include <TM1637Display.h>

const char *ssid = "ssid";
const char *password = "password";

const int clk_pin = D6;
const int dio_pin = D5;
const uint8_t dot_bitmask = 0b01000000;

static esp8266::polledTimeout::periodicMs setTimeNow(1000);
static esp8266::polledTimeout::periodicMs checkWifiNow(30000);
static tm *local_now;

TM1637Display display(clk_pin, dio_pin);

void connectToWifi()
{
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void calcTime()
{
  time_t now = time(nullptr);
  local_now = localtime(&now);
}

void setTime()
{
  int hour = local_now->tm_hour;
  int min = local_now->tm_min;

  int time_num = (hour*100) + min;

  display.showNumberDecEx(time_num, dot_bitmask, true);
}

void setup()
{
  Serial.begin(115200);

  connectToWifi();

  configTime(TZ_Europe_Berlin, "pool.ntp.org");

  uint8_t data[] = {0xff, 0xff, 0xff, 0xff};
  display.setBrightness(0);
  display.setSegments(data);
}

void loop()
{
  if (setTimeNow)
  {
    calcTime();
    setTime();
  }

  if(checkWifiNow){
    if(WiFi.status() != WL_CONNECTED){
      connectToWifi();
    }
  }
}