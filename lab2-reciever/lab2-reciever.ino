#include "QSerial.h"

int rxpin = A1;
int txpin = -1;

QSerial myIRserial; 

void setup() {
  myIRserial.attach(rxpin, txpin);

  Serial.begin(9600);
}

void loop() {
  int val = myIRserial.receive(200);

    Serial.println(val);
  
  delay(100);

  // put your main code here, to run repeatedly:

}
