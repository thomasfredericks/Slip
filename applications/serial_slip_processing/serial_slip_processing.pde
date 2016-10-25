import processing.serial.*;

String port = "COM6";
Serial serial;
Slip slip;

void setup() {
  size(400, 400);
  Serial.list();
  println("Serial opening port : "+port);
  serial = new Serial(this, port, 57600);
  slip = new Slip(serial);
}

void draw() {
  if ( slip.parsePacket( ) > 0 ) {
    println("Received");
    while ( slip.available() > 0 ) {
      println(slip.read());
    }
  }
}

void mousePressed() {
  println("Sending");
  slip.beginPacket();
  slip.write(millis()%256);
  slip.write(100);
  slip.write(94);
  slip.endPacket();
}