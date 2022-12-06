#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <TimeLib.h>
#include <WiFiUdp.h>

// initialize for displaying
#define OLED_RESET 2
Adafruit_SSD1306 display(OLED_RESET);

// wifi settings
char ssid[] = "bld2-guest";
char pass[] = "MatsumotoWay";
char host[] = "http://iot.hongo.wide.ad.jp/";
char ntphost[] = "ntp.nict.jp";

void setup() {
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // WiFi接続を試みる
  setUpWifi(ssid, pass);
  // 時刻を合わせる
  syncNTPtime(ntphost);
}

void loop() {}

void setUpWifi(char *ssid, char *pass) {
  displayText("Connecting to" + String(ssid))
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  for (int count = 0; count < 20 && WiFi.status() != WL_CONNECTED; count++) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Succeeded connecting to WiFi.");
    displayText("IP Address:\n " + ipToStr(WiFi.localIP()), 1);
    return;
  } else {
    Serial.println("Failed connecting to WiFi.");
    return;
  }
}

void displayText(String str, int textSize) {
  display.setTextSize(textSize);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.clearDisplay();
  display.println(str);
  display.display();
}

String ipToStr(const IPAddress &ip) {
  return String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." +
         String(ip[3]);
}

boolean syncNTPtime(const char *ntp_server) {
  unsigned long unix_time = getNTPTime(ntp_server);
  if (unix_time > 0) {
    setTime(unix_time);
    Serial.println("Succeeded getting time.");
    return true;
  } else {
    Serial.println("Failed getting time.");
    return false;
  }
}

unsigned long getNTPTime(const char *ntp_server) {
  // 事前に WiFi に関する接続処理が行われている必要がある
  WiFiUDP udp;
  udp.begin(8888);
  unsigned long unix_time = 0UL;
  byte packet[48];
  memset(packet, 0, 48);
  packet[0] = 0b11100011;
  packet[1] = 0;
  packet[2] = 6;
  packet[3] = 0xEC;
  packet[12] = 49;
  packet[13] = 0x4E;
  packet[14] = 49;
  packet[15] = 52;

  Serial.println("connecting to" + String(ntp_server) + "...");
  udp.beginPacket(ntp_server, 123);
  udp.write(packet, 48);
  udp.endPacket();

  for (int i = 0; i < 10; i++) {
    delay(500);
    if (udp.parsePacket()) {
      udp.read(packet, 48);
      unsigned long highWord = word(packet[40], packet[41]);
      unsigned long lowWord = word(packet[42], packet[43]);
      unsigned long secsSince1900 = highWord << 16 | lowWord;
      const unsigned long seventyYears = 2208988800UL;
      unix_time =
          secsSince1900 - seventyYears + 32400UL; // 32400 = 9 hours(JST)
      break;
    }
  }
  udp.stop();
  return unix_time;
}