// Timer bits

//#define TIFR0 _SFR_IO8(0x15) Section 14.9.7	// Timer 0 Interrupt Flag Register.
#define bTOV0 (1 << TOV0)			// Timer 0 Overflow flag. TCNT0 has reached 256.
#define bOCF0A (1 << OCF0A)			// Timer 0 Output Compare Flag (TCNT0 has matched the value in OCR0A).
#define bOCF0B (1 << OCF0B)			// Timer 0 Output Compare Flag (TCNT0 has matched the value in OCR0B).

//#define TIMSK0 _SFR_MEM8(0x6E) Section 14.9.6	// Timer 0 Interrupt mask. (Interrupts enabled as selected on these pins.)
#define bTOIE0 (1 << TOIE0)			// Timer 0 Overflow Interrupt Enable - when TOV0 is raised.
#define bOCIE0A (1 << OCIE0A)		// Timer 0 Compare match on chA Interrupt Enable - when OCF0A is raised.
#define bOCIE0B (1 << OCIE0B)		// Timer 0 Compare match on chB Interrupt Enable - when OCF0B is raised.

//#define TCCR0A _SFR_IO8(0x24)	Section 14.9.1 // Timer0 Compare Output Control Register. Part A.
#define bWGM00 (1 << WGM00)		// Timer0 Waveform Generator Mode (3 bits - see TCCR0B): 000=Normal, 010=CTC, 
#define bWGM01 (1 << WGM01)		// 
#define bCOM0B0 (1 << COM0B0)	// Compare Output Mode for T0, chB (low bit). 2 bits determine mode. (See COM0A1.)
#define bCOM0B1 (1 << COM0B1)	// Compare Output Mode for T0, chB (high bit). Operates same as for COM0A.
#define bCOM0A0 (1 << COM0A0)	// Compare Output Mode for T0, chA (low bit). 2 bits determine mode.
#define bCOM0A1 (1 << COM0A1)	// 00=normal port (OC0A disconnected), 01=Toggle OC0A on compare match,
								// 10=clear on match, 11=Set on match.

//#define TCCR0B _SFR_IO8(0x25) Section 14.9.2 // More Timer0 Compare Output Control Register. Part B.
#define bCS00 (1 << CS00)		// Counter Source. 3 bits. 000=no source, counter off. 001=system clk (16MHz).
#define bCS01 (1 << CS01)		// 010=clk/8, 011=clk/64, 100=clk/256, 101=clk/1024.
#define bCS02 (1 << CS02)		// 110=rising edge counter on pin0, 111=falling edge counter on pin0.
#define bWGM02 (1 << WGM02)		// Continuation of WGM0x from TCCR0A.
#define bFOC0B (1 << FOC0B)		// Force Output Compare chB. Make like a compare match happened.
#define bFOC0A (1 << FOC0A)		// Force Output Compare chA. Make like a compare match happened.

//#define TCNT0 _SFR_IO8(0x26) Section 14.9.3	// Timer/Counter0. Holds an 8-bit value, bits not enumerated.
//#define OCR0A _SFR_IO8(0x27) Section 14.9.4	// Output Compare Register ch A (Timer0). Holds chA value to compare to TCNT0.
//#define OCR0B _SFR_IO8(0x28) Section 14.9.5	// Output Compare Register ch B (Timer0). Holds chB value to compare to TCNT0.

/*------------ Bits for Timer 1 and Timer 2 will be added later. --------------------*/
