
class Slip {

  final int SLIP_END   =  0xC0;
  final int SLIP_ESC =    0xDB;
  final int SLIP_ESC_END= 0xDC;
  final int SLIP_ESC_ESC= 0xDD;
  final int SLIP_BUFFERSIZE = 128;

  boolean  inEscaping;

  boolean needToFlush;

  boolean error;

  //the serial port used
  Serial stream;

  int in[];

  int inIndex;

  int inCount;


  /*
 CONSTRUCTOR
   */
  //instantiate with the tranmission layer
  //use Hardwarestream
  Slip(Serial s) {
    stream = s;
    flush();
    in = new int[SLIP_BUFFERSIZE];
  }

  void flush() {
    inEscaping = false;
    inIndex = 0;
    inCount = 0;
    needToFlush = false;
    error = false;
  }



  int available() {
    return inCount-inIndex;
  }

  int parsePacket() {

    if ( needToFlush ) flush();

    while ( stream.available() > 0  ) {

      int streamByte = stream.read();
      
      // Check if we've reached the end of the buffer.
      if (inCount >= SLIP_BUFFERSIZE) {
        // NOT SURE ABOUT THIS!
        error = true;
      } else {

        // Process byte.
        if ( streamByte == SLIP_END ) {


          if ( error == false ) {
            inCount = inIndex;
            inIndex = 0;
            needToFlush = true;
            return inCount;
          } else {
            flush();
          }
        } else {
          if (  inEscaping  ) {
            switch (streamByte) {
            case SLIP_ESC_END:
              store(SLIP_END);
              break;
            case SLIP_ESC_ESC:
              store(SLIP_ESC);
              break;
            default:
              error = true;
              break;
            }
            inEscaping = false;
          } else {
            if ( streamByte == SLIP_ESC ) inEscaping  = true;
            else store(streamByte);
          }
        }
      }
    }

    return 0;
  }


  //reads a byte from the buffer
  int read() {
    if ( inIndex < inCount ) {
      int  data = in[inIndex];
      inIndex++;
      return data;
    }
    return -1;
  }

  private void store(int data) {
    in[inIndex] = data;
    inIndex++;
  }

  //encode SLIP
  void write(int  b) {
    if ( b > -1 && b < 256) {
     
      if (b == SLIP_END) { 
        stream.write(SLIP_ESC);
        stream.write(SLIP_ESC_END);
        return ;
      } else if (b==SLIP_ESC) {  
        stream.write(SLIP_ESC);
        stream.write(SLIP_ESC_ESC);
        return ;
      } else {
        stream.write(b);
        return ;
      }
    } else {
      println("SLIP value "+b+" is not in range [0..255]");
    }
  }


  //SLIP specific method which begins a transmitted packet
  void beginPacket() {   
    stream.write(SLIP_END);
  }

  //signify the end of the packet with an EOT
  void endPacket() {
    stream.write(SLIP_END);
  }

  void write(int data[]) { 
    for ( int i =0; i < data.length; i++ ) {
      write(data[i]);
    }
  }
}





/*
 class Slip {
 
 final byte eot = byte(192);
 final byte slipesc = byte(219);
 final byte slipescend = byte(220);
 final byte slipescesc = byte(221);
 private byte previousByte;
 private byte[]  buffer;
 private Serial serial;
 
 Slip(Serial serial) {
 this.serial = serial;
 buffer = new byte[256];
 }
 
 // return true if the packet is complete
 boolean decode(int incoming ) {
 int previous = previousByte;
 previousByte = (byte)incoming;
 //if the previous was the escape char
 if (previous == slipesc) {
 //if this one is the esc eot
 if (incoming==slipescend) { 
 buffer.add(eot);
 } 
 else if (incoming==slipescesc) {
 buffer.add(slipesc);
 }
 } 
 else if (incoming==eot) {
 //if it's the eot
 //send off the packet
 return true;
 } 
 else if (incoming != slipesc) {
 buffer.add((byte)incoming);
 }
 return false;
 }
 
 void encode(int incoming) {
 if(incoming == eot){ 
 serial.write(slipesc);
 serial.write(slipescend); 
 } else if(incoming==slipesc) {  
 serial.write(slipesc);
 serial.write(slipescesc); 
 } else {
 serial.write(incoming);
 }  
 }
 
 }
 */