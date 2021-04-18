
/*
 *
 *
 */
#include <Arduino.h>
#include <TWIbits.h>
#include <TWIbuffer.h>
#include <AvrTWI.h>
#include "..\include\TIMbits.h"
#include "..\include\ESC.h"


//----------- TWI Declarations -------------
AvrTWI twi;			// two-wire interface

//------------ OLED Declarations ------------------------

extern void initDisplay(pESCdata);
extern void updateDisplay(uint8_t);

//------------ Encoder Declarations ------------------------

extern void initEncoder(pESCdata);
extern void updateEncoder(uint8_t);

//--------------- DSC Data Package ------------------------------
// Save this structure (or selected elements: Lat/Lon) to EEPROM.
ESCdata escData = {19,		// local time hours
				38,		// local time minutes
				41,		// local latitude degrees
				31,		// local latitude minutes
				-71,	// local longitude degrees
				23,		// local longitude minutes
				58,		// scope altitude degrees
				0,		// scope altitude minutes
				228,	// azimuth degrees
				0,		// azimuth minutes
				18,		// RA hours
				13,		// RA minutes
				-42,	// Dec degrees
				40,		// Dec minutes
				0		// Encoder pulses
				};
volatile uint8_t iPage;

//------------------------------------------------------------------------------
void setup() {

	sei();

// -------------------Initialize TWI. -------------------------
	twi.setup(true);		// false = slow mode (100k bps), true = fast (400k bps)
	
//-------------------- Initialize OLED ------------------------
	initDisplay(&escData);
//	initEncoder(&escData);

iPage = 0;
//-------------------- Initialize Timer 0 ------------------------
	// The following sets up a timer interrupt every 4 seconds
	TCCR0A = 0;					// Normal mode (keep counting 0-255 over-and-over). 
	TCCR0B = bCS00 + bCS02;		// clk / 1024 = 16ms = 60Hz (@16MHz crystal).
	TIMSK0 = bTOIE0;			// Set mask to interrupt when TOV0 (in TIFR) signals overflow.
	}

//------------------------------------------------------------------------------

void loop() {
	updateDisplay(iPage);
//	updateEncoder();	
	}

ISR (TIMER0_OVF_vect) {
static int8_t ovCnt;
	if (ovCnt--)
		return;
	ovCnt = 244;	// 4 sec page. Value determined from https://eleccelerator.com/avr-timer-calculator/
	iPage++;
	iPage &= 0x03;	// just 4 pages right now
	}

int main() {
	setup();
	while (1)
		loop() ;
	return 0;
	}
