#include <Servo.h>

Servo pan, tilt, grab;
int fsense = A5;

//int openclose = 0;
int force = 0;

int openclose = 180;
int ocdirection = 1;


void setup() {
  Serial.begin(9600);
  pinMode(12, OUTPUT);
  
  pan.attach(8);
  tilt.attach(9);
  grab.attach(10);
  
  grab.write(0);
  pan.write(90);
  tilt.write(160);
}

void loop() {
  force = analogRead(fsense);
  Serial.println(force);
  
  if(openclose == 180){
    openclose = 0;
    grab.write(0);
  }
  if(force <= 0){
    digitalWrite(12,HIGH);
    tilt.write(100);
    delay(200);
    openclose = 0;
    //grab.write(0);
    Serial.println("found the ball");
  }else{
    tilt.write(160);
    delay(50);
    openclose += 1;
    grab.write(openclose);
    digitalWrite(12,LOW);
  }
  
  delay(5);
}
