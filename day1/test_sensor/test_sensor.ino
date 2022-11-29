// 人感センサー入力プログラム
#define SENSOR 16
// 1つ前の状態
int prev_stat = LOW;
void setup() {
  pinMode(16, INPUT);
  Serial.begin(9600);
}
void loop() {
  int stat = digitalRead(SENSOR);

  if (stat == HIGH && prev_stat == LOW) {
    Serial.println("High");
  } else if (stat == LOW) {
   Serial.println("Low");
  }

  prev_stat = stat;
}
