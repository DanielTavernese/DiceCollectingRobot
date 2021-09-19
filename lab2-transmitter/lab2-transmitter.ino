
#include "QSerial.h"

int rxpin = -1;
int txpin = A0;

QSerial myIRserial; 

void setup() {
  myIRserial.attach(rxpin, txpin);
}

void loop() {
  myIRserial.transmit('q');

  delay(100);

  
  // put your main code here, to run repeatedly:

}
