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

void setup() {
  pinMode(16, INPUT);
  Serial.begin(9600);
}

void loop() {
  delay(100);
  bool sensor = getMDStatus();
  if (sensor) {
    Serial.println("Detected!");
  }
}
