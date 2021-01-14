//ifMoistureIsBiggerThan - this value controls when to water your plant
int ifMoistureIsBiggerThan = 700;
int numberOfReadingsAboveTreshold = 0;
//this valu controls how long plants should be watered
int wateringTimeInSeconds = 120;
void setup() {

  pinMode(9, OUTPUT);
  pinMode(A0, INPUT);
  digitalWrite(9, LOW);
}

void loop() {
  //to avoid any errors system will water flower only when 10 last readings will be smaller than ifMoistureIsBiggerThan value
if(analogRead(A0) > ifMoistureIsBiggerThan){
  numberOfReadingsAboveTreshold += 1;
}else{
  numberOfReadingsAboveTreshold = 0;
}

if(numberOfReadingsAboveTreshold == 10){
  numberOfReadingsAboveTreshold = 0;
  digitalWrite(9, HIGH);
  //we have to delay in for loop just in case we would need to delay for a long time
  for(int a = 0; a < wateringTimeInSeconds; a++){
    delay(1000);
  }
  digitalWrite(9, LOW);
}

delay(10 * 1000);
}


