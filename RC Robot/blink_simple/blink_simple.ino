void setup() {
  Serial.begin(9600);
 pinMode(9, INPUT);
pinMode(10, OUTPUT);
}

void loop() {
analogWrite(10, map(pulseIn(9, HIGH), 1100, 1900, 0, 255));
delay(20);
}
