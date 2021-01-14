#include <PIDController.h>
#include <Wire.h>
volatile long int encoder_pos = 0;
PIDController pos_pid; 
PIDController speed_pid; 
int motor_mode = 0; // 0 - none, 1 - position control, 2 speed control
long int last_encoder_pos = 0;
long int last_millis = 0;
int motor_value = 255;

#define MOTOR1A 11
#define MOTOR1B 12
#define MOTOR2A 13
#define MOTOR2B 7
#define PWM1 10
#define PWM2 9
#define ENCODER1 2
#define ENCODER2 3
#define GREEN_LED 5
#define RED_LED 4
#define BUZZER 6


void setup() {
  Wire.begin(20);
  Wire.onReceive(dataReceive);
  attachInterrupt(digitalPinToInterrupt(ENCODER1), encoder, RISING);
  // initialize digital pin 13 as an output.
  pinMode(MOTOR1A, OUTPUT);
  pinMode(MOTOR1B, OUTPUT);
  pinMode(MOTOR2A, OUTPUT);
  pinMode(MOTOR2B, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(ENCODER1, INPUT);
  pinMode(ENCODER2, INPUT);

    pos_pid.begin();    
  pos_pid.tune(15, 0, 2000);    
  pos_pid.limit(-255, 255);
  pos_pid.setpoint(0);

  speed_pid.begin();   
  speed_pid.tune(10, 0, 100);    
  speed_pid.limit(-255, 255);
   speed_pid.setpoint(0); 

}


void loop() {
if(motor_mode == 1){
   motor_value = pos_pid.compute(encoder_pos);
if(motor_value > 0){
  MotorCounterClockwise(motor_value);
}else{
  MotorClockwise(abs(motor_value));
}
}else if(motor_mode == 2){
  int rpm_speed = (((float)encoder_pos - (float)last_encoder_pos) / 374.0) *60.0 * (1000/(millis()-last_millis));
int motor_value = speed_pid.compute(rpm_speed);
if(motor_value > 0){
  MotorCounterClockwise(motor_value);
}else{
  MotorClockwise(abs(motor_value));
}
  last_encoder_pos = encoder_pos;
  last_millis = millis();
  delay(20);
}
}


void encoder(){

  if(digitalRead(ENCODER2) == HIGH){
    encoder_pos++;
  }else{
    encoder_pos--;
  }
}

void MotorClockwise(int power){
  if(power > 100){
  digitalWrite(MOTOR1A, HIGH);
  digitalWrite(MOTOR1B, LOW);
  analogWrite(PWM1, power);
  }else{
    digitalWrite(MOTOR1A, LOW);
    digitalWrite(MOTOR1B, LOW);
  }
}

void MotorCounterClockwise(int power){
  if(power > 100){
  digitalWrite(MOTOR1B, HIGH);
  digitalWrite(MOTOR1A, LOW);
  analogWrite(PWM1, power);
  
  }else{
    digitalWrite(MOTOR1B, LOW);
    digitalWrite(MOTOR1A, LOW);
  }
}

void MotorStop(){
  digitalWrite(MOTOR1B, LOW);
  digitalWrite(MOTOR1A, LOW);
  pos_pid.setpoint(last_encoder_pos);
  motor_mode = 0;
}


void dataReceive(int data_length){
  int i = 0;
  
  char data[10]={};
  
  while(Wire.available()){
   data[i] = Wire.read();
   i++;
  }
  String dataString = String(data);
  if(dataString.equals("stop")){
    MotorStop();
  }else if(dataString.substring(0, 1).equals("s")){
      motor_mode = 2;
      int value = dataString.substring(1).toInt();
       speed_pid.setpoint(value); 
  }else if(dataString.substring(0, 1).equals("r")){
    motor_mode = 1;
     int value = dataString.substring(1).toInt();
     pos_pid.setpoint(last_encoder_pos + value);
  }else if(dataString.substring(0, 1).equals("l")){
    motor_mode = 1;
      int value = dataString.substring(1).toInt();
      pos_pid.setpoint(last_encoder_pos - value);
     //  digitalWrite(RED_LED, HIGH);
  }
}


