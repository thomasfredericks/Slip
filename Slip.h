/*
Extends the Serial class to encode SLIP over serial
*/

#ifndef Slip_h
#define Slip_h


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Stream.h>

#define SLIP_BUFFERSIZE 128

class Slip {
	

public:
	
	//the serial port used
	Slip(Stream * );
    
    // Checks for the presence of a SLIP packet, and reports the size. parsePacket() must be called before reading the buffer 
	int parsePacket();
    
    // Get the number of bytes available for reading from the buffer. This is data that's already arrived. 
	int available();

    // Reads the data and returns the next character in the buffer.  
	int read();

    //SLIP specific method which begins a transmitted packet
	void beginPacket();
	
	//SLIP specific method which ends a transmittedpacket
	void endPacket();

	size_t write(uint8_t b);

    size_t write(const uint8_t *buffer, size_t size);

private:
	bool  inEscaping;

	bool needToFlush;

	bool error;
	
	//the serial port used
	Stream * stream;
    
   // bool receivedData;

	void store(uint8_t data);

    uint8_t in[SLIP_BUFFERSIZE];

    uint8_t inIndex;

    uint8_t inCount;

    void flush();



};


#endif