#include <ESP8266WiFi.h>
#include <TimeLib.h>
#include <WiFiUdp.h>

void setBZ(boolean on) {
  // 事前に pinMode(14,OUTPUT) の処理が必要
  if (on) {
    digitalWrite(14, HIGH);
  } else {
    digitalWrite(14, LOW);
  }
}

boolean getPushSWStatus() {
  // 事前に pinMode(2, IN PUT) の処理が必要
  int stat = digitalRead(2);
  if (stat == LOW) {
    return true;
  } else {
    return false;
  }
}

boolean detectPushSWON() {
  // 事前に pinMode(2, IN PUT) の処理が必要
  static int prev_state = LOW;
  int current_state = digitalRead(2);
  bool res = current_state == LOW && prev_state == HIGH;
  prev_state = current_state;
  return res;
}

int getDIPSWStatus() {
  // 事前に pinMode(12, INPUT); pinMode(13, INPUT); の処理が必要
  int res = 0;
  int bit1 = digitalRead(12);
  int bit0 = digitalRead(13);
  if (bit0 == LOW) {
    res |= 0x01;
  }
  if (bit1 == LOW) {
    res |= 0x02;
  }
  return res;
}

bool getMDStatus() {
  // 事前に pinMode(16, INPUT); の処理が必要
  int stat = digitalRead(16);
  if (stat == HIGH) {
    return true;
  } else {
    return false;
  }
}

int getIlluminance() {
  int out = analogRead(A0);
  return out * 1024 / 1070;
}

boolean syncNTPtime(const char *ntp_server) {
  unsigned long unix_time = getNTPTime(ntp_server);
  if (unix_time > 0) {
    setTime(unix_time);
    return true;
  }
  return false;
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

void setUpWifi(char *ssid, char *pass) {
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
}

void setup() {
  // WiFiについてのセットアップ
  char ssid[] = "bld2-guest";
  char pass[] = "MatsumotoWay";
  syncNTPtime("ntp.nict.jp");
  Serial.begin(9600);
}

void loop() {
  unsigned long t = now();
  char str_time[30];
  sprintf(str_time, "%04d/%02d/%02d\n (%s)\n%02d:%02d:%02d", year(t), month(t),
          day(t), weekdayIntToString(weekday(t)), hour(t), minute(t),
          second(t));

  
  Serial.println(str_time);
}
