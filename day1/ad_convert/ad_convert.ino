void setup() {
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop() {
  int val = analogRead(A0);
  double lighting = (double)val / 307200000;
  Serial.println(val);
  Serial.println("Light:");
  Serial.println(lighting);
  delay(1000);
}
