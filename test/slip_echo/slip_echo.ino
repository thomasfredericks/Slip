#include <Slip.h>

Slip slip(&Serial);

void setup() {
  Serial.begin(57600); // Initiate serial communication.
}

void loop() {

  while ( slip.parsePacket() ) { 
    //Serial.print("data: ");
    slip.beginPacket();
    while ( slip.available() ) {
     // Serial.print( slip.read() );
     // Serial.print(' ');
     slip.write(slip.read());
    }
    slip.endPacket();
    //Serial.println();
  }
}


