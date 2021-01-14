#include <Wire.h>

void setup() {
 Wire.begin();

Wire.beginTransmission(20);
Wire.write("s200");
Wire.endTransmission();
}

void loop() {
Wire.beginTransmission(20);
Wire.write("r1000");
Wire.endTransmission();
delay(5000);
Wire.beginTransmission(20);
Wire.write("l1000");
Wire.endTransmission();
delay(5000);
Wire.beginTransmission(20);
Wire.write("s120");
Wire.endTransmission();
delay(3000);
Wire.beginTransmission(20);
Wire.write("s200");
Wire.endTransmission();
delay(3000);
Wire.beginTransmission(20);
Wire.write("s300");
Wire.endTransmission();
delay(3000);
Wire.beginTransmission(20);
Wire.write("stop");
Wire.endTransmission();
delay(3000);




}
