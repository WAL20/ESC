/*
 * Display.cpp
 *
 * Created: 2/28/2021 10:25:40 AM
 *  Author: Sisyphus
 */ 
#include <Arduino.h>

#include <TWIbuffer.h>
#include <TWI.h>
#include <twiOLED.h>
#include "..\include\ESC.h"

//----------- TWI Declarations -------------

extern TWI twi;			// two-wire interface

//----------- OLED Declarations -------------

uint8_t OLEDdata[254];
TWIbuffer OLEDbuf(OLEDdata,254);
TWIbuffer *pOLEDbuf=&OLEDbuf;

#define OLED_ADDRESS 0x3C
twiOLED oled(pOLEDbuf);		// OLED display object

pESCdata pESC;
//--------------------------------------------------

void initDisplay(ESCdata *pesc) {
	pESC = pesc;
	oled.setup();	// This will put OLED initialization data into buffer.
	while (!twi.write(OLED_ADDRESS, pOLEDbuf)) ;	// Write the buffered initialization data to OLED via TWI.
	}

bool clearDisplay(bool fromthetop) {
	static uint8_t step;
	static uint8_t *pRow;
	if (fromthetop)
		step=0;
	switch(step) {
		case 0:
			oled.setCursor(0,0);
			pRow=pOLEDbuf->getAddr(pOLEDbuf->getPtr()-1);	// point to setRow operand (the row number to clear).
			oled.clear();				// Fill buffer with graphical spaces - enough to clear a screen row.
			step++;						// Advance to next step.
		case 1:
		case 4:
			if (twi.write(OLED_ADDRESS, pOLEDbuf))	// When buffer goes on its way...
				step++;								//     set to advance to next step.
			break;							// Success or failure, do something else, then come back later.
		case 2:
			(*pRow)++;					// Bump the cursor to next row. Just stuff the hex value.
			if (((*pRow) & 0x0f) < (twiOLED_HEIGHT/8))
				step--;					// One step back.
			else
				step++;					// One step forward.
			break;
		case 3:							// As a courtesy to the caller, put the cursor in the upper corner.
			oled.setCursor(0,0);		// Call the function so side effects (keeping track of cursor position) can happen.
			step++;
			break;				// Almost done.
		case 5:
		default:
			return true;				// Finally done here. Caller can stop repeat calls.
		}
	return false;
	}


void titlePage(bool bNew) {
	static uint8_t step;
	if (bNew)					// Caller will indicate if this is a new page (needing clearing).
		step = 0;				//		abandon any in-progress activity and start over.
	switch(step) {				// Pick up where we left off.
		case 0:					// Start at the beginning by clearing previous screen detritus.
			if (clearDisplay(bNew))	// Several calls to clearDisplay() are required. The last one returns true...
				step++;				//    ... when it's finished, move on.
			break;
		case 1:
			oled.setCursor(0,47);
			oled.writeChar("ESC", DOUBLEWIDE | BOXBOTTOM | BOXLEFT | BOXRIGHT);
			oled.setCursor(2,20);
			oled.writeChar("Electronic Setting");
			step++;						// Case 1 always completes - go directly to case 2.
		case 2:
		case 4:
			if (twi.write(OLED_ADDRESS, pOLEDbuf))	// If writing succeeds...
				step++;								//   ... next step.
			break;
		case 3:
			oled.setCursor(3,48);
			oled.writeChar("Circle");
			if (twiOLED_HEIGHT > 32) {				// If there's more room...
				oled.setCursor(5,32);				//     add a by-line
				oled.writeChar("by Sisyphus");
				}
			step++;
		default:
			break;
		}	
	}

void setTimePage(bool bNew) {		// Titlepage comments apply here, too.
	static uint8_t step;
	char ch[3];
	if (bNew)
		step=0;
	switch(step) {
		case 0:
			if (clearDisplay(bNew))
				step++;
			break;
		case 1:	
			oled.setCursor(0,2);
			oled.writeChar("Local Sidereal Time", UNDERSCORE);
			step++;
		case 2:
		case 4:
			if (twi.write(OLED_ADDRESS, pOLEDbuf))
				step++;
			break;
		case 3:
			oled.setCursor(2,50);
			itoa(pESC->ilocalHour, ch,10);		// Convert integer to ASCII
			oled.rJustify(ch,3);					// Right justify in 3-byte field.
			oled.writeChar(ch,DOUBLESIZE);
			oled.setRow(2);
			itoa(pESC->ilocalMinute, ch,10);
			oled.rJustify(ch,3,'0');				// Right justify with leading zero.
			oled.writeChar(ch,DOUBLEWIDE);
			step++;
		default:
			break;
		}
	}

void setLocationPage(bool bNew) {		// Titlepage comments apply here, too.
	static uint8_t step;
	char ch[5];
	if (bNew)
		step=0;
	switch(step) {
		case 0:
			if (clearDisplay(bNew))
				step++;
			break;
		case 1:
			oled.setCursor(0,10);
			oled.writeChar("Location", UNDERSCORE);
			step++;
		case 2:
		case 4:
		case 6:
			if (twi.write(OLED_ADDRESS, pOLEDbuf))
				step++;
			break;
		case 3:
			oled.setCursor(2,5);
			itoa(pESC->ilocalLatDegrees, ch,10);
			oled.rJustify(ch,4);
			oled.writeChar(ch,DOUBLESIZE);
			oled.setRow(2);
			itoa(pESC->ilocalLatMinutes, ch,10);
			oled.rJustify(ch,3,'0');
			oled.writeChar(ch,DOUBLEWIDE);
			oled.setCursor(3,41);
			oled.writeChar("Lat");
			step++;
			break;
		case 5:
			oled.setCursor(2,58);
			itoa(pESC->ilocalLongDegrees, ch,10);
			oled.rJustify(ch,5);
			oled.writeChar(ch,DOUBLESIZE);
			oled.setRow(2);
			itoa(pESC->ilocalLongMinutes, ch,10);
			oled.rJustify(ch,3,'0');
			oled.writeChar(ch,DOUBLEWIDE);
			oled.setCursor(3,106);
			oled.writeChar("Lon");
			step++;
		case 7:
		default:
			break;
		}
	}
	
void AltAzPage(bool bNew) {		// Titlepage comments apply here, too.
	static uint8_t step;
	char ch[4];
	if (bNew)
		step=0;
	switch(step) {
		case 0:
			if (clearDisplay(bNew))
				step++;
			break;
		case 1:
			itoa(pESC->ilocalLatDegrees, ch,10);
			oled.rJustify(ch,4);
			oled.writeChar(" N", BOXLEFT | BOXBOTTOM);
			oled.writeChar(ch, BOXBOTTOM);
			oled.writeChar(0x80, BOXBOTTOM);
			itoa(pESC->ilocalLatMinutes, ch,10);
			oled.rJustify(ch,3,'0');
			oled.writeChar(ch, BOXBOTTOM);
			oled.writeChar(0x27, BOXBOTTOM | BOXRIGHT);
			step++;
			break;
		case 3:	
			oled.setCursor(0,65);
			itoa(pESC->ilocalLongDegrees, ch,10);
			oled.rJustify(ch,5);
			oled.writeChar(" W",BOXLEFT | BOXBOTTOM);
			oled.writeChar(ch, BOXBOTTOM);
			oled.writeChar(0x80, BOXBOTTOM);
			itoa(pESC->ilocalLongMinutes, ch,10);
			oled.rJustify(ch,3,'0');
			oled.writeChar(ch, BOXBOTTOM);
			oled.writeChar(0x27, BOXBOTTOM | BOXRIGHT);
			step++;
		case 2:
		case 4:
		case 6:
		case 8:
			if (twi.write(OLED_ADDRESS, pOLEDbuf))
				step++;
			break;
		case 5:
			oled.setCursor(2,5);
			itoa(pESC->iAltDegrees, ch,10);		// Convert integer to ASCII
			oled.rJustify(ch,4);					// Right justify in 4-byte field.
			oled.writeChar(ch,DOUBLESIZE);
			oled.setRow(2);
			itoa(pESC->iAltMinutes, ch,10);
			oled.rJustify(ch,3,'0');				// Right justify with leading zeros.
			oled.writeChar(ch,DOUBLEWIDE);
			oled.setCursor(3,40);
			oled.writeChar("Alt");
			step++;
			break;
		case 7:
			oled.setCursor(2,68);
			itoa(pESC->iAzDegrees, ch,10);
			oled.rJustify(ch,4);
			oled.writeChar(ch,DOUBLESIZE);
			oled.setRow(2);
			itoa(pESC->iAzMinutes, ch,10);
			oled.rJustify(ch,3,'0');
			oled.writeChar(ch,DOUBLEWIDE);
			oled.setCursor(3,103);
			oled.writeChar("Az");
			step++;
		default:
			break;
		}
	}


void RADecPage(bool bNew) {		// Titlepage comments apply here, too.
	static uint8_t step;
	char ch[5];
	if (bNew)
	step=0;
	switch(step) {
		case 0:
			if (clearDisplay(bNew))							// Returns false until entire display is cleared.
				step++;
			break;
		case 1:
			itoa(pESC->iAltDegrees, ch,10);					// Convert integer to ASCII.
			oled.rJustify(ch,5);							// Right justify in 5-byte field.
			oled.writeChar(ch, BOXLEFT | BOXBOTTOM);
			oled.writeChar(0x80, BOXBOTTOM);				// degree symbol added to font.
			itoa(pESC->iAltMinutes, ch,10);
			oled.rJustify(ch,3,'0');						// Right justify in 3-byte field with leading zeros.
			oled.writeChar(ch, BOXBOTTOM);
			oled.writeChar(0x27, BOXBOTTOM | BOXRIGHT);
			step++;
			break;
		case 2:
		case 4:
		case 6:
		case 8:
			if (twi.write(OLED_ADDRESS, pOLEDbuf))
				step++;
			break;
		case 3:
			oled.setCursor(0,65);
			itoa(pESC->iAzDegrees, ch,10);
			oled.rJustify(ch,5);
			oled.writeChar(ch, BOXLEFT | BOXBOTTOM);
			oled.writeChar(0x80,  BOXBOTTOM);
			itoa(pESC->iAzMinutes, ch,10);
			oled.rJustify(ch,3,'0');
			oled.writeChar(ch, BOXBOTTOM);
			oled.writeChar(0x27, BOXBOTTOM | BOXRIGHT);
			step++;
			break;
		case 5:
			oled.setCursor(2,5);
			itoa(pESC->iRAHours, ch,10);
			oled.rJustify(ch,4);
			oled.writeChar(ch,DOUBLESIZE);
			oled.setRow(2);
			itoa(pESC->iRAMinutes, ch,10);
			oled.rJustify(ch,3,'0');
			oled.writeChar(ch,DOUBLEWIDE);
			oled.setCursor(3,40);
			oled.writeChar("RA");
			step++;
			break;
		case 7:
			oled.setCursor(2,68);
			itoa(pESC->iDecDegrees, ch,10);
			oled.rJustify(ch,4);
			oled.writeChar(ch,DOUBLESIZE);
			oled.setRow(2);
			itoa(pESC->iDecMinutes, ch,10);
			oled.rJustify(ch,3,'0');
			oled.writeChar(ch,DOUBLEWIDE);
			oled.setCursor(3,103);
			oled.writeChar("Dec");
			step++;
		default:
			break;
		}
	}



	
	// The test for "busy" doesn't need CLI.
	// The ISR never sets it "free". The ISR doesn't set it "busy".
void updateDisplay(uint8_t iPage) {	// Called from Loop() when timer overflows (~60 times per second).
									// When Encoder is done, remove iPage as an argument.
	if(pOLEDbuf->isBusy(true))
		return;

	static uint8_t iPrev=255;
	bool bNew = (iPrev != iPage);		// Determine if this is a page change.
	iPrev = iPage;

	pOLEDbuf->reset(true);
	
	switch (iPage) {
		case 0:
		default:
			titlePage(bNew); // bNew, if true, will clear the display and draw it from scratch.
			break;
		case 1:
			setTimePage(bNew);
			break;
		case 2:
			AltAzPage(bNew);
			break;
		case 3:
			RADecPage(bNew);
			break;
		case 4:						// Never accessed with only 2 bits
			setLocationPage(bNew);
			break;
		}
	}


