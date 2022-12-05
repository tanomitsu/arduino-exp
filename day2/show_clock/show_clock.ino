#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
char ssid[] = "bld2-guest";
char pass[] = "MatsumotoWay";
const char *ntp_server = "ntp.nict.jp";
unsigned long last_sync_time = 0;
#define OLED_RESET 2
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
void setup()
{
    Serial.begin(115200);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Connecting to ");
    display.println(ssid);
    display.display();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        di splay.print(".");
        display.display();
        display.println();
        display.display();
        display.println("WiFi connected");
        display.println("IP address: ");
        display.println(WiFi.localIP());
        display.display();
        syncNTPtime();
    }
}