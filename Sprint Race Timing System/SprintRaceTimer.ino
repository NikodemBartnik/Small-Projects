/*
 * Epaper BUSY -> Arduino D7
 * Epaper RST -> Arduino D8
 * Epaper DC -> Arduino D9
 * Epaper CS -> Arduino D10
 * Epaper CLK -> Arduino D13
 * Epaper DIN -> Arduino D11
 * Epaper GND -> Arduino GND
 * Epaper VCC -> Arduino 3.3V
 * 
 * HC-SR04 VCC - > Arduino 5V
 * HC-SR04 GND - > Arduino GND
 * HC-SR04 TRIG - > Arduino D2
 * HC-SR04 ECHO - > Arduino D3
 * 
 * Buzzer + -> Arduino A0
 * Buzzer - -> Arduino GND
 * 
 * Buttons: D4, D5, D6
 * 
 */

#define TRIG 2
#define ECHO 3

#include <SPI.h>
#include "epd1in54.h"
#include <epdpaint.h>
#include "imagedata.h"

unsigned char image[1024];
Paint paint(image, 0, 0);    // width should be the multiple of 8 
Epd epd;
unsigned long runTimeStart;
unsigned long runTimeMs;
int runNumber = 0;
char buf[4];
double times[10] = {9.567, 9.028, 10.203, 53.203, 18.032, 7.392, 1.382, 3.3943, 14.432, 13.230};
unsigned int distance = 100;
bool timeMeasuring = false;
bool timesDisplayed = false;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
   if (epd.Init(lut_full_update) != 0) {
      Serial.print("e-Paper init failed");
      return;
  }

  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();

  pinMode(6, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(A0, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  DisplayStartMenu(distance);
  if (epd.Init(lut_partial_update) != 0) {
      Serial.print("e-Paper init failed");
      return;
  }
}

void loop() {
//center button
if(digitalRead(6) == LOW){
  if(timesDisplayed == false){
  PrepareForRun();
  }else{
    timesDisplayed = false;
    DisplayStartMenu(distance);
    delay(1000);
  }
}
//red button
if(digitalRead(5) == LOW){
  if(distance > 50){
  distance-=50;
  }
  UpdateDistance(distance);
  delay(500);
}
//green button
if(digitalRead(4) == LOW){
  distance+=50;
  UpdateDistance(distance);
  delay(500);
}   
}


void DisplayTimes(int time_number){
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  timesDisplayed = true;
  paint.SetRotate(ROTATE_0);
  paint.SetWidth(200);
  paint.SetHeight(20);
  
paint.Clear(0);
  paint.DrawStringAt(40, 4, "Time", &Font16, 1);
  epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  for(int a = 0; a < 10; a++){
    paint.Clear(0);
  paint.SetWidth(25);
  paint.SetHeight(22);
  paint.DrawStringAt(10, 1, itoa(a, buf, 10), &Font16, 1);
  epd.SetFrameMemory(paint.GetImage(), 0, 22+a*18, paint.GetWidth(), paint.GetHeight());
  }

 
  paint.SetWidth(80);
  paint.SetHeight(20);
 for(int a = 0; a < time_number; a++){
  paint.Clear(1);
  String runtime = "";
  runtime.concat(times[a]);
  runtime += "s";
  paint.DrawStringAt(0, 0, runtime.c_str(), &Font16, 0);
  epd.SetFrameMemory(paint.GetImage(), 40, 23+a*18, paint.GetWidth(), paint.GetHeight());
 }
 
   epd.DisplayFrame();
}


void DisplayStartMenu(int _distance){
   paint.SetRotate(ROTATE_0);
  paint.SetWidth(200);
  paint.SetHeight(20);
  
paint.Clear(1);
  paint.DrawStringAt(40, 0, "I will run", &Font16, 0);
  epd.SetFrameMemory(paint.GetImage(), 0, 20, paint.GetWidth(), paint.GetHeight());
  
    paint.Clear(1);
  paint.SetWidth(64);
  paint.SetHeight(24);
  String text = itoa(_distance, buf, 10);
  text += "m";
  paint.DrawStringAt(0, 0, text.c_str(), &Font24, 0);
  epd.SetFrameMemory(paint.GetImage(), 70, 80, paint.GetWidth(), paint.GetHeight());
  

 paint.Clear(1);
  paint.SetWidth(64);
  paint.SetHeight(20);
  
  paint.DrawStringAt(0, 0, "-", &Font16, 0);
  epd.SetFrameMemory(paint.GetImage(), 20, 180, paint.GetWidth(), paint.GetHeight());
  paint.Clear(1);
  paint.DrawStringAt(0, 0, "START", &Font16, 0);
  epd.SetFrameMemory(paint.GetImage(), 75, 180, paint.GetWidth(), paint.GetHeight());
  paint.Clear(1);
  paint.DrawStringAt(0, 0, "+", &Font16, 0);
  epd.SetFrameMemory(paint.GetImage(), 180, 180, paint.GetWidth(), paint.GetHeight());
 
 
   epd.DisplayFrame();

}


void UpdateDistance(int _distance){
   String text = itoa(_distance, buf, 10);
  text += "m";
//  paint.SetRotate(ROTATE_0);
  paint.SetWidth(80);
  paint.SetHeight(24);
  paint.Clear(1);
  paint.DrawStringAt(0, 0, text.c_str(), &Font24, 0);
  epd.SetFrameMemory(paint.GetImage(), 70, 80, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
  epd.SetFrameMemory(paint.GetImage(), 70, 80, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
}


//preaparing for run, counting down and beeping the buzzer
void PrepareForRun(){
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  paint.SetWidth(120);
  paint.SetHeight(24);
  paint.Clear(1);
  paint.DrawStringAt(0, 0, "Prepare", &Font24, 0);
  epd.SetFrameMemory(paint.GetImage(), 40, 90, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
  epd.SetFrameMemory(paint.GetImage(), 40, 90, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
  digitalWrite(A0, HIGH);
    delay(200);
    digitalWrite(A0, LOW);
    delay(200);
    digitalWrite(A0, HIGH);
    delay(200);
    digitalWrite(A0, LOW);
    delay(200);
  for(int a = 0; a < distance/5; a++){
   delay(1000);
  }
  for(int b = 0; b < 10; b++){
    digitalWrite(A0, HIGH);
    delay(100);
    digitalWrite(A0, LOW);
    delay(100);
  }
  digitalWrite(A0, HIGH);
  delay(1000);
  digitalWrite(A0, LOW);
  MeasureRunTime();
}


void MeasureRunTime(){
  timeMeasuring = true;
  runTimeStart = millis();
  while(MeasureDistance() > 100){
    delay(10);
  }
  runTimeMs = millis() - runTimeStart;
  times[runNumber] = (double)runTimeMs/1000;
  Serial.print("Time: ");
  Serial.println((double)runTimeMs/1000);
  timeMeasuring = false;
  runNumber++;
  DisplayTimes(runNumber);
}

float MeasureDistance(){
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(5);
  digitalWrite(TRIG, LOW);

  float t = pulseIn(ECHO, HIGH);
  float distance = t*0.017015;
  Serial.print("Distance: ");
  Serial.println(distance);
  return distance;
}






