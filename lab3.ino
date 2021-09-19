#include <EEPROM.h>           //needed to access on

#define M1 7
#define M2 4
#define E1 6
#define E2 5
#define RIGHT 2
#define LEFT 3
#define EL 9
#define ER 10
#define PUSHPIN 12
#define TH 50
byte left_speed = 100, right_speed = 100; //PWM values from 0 to 255 


int left, right;

int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;

int ir;

void setup() {
  
  pinMode(PUSHPIN, INPUT);
  pinMode(LEFT, INPUT);
  pinMode(RIGHT, INPUT);
    
  waitButton();


  
  Serial.begin(9600);
}

void waitButton() {

  boolean running;

  while(!running) {
    
    buttonState = digitalRead(PUSHPIN);
  
    if (buttonState != lastButtonState) {
      if (buttonState == HIGH) {
        buttonPushCounter++;
     } else {
     delay(50);
    }
    lastButtonState = buttonState;
    if (buttonPushCounter % 2 == 0) {
      running = true;
      return;
      }
    }
  }
}


void forward() {
    digitalWrite(M1, HIGH);
    digitalWrite(M2, HIGH);
    analogWrite(E1, left_speed);
    analogWrite(E2, right_speed);
}

void adjustSpeed() {


  if(left == 0 || right == 0) {
    delay(500);
  }
  if(left == 0) {
    left_speed -= 10;
  }
  if(right == 0) {

    right_speed -= 10;
  }
  
  if(left == 0 && right == 0) {
    if(right_speed != 0 && left_speed != 0) {
    EEPROM.write(0, left_speed);
    EEPROM.write(1, right_speed);
    Serial.println("SAVED");
    }
    right_speed = left_speed = 0;

    delay(100);
  }
}
  
void loop() {

 
  ir = analogRead(A2);
  left = digitalRead(LEFT);
  right = digitalRead(RIGHT);

  Serial.println(ir);

 if(ir >= 600) {
    left_speed = 0;
    right_speed = 0;
  } else 
  if(ir >= 400) {
    left_speed = 60;
    right_speed = 60;
  }


  forward();
  adjustSpeed();

  // put your main code here, to run repeatedly:

}
