#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <TimeLib.h>
#include <WiFiUdp.h>

#define OLED_RESET 2
Adafruit_SSD1306 display(OLED_RESET);

char ssid[] = "bld2-guest";
char pass[] = "MatsumotoWay";

const char *ntp_server = "ntp.nict.jp";
unsigned long last_sync_time = 0;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  display.clearDisplay();
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP adress: ");
  Serial.println(WiFi.localIP());
  syncNTPtime();
}

void loop() {
  unsigned long t = now();

  char str_time[30];
  sprintf(str_time, "%04d/%02d/%02d\n (%s)\n%02d:%02d:%02d", year(t), month(t),
          day(t), weekdayIntToString(weekday(t)), hour(t), minute(t),
          second(t));
  showText(str_time);

  if (t / 300 != last_sync_time / 300) {
    syncNTPtime();
    last_sync_time = t;
  }
  delay(1000);
}

boolean syncNTPtime() {
  unsigned long unix_time = getNTPtime();
  if (unix_time > 0) {
    setTime(unix_time);
    return true;
  }
  return false;
}

unsigned long getNTPtime() {
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

void showText(char *str) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.clearDisplay();
  display.println(str);
  display.display();
}

char *weekdayIntToString(int n) {
  switch (n) {
  case 1:
    return "Sun";
  case 2:
    return "Mon";
  case 3:
    return "Tue";
  case 4:
    return "Wed";
  case 5:
    return "Thu";
  case 6:
    return "Fri";
  case 7:
    return "Sat";
  default:
    return "Err";
  }
}
