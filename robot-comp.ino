#include "QSerial.h"
#include "PathList.c"

#include <Servo.h>

QSerial ir_serial;
int M2 = 4;
int E2 = 5;
int E1 = 6;
int M1 = 7;

Servo gripper_pan;
Servo gripper_tilt;
Servo gripper;


int right_wheel_encoder = 3; //digital
int push_button = 11; //digital
int left_bumper = 2; //digital
int right_bumper = A0;
int IR_Center = A5; //analog
int IR_Left = A4; //analog
int IR_Right = A3; //analog
int force_sensor = A1; //analog
int front_IR = A2; // analog
int IR_reciever = 12;
int IR_transmitter = -1;

int left_speed = 80; //speed of left wheel
int right_speed = 75;                                                                                                                                                                                                                                                                                           ; //speed of right wheel
int wheelState = 0; //the wheel state used for encoder
int wheelTurns = 0; //amount of wheel turns using encoder
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0; //used for push button
int force = 0;
int openclose = 160;
int pan_angle = 20;


Path currPath;

int count = 0;
int pos = 0;

Path path_1;
Path path_2;
Path path_3;

int position_1[2][23] = {
   { -1, -2, 3, -1,  2, -2, 5, -1, 4, -2, 1, -1, 2, -2, 2, -1,  2, -2, -3, -3, -3, -3, -3},
   {  0, 0, -1,  0,  1,  0, 1, 0, -1, 0, -1, 0,  1,  0, -1, 0,  1, 0,  0,  0,  0,  0, 0}
  };

int position_2[2][23] = {
   { -1, -2, 4, -1, 3, -2, 4, -1, 3,  -2,  5,  2, -1, 1, 2, -2, 5, 2, -1, 1, 2, -2, -3},
   { 0, 0, -1,  0,  1,  0, 1,  0, -1,  0,  -1, 1,  0, -1, 1, 0, 1, -1, 0, 1, -1,  0,  0}
  };

int position_3[2][23] = {
  { -1, -2, 5, -1,  4, -2, 3, -1, 2, -2,  1, -1, 2, -2, 2, -1, 2, -2, -3, -3, -3, -3, -3},
   { 0, 0, -1,  0,  1,  0, 1,  0, -1, 0,  1,  0, -1, 0, 1, 0, -1, 0,  0,  0,  0,  0,  0}
  };


bool finished;

void setup() {
  // put your setup code here, to run once:
  pinMode(M2, OUTPUT);
  pinMode(E2, OUTPUT);
  pinMode(E1, OUTPUT);
  pinMode(M1, OUTPUT);
  gripper_pan.attach(8);
  gripper.attach(9);
  gripper_tilt.attach(10);
  pinMode(IR_Left, INPUT);
  pinMode(IR_Right, INPUT);
  pinMode(IR_Center, INPUT);
  pinMode(left_wheel_encoder, INPUT);
  pinMode(right_wheel_encoder, INPUT);
  pinMode(left_bumper, INPUT);
  pinMode(right_bumper, INPUT);

  pinMode(force_sensor, INPUT);
  pinMode(front_IR, INPUT);
  pinMode(push_button, INPUT);
  gripper.write(140);
  gripper_pan.write(pan_angle);
  gripper_tilt.write(85);
  ir_serial.attach(IR_reciever, IR_transmitter);

  Serial.begin(9600);

  initPaths();
  waitButton();
}

int buttonCount = 0;

Path findCurrentPath() { 

  int ticks = 0;
  while(true) {

    if(ticks >= 1000) {

    if(buttonCount == 2) {
      return path_1;
    } else if(buttonCount == 4) {
      return path_2;
    } else if(buttonCount == 6) {

      return path_3;
    }
      //buttonPushCounter = 0;
      ticks = 0;
    }

    char val = (char) ir_serial.receive(200);

    buttonState = digitalRead(push_button);

    if (buttonState != lastButtonState) {
      if (buttonState == HIGH) {
        buttonPushCounter++;
     } else {
     delay(50);
    }
    lastButtonState = buttonState;
    buttonCount++;
    }

    ticks++;


    if(val == '0') {
      return path_1;
    } else if(val == '1') {
      return path_2;
    } else if(val == '2') {
      return path_3;
    }
  }

  return path_1;
}





/**
 * Waits until button is pushed before moving
 */
void waitButton() {

  boolean running;

  while(!running) {
    
    buttonState = digitalRead(push_button);

    if (buttonState != lastButtonState) {
      if (buttonState == HIGH) {
        buttonPushCounter++;
     } else {
     delay(50);
    }
    lastButtonState = buttonState;
    if (buttonPushCounter % 2 == 0) {
      buttonPushCounter = 0;
      running = true;
      openclose = 160;
      gripper.write(openclose);
      return;
      }
    }
  }
}


  bool capturing = false;

  bool depositing = false;

int grip_tilt;

void captureLoad() {
  
  analogWrite(E1, 0);
  analogWrite(E2, 0);
     
  openclose = 40;
  pan_angle = 20;
  gripper.write(openclose);


  while(capturing) {
      pan_angle = pan_angle + 3;

    if(pan_angle >= 180) {
      pan_angle = 20;
    }

    if(pan_angle >= 130){

      openclose = 160;
      gripper.write(openclose);
      capturing = false;
      return;
    }
    delay(100);

    gripper_pan.write(pan_angle);


  }
}

void depositLoad() {



  analogWrite(E1, 0);
  analogWrite(E2, 0);

  
  openclose = 180;
  gripper.write(openclose);

  delay(100);

  openclose = 105;
  gripper.write(openclose);


  delay(50);
  
  pan_angle = 20;
  gripper_pan.write(pan_angle);

  delay(300);

  openclose = 160;
  gripper.write(openclose);
 
}


 int ticks = 0;


  
void loop() {

  
  if(finished) {
    analogWrite(E1, 0);
    analogWrite(E2, 0);
    return;
  }



  if(currPath.size == 0) {
    currPath = findCurrentPath();
    turn(0);
  }


 int FThreshold = 400;
 int F2Threshold = 400;
 int LThreshold = 925;
 int RThreshold = 925;

 int frontIRSensor = analogRead(front_IR);
 int leftSensor = analogRead(IR_Left);
 int centerSensor = analogRead(IR_Center);
 int rightSensor = analogRead(IR_Right);
 int leftBumper = digitalRead(left_bumper);
 int rightBumper = analogRead(right_bumper);



 if(frontIRSensor >= FThreshold && (leftBumper == LOW || rightBumper <= 10) && currPath.direction[pos].intersections == -1) {
    digitalWrite(M1, LOW);
    digitalWrite(M2, LOW);
    analogWrite(E1, 90);
    analogWrite(E2, 90);
    delay(300);
    capturing = true;
    captureLoad();
    delay(100);
    digitalWrite(M1, HIGH);
    digitalWrite(M2, HIGH);
    analogWrite(E1, 90);
    analogWrite(E2, 90);
    delay(300);
    analogWrite(E1, 0);
    analogWrite(E2, 0);
    delay(50);
    turn(2);
    pos++;
    count = 0;
    delay(500);
    return;
  }
 


    

  if(frontIRSensor >= F2Threshold && (leftBumper == LOW || rightBumper <= 10) && currPath.direction[pos].intersections == -2) {
    delay(200);
    depositing = true;
    depositLoad();
    digitalWrite(M1, LOW);
    digitalWrite(M2, LOW);
    analogWrite(E1, 90);
    analogWrite(E2, 90);
    delay(1000);
    turn(2);
    pos++;
    if(currPath.direction[pos].intersections == -3) {
      finished = true;
      return;
    }
    count = 0;
  }
 


   
 if(leftSensor >= LThreshold && rightSensor >= RThreshold) {
      count++;
    delay(1250);
  
  } else if(leftSensor >= LThreshold)
      {
        
         analogWrite(E1, 20);
         analogWrite(E2, 85);
         delay(40);
      }
      else if (rightSensor >= RThreshold)
      {
        
         analogWrite(E1, 85);
         analogWrite(E2, 20);
         delay(40);
   
      }


 if(count >= currPath.direction[pos].intersections) {
  if(currPath.direction[pos].intersections != -1 || currPath.direction[pos].intersections != -2) {

   switch(currPath.direction[pos].turn) {
      case -1:
      turn(0);
      pos++;
      count = 0;
      break;
      
      case 1:
      turn(1);
      pos++;
      count = 0;
      break;
   }
  }
 }

    digitalWrite(M1, HIGH);
   digitalWrite(M2, HIGH);
   analogWrite(E1, left_speed); // one of the wheels is slower then other
   analogWrite(E2, right_speed); 
}


void wheelEncoder() {
    int wheel = digitalRead(right_wheel_encoder);


    if(wheelState != wheel) {
      wheelTurns++;
      delay(50);
    }

    wheelState = wheel;

}

void initPaths() {

  
  initPath(&path_1, 30);
  addAllPaths(&path_1, position_1); 

  initPath(&path_2, 30);
  addAllPaths(&path_2, position_2);

  initPath(&path_3, 30);
  addAllPaths(&path_3, position_3);
  
  
}

void resetWheelCount() {
  wheelTurns = 0;
}



/**
 * direction 0 - left
 * direction 1 - right
 * direction 2 - 180 (off a bit)
 * direction 3 - 360 (off a bit)
 */
void turn(int direction) {
  resetWheelCount();
  int CThreshold = 940;
  int centerSensor = analogRead(IR_Center);
  boolean turning = true;
  
  while(turning) {
    wheelEncoder();
    centerSensor = analogRead(IR_Center);



   switch(direction) {
      case 0:
          if(wheelTurns < 7 || centerSensor < CThreshold) {
          digitalWrite(M1, HIGH);
          digitalWrite(M2, LOW);
          analogWrite(E1, 85);
          analogWrite(E2, 85); 
          continue;
          }          
          
          resetWheelCount();
          turning = false;
          analogWrite(E1, 0);
          analogWrite(E2, 0); 
          return;
         
      break;

      case 1:
          if(wheelTurns < 7 || centerSensor < CThreshold) {
          digitalWrite(M1, LOW);
          digitalWrite(M2, HIGH);
          analogWrite(E1, 85);
          analogWrite(E2, 85); 
            continue;
          }          
          
          resetWheelCount();
          turning = false;
          analogWrite(E1, 0);
          analogWrite(E2, 0); 
          return;
        
      break;

       case 2:
          if(wheelTurns < 17 || centerSensor < CThreshold) {
          digitalWrite(M1, LOW);
          digitalWrite(M2, HIGH);
          analogWrite(E1, 95);
          analogWrite(E2, 95); 
          continue;
          }          
          
          resetWheelCount();
          turning = false;
          analogWrite(E1, 0);
          analogWrite(E2, 0); 
          return;
      break;

        case 3:
          if(wheelTurns < 40 || centerSensor < CThreshold) {
          digitalWrite(M1, LOW);
          digitalWrite(M2, HIGH);
          analogWrite(E1, 95);
          analogWrite(E2, 95); 
           continue;
          }          
          
          resetWheelCount();
          turning = false;
          analogWrite(E1, 0);
          analogWrite(E2, 0); 
          return;
        
      break;
    }
  }
}
