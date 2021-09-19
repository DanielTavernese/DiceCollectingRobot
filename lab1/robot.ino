#define M1 7
#define M2 4
#define E1 6
#define E2 5
#define L 2
#define LED 12
#define LTHRESH 1000
#define CTHRESH 1000
#define RTHRESH 1000
#define PUSHPIN 13
#define EL 2
#define ER 3

int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;
int count = 0;
int lt;
int rt;
int ct;
bool clicked = false;
bool motorrun = false;


int LCount = 0;
int RCount = 0;
int cur = 0;

int l;


int countEncoder() {
  if(digitalRead(EL) == 1) {
    LCount++;
  }
  if(digitalRead(ER) == 1) {
    RCount++;
  }
  return LCount;
}


void setup() {
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  
  pinMode(L, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(PUSHPIN, INPUT);

  Serial.begin(9600);
}

void loop() { 
 buttonState = digitalRead(PUSHPIN);
 lt = analogRead(L);


  
//task 5
  if(motorrun){
    while (true){
       cur = countEncoder();
       Serial.println(cur);
      if(cur < 250){
        Serial.print("Forward");
          digitalWrite(M1, HIGH);
          digitalWrite(M2, HIGH);
          analogWrite(E1, 100);
          analogWrite(E2, 100);
        } else if(cur < 450){
        Serial.print("Back");
          digitalWrite(M1, LOW);
          digitalWrite(M2, LOW);
          analogWrite(E1, 100);
          analogWrite(E2, 100);
      }else if(cur < 525){
        Serial.print("spin");
          digitalWrite(M1, HIGH);
          digitalWrite(M2, LOW);
          analogWrite(E1, 100);
          analogWrite(E2, 100);        
      }else{
        analogWrite(E1, 0);
        analogWrite(E2, 0);
        Serial.print("cleanup");
        motorrun = false;
        LCount = 0;
        RCount = 0;
        cur = 0;
        break;
      }
    }  
  }


//task 3
  if(lt >= 950) {
    analogWrite(E1, 0);
    analogWrite(E2, 0);
  }



// task 4
  digitalWrite(LED, l == 1 ? HIGH : LOW);
  
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;
    } else {
      // if the current state is LOW then the button went from on to off:    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;


  // turns on the LED every four button pushes by checking the modulo of the
  // button push counter. the modulo function gives you the remainder of the
  // division of two numbers:
  if (buttonPushCounter % 2 == 0) {

    // task 1
    motorrun = true;
    
    /*digitalWrite(M1, HIGH);
    digitalWrite(M2, HIGH);
    analogWrite(E1, 100);
    analogWrite(E2, 100);*/

  } else {
     analogWrite(E1, 0);
    analogWrite(E2, 0);
      }
  }
}
