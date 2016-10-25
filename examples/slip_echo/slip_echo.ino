/*
This is a test sketch for the Slip library. It will simply echo back anything received.
*/

#include <Slip.h>

Slip slip; // Same as Slip slip(&Serial);

void setup() {
  Serial.begin(57600); // Initiate serial communication.
}

void loop() {

  while ( slip.parsePacket() ) { 
   
    slip.beginPacket();
    
    while ( slip.available() ) {
 
     slip.write(slip.read());

    }
    slip.endPacket();
   
  }
}

