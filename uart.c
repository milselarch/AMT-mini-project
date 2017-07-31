#include <xc.h>

void uart_init(void)
{
	TRISCbits.TRISC6=0;	// set bit 6 as output
    TRISCbits.TRISC7=1;	// set bit 7 as input
	TXSTA1 = 0x20;
	BAUDCON1 = 0x00;
	SPBRG1 = 0x28;	//Baud = 9600
	TXSTA1bits.TXEN = 1;
	RCSTA1bits.CREN = 1;
	RCSTAbits.SPEN = 1;
}

void uart_send_char (unsigned char ucChar)
{
	while(PIR1bits.TXIF == 0);
	TXREG1 = ucChar;
}

void clr_screen(void)
{
	unsigned char clear[] = {0x1B , '[', '2', 'J'};
	int t=0;
  	while(t<4)
	{
		while(PIR1bits.TX1IF == 0);  // Previous Byte is transmitted. A New Byte can be written.
     	TXREG1 = clear[t];       //  Send Character
   		t++;
	}
}

void cursor_home(void)
{
	unsigned char clear[] = {0x1B , '[', '2', 'H'};
	int t=0;
  	while(t<4)
	{
		while(PIR1bits.TX1IF == 0);  // Previous Byte is transmitted. A New Byte can be written.
     	TXREG1 = clear[t];       //  Send Character
   		t++;
	}
}