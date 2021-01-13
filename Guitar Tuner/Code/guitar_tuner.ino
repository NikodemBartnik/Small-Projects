#include "arduinoFFT.h"

/*
 * Open Source Project by Nikodem Bartnik
 * https://nikodembartnik.pl/
 * https://github.com/nikodembartnik
 * https://www.youtube.com/user/nikodembartnik
 * Feel free to make it even better
 * And don't forget to share with others :)
 */


#define SAMPLES 128         //Must be a power of 2
#define SAMPLING_FREQUENCY 1000 //Hz, must be less than 10000 due to ADC

#define STRING_E1 11
#define STRING_B 10
#define STRING_G 9
#define STRING_D 8
#define STRING_A 7
#define STRING_E2 6

#define TOO_LOW 3
#define PERFECT 4
#define TOO_HIGH 5

#define BUTTON 2
 
arduinoFFT FFT = arduinoFFT();
 
unsigned int sampling_period_us;
unsigned long microseconds;

bool ukuleleMode = false;
 
double vReal[SAMPLES];
double vImag[SAMPLES];
 
void setup() {
  
    sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
    pinMode(BUTTON, INPUT);

    pinMode(STRING_E1, OUTPUT);
    pinMode(STRING_B, OUTPUT);
    pinMode(STRING_G, OUTPUT);
    pinMode(STRING_D, OUTPUT);
    pinMode(STRING_A, OUTPUT);
    pinMode(STRING_E2, OUTPUT);

    pinMode(TOO_LOW, OUTPUT);
    pinMode(PERFECT, OUTPUT);
    pinMode(TOO_HIGH, OUTPUT);
    
}
 
void loop() {
    DisplayNoteAndBar(FindDominantFrequency());
    if(digitalRead(BUTTON) == LOW){
      delay(300);
      if(digitalRead(BUTTON) == LOW){
        ukuleleMode = !ukuleleMode;
        if(ukuleleMode){
        digitalWrite(STRING_B, HIGH);
        digitalWrite(STRING_G, HIGH);
        digitalWrite(STRING_D, HIGH);
        digitalWrite(STRING_A, HIGH);
          delay(1000);
        digitalWrite(STRING_B, LOW);
        digitalWrite(STRING_G, LOW);
        digitalWrite(STRING_D, LOW);
        digitalWrite(STRING_A, LOW);

      }else{
        digitalWrite(STRING_E1, HIGH);
        digitalWrite(STRING_B, HIGH);
        digitalWrite(STRING_G, HIGH);
        digitalWrite(STRING_D, HIGH);
        digitalWrite(STRING_A, HIGH);
        digitalWrite(STRING_E2, HIGH);
          delay(1000);
          digitalWrite(STRING_E1, LOW);
        digitalWrite(STRING_B, LOW);
        digitalWrite(STRING_G, LOW);
        digitalWrite(STRING_D, LOW);
        digitalWrite(STRING_A, LOW);
        digitalWrite(STRING_E2, LOW);

      }
    }
}
}

double FindDominantFrequency(){
    for(int i=0; i<SAMPLES; i++)
    {
        microseconds = micros();    //Overflows after around 70 minutes!
        vReal[i] = analogRead(2);
        vImag[i] = 0;
        while(micros() < (microseconds + sampling_period_us));
    }
 
    /*FFT*/
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
 
 //   Serial.println(peak);     //Print out what frequency is the most dominant.

  peak = peak * 0.990;
  return peak;
}


void DisplayNoteAndBar(double frequency){

int ArrayWithNoteAndBarWidth[2];
DetectClosestNote(frequency, ArrayWithNoteAndBarWidth);

char note = NoteNumberToString(ArrayWithNoteAndBarWidth[0]);
Serial.println(frequency);

    if(ArrayWithNoteAndBarWidth[1] < 64){
      analogWrite(TOO_LOW, map(ArrayWithNoteAndBarWidth[1], 0, 64, 255, 0));
      digitalWrite(PERFECT, LOW);
      digitalWrite(TOO_HIGH, LOW);
    }else if(ArrayWithNoteAndBarWidth[1] > 64){
      analogWrite(TOO_HIGH, map(ArrayWithNoteAndBarWidth[1], 64, 128, 0, 255));
      digitalWrite(PERFECT, LOW);
      digitalWrite(TOO_LOW, LOW);
    }else{
      digitalWrite(PERFECT, HIGH);
      digitalWrite(TOO_LOW, LOW);
      digitalWrite(TOO_HIGH, LOW);
    }

}


int* DetectClosestNote(double frequency, int *arr){
  if(ukuleleMode == false){
  if(InRange(frequency, 62, 102)){
    arr[0] = 6;
    arr[1] = map(frequency, 62, 102, 1, 128);
  }else if(InRange(frequency, 100, 120)){
    arr[0] = 5;
    arr[1] = map(frequency, 100, 120, 1, 128);
  }else if(InRange(frequency, 120, 165)){
    arr[0] = 4;
    arr[1] =  map(frequency, 127, 167, 1, 128);
  }else if(InRange(frequency, 165, 210)){
    arr[0] = 3;
    arr[1] = map(frequency, 176, 216, 1, 128);
  }else if(InRange(frequency, 210, 290)){
    arr[0] = 2;
    arr[1] = map(frequency, 217, 277, 1, 128);
  }else if(InRange(frequency, 290, 380)){
    arr[0] = 1;
    arr[1] = map(frequency, 290, 370, 1, 128);
  }
  }else{
    //notes for ukulele
   if(InRange(frequency, 420, 460)){
    arr[0] = 5;
    arr[1] = map(frequency, 420, 460, 1, 128);
  }else if(InRange(frequency, 310, 350)){
    arr[0] = 4;
    arr[1] = map(frequency, 310, 350, 1, 128);
  }else if(InRange(frequency, 232, 292)){
    arr[0] = 3;
    arr[1] =  map(frequency, 232, 292, 1, 128);
  }else if(InRange(frequency, 372, 412)){
    arr[0] = 2;
    arr[1] = map(frequency, 372, 412, 1, 128);
  }
  }
}

  bool InRange(double frequency, int low_limit, int high_limit){
    if(frequency < high_limit && frequency > low_limit){
      return true;
    }else{
      return false;
    }
  }


  char NoteNumberToString(int note_number){
  
    switch(note_number){
  case 1:
    digitalWrite(STRING_E1, HIGH);
    digitalWrite(STRING_B, LOW);
    digitalWrite(STRING_G, LOW);
    digitalWrite(STRING_D, LOW);
    digitalWrite(STRING_A, LOW);
    digitalWrite(STRING_E2, LOW);
    
    return 'E';
  break;
  case 2:
    digitalWrite(STRING_E1, LOW);
    digitalWrite(STRING_B, HIGH);
    digitalWrite(STRING_G, LOW);
    digitalWrite(STRING_D, LOW);
    digitalWrite(STRING_A, LOW);
    digitalWrite(STRING_E2, LOW);
    return 'B';
  break;
  case 3:
    digitalWrite(STRING_E1, LOW);
    digitalWrite(STRING_B, LOW);
    digitalWrite(STRING_G, HIGH);
    digitalWrite(STRING_D, LOW);
    digitalWrite(STRING_A, LOW);
    digitalWrite(STRING_E2, LOW);
    return 'G';
  break;
  case 4:
    digitalWrite(STRING_E1, LOW);
    digitalWrite(STRING_B, LOW);
    digitalWrite(STRING_G, LOW);
    digitalWrite(STRING_D, HIGH);
    digitalWrite(STRING_A, LOW);
    digitalWrite(STRING_E2, LOW);
    return 'D';
  break;
  case 5:
    digitalWrite(STRING_E1, LOW);
    digitalWrite(STRING_B, LOW);
    digitalWrite(STRING_G, LOW);
    digitalWrite(STRING_D, LOW);
    digitalWrite(STRING_A, HIGH);
    digitalWrite(STRING_E2, LOW);
    return 'A';
  break;
  case 6:
    digitalWrite(STRING_E1, LOW);
    digitalWrite(STRING_B, LOW);
    digitalWrite(STRING_G, LOW);
    digitalWrite(STRING_D, LOW);
    digitalWrite(STRING_A, LOW);
    digitalWrite(STRING_E2, HIGH);
    return 'e';
  break;
}
}
