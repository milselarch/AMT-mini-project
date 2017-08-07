#include <xc.h>
#include "amt.h"
#include <stdio.h>
#include "EEPROM.h"

#pragma config XINST = OFF
#pragma config FOSC = HS
#pragma config WDT = OFF
int counter=0;
int sec=0;


void interrupt HighIsr(void) // High priority interrupt
{
	if(PIR3bits.TMR4IF==1)
	{	
		counter++;
        if (counter>=1000)
        {
            sec++;
            counter=0;
        }
		PIR3bits.TMR4IF = 0;	// Clear Timer 4 Flag. 
	}
}

void interrupt low_priority LowIsr(void) //Low priority interrupt
{

}

void main(void) {
    LCD8init();
    delay_ms(1000);
    
    unsigned char *data;
    unsigned char address = 0;
    ee_read_byte(address, &data);
    LCD8send(*data, 1);
    
    while (1) {}
    //LCD8send(0x01, 0);
    //LCD8send(0x80, 0);
    //LCD8send('o', 1);
}

