#include "Slip.h"


#define SLIP_END     0xC0
#define SLIP_ESC     0xDB
#define SLIP_ESC_END 0xDC
#define SLIP_ESC_ESC 0xDD
/*
 CONSTRUCTOR
 */
//instantiate with the tranmission layer
//use Hardwarestream
Slip::Slip(Stream * s){
	stream = s;
	flush();
}


void Slip::flush() {
	inEscaping = false;
	inIndex = 0;
    inCount = 0;
    needToFlush = false;
    error = false;
}

int Slip::available(){
	return inCount-inIndex;
}

int Slip::parsePacket(){

	if ( needToFlush ) flush();
    
 while ( stream->available()  )  {
  
   int streamByte = stream->read();

		 // Check if we've reached the end of the buffer.
  if (inCount >= SLIP_BUFFERSIZE) {
    // NOT SURE ABOUT THIS!
    error = true;
   
  } else {

		    // Process byte.
		  if ( streamByte == SLIP_END ) {
		     #ifdef DEBUG_OSC_MASSENGER
		     stream->print("in: ended with size: ");
		     stream->println(inCount);
		      #endif
		    
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
int Slip::read(){
	if ( inIndex < inCount ) {
		uint8_t  data = in[inIndex];
    	inIndex++;
    	return data;
	}
    return -1;
}

void Slip::store(uint8_t data) {
	in[inIndex] = data;
	inIndex++;
}
	 



//encode SLIP
size_t Slip::write(uint8_t b){
	if(b == SLIP_END){ 
		stream->write(SLIP_ESC);
		return stream->write(SLIP_ESC_END); 
	} else if(b==SLIP_ESC) {  
		stream->write(SLIP_ESC);
		return stream->write(SLIP_ESC_ESC); 
	} else {
		return stream->write(b);
	}	
}
size_t Slip::write(const uint8_t *buffer, size_t size) { size_t result; while(size--) result = write(*buffer++); return result; }




//SLIP specific method which begins a transmitted packet
void Slip::beginPacket() { 	stream->write(SLIP_END); }

//signify the end of the packet with an EOT
void Slip::endPacket(){
	stream->write(SLIP_END);

}



