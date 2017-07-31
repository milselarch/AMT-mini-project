#include <xc.h>

void T2intr(void);
void T4intr(void);

void T2intr(void)
{
	INTCONbits.GIEH = 0;
	RCONbits.IPEN = 1; // Enable Priority Features \par
	IPR1bits.TMR2IP = 1;// Timer 2 is High Priority \par
	PIR1bits.TMR2IF = 0;// Clear Timer 2 Flag. \par
	PIE1bits.TMR2IE = 1;// Enable Timer 2 Interrupt. \par
	INTCONbits.GIEH = 1;// Enable Global Interrupt, High. \par
}

void T4intr(void)
{
	INTCONbits.GIEH = 0;
	RCONbits.IPEN = 1;	// Enable Priority Features 
	IPR3bits.TMR4IP = 1;	// Timer 4 is High Priority 
  	PIR3bits.TMR4IF = 0;	// Clear Timer 4 Flag. 
	PIE3bits.TMR4IE = 1;	// Enable Timer 4 Interrupt. 
  	INTCONbits.GIEH = 1;	// Enable Global Interrupt, High. 
}
