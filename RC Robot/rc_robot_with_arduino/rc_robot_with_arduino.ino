int ENA = 5;
int ENB = 6;
int INPUT2 = 7;
int INPUT1 = 8;
int INPUT3 = 12;
int INPUT4 = 13;  
int RightMotor = 0;
int LeftMotor = 0;
int SpeedValue = 0;
void setup() {
  Serial.begin(9600);
pinMode(5, OUTPUT);
pinMode(6, OUTPUT);
pinMode(7, OUTPUT);
pinMode(8, OUTPUT);
pinMode(12, OUTPUT);
pinMode(13, OUTPUT);


pinMode(9, INPUT);
pinMode(10, INPUT);
digitalWrite(INPUT1,LOW);
digitalWrite(INPUT2,HIGH);
digitalWrite(INPUT3,LOW);
digitalWrite(INPUT4,HIGH);

}

void loop() {
  SpeedValue = map(pulseIn(9, HIGH), 1100, 1900, 0, 255);
  RightMotor = SpeedValue - map(pulseIn(10, HIGH), 1000, 2000, -70, 70);
  LeftMotor = SpeedValue - map(pulseIn(10, HIGH), 1000, 2000, +70, -70);
Serial.print("RIGHT MOTOR: ");
Serial.print(RightMotor);
Serial.print("  LEFT MOTOR: ");
Serial.println(LeftMotor);
if(RightMotor < 0){
  RightMotor = 0;
}
if(LeftMotor < 0){
  LeftMotor = 0;
}
analogWrite(5, LeftMotor);
analogWrite(6, RightMotor);
}

void Forward(){
  
}

