#include <xc.h>

void SPI1init(void);
void SPI1out(char Code);

void SPI1init(void)
{
	SSP1CON1 = 0b00100010;	// Enable SPI, CKP=0, Master, Fosc/64
	SSP1STATbits.CKE = 1;	// CKE=1, (rising edge for shift register)
	TRISCbits.TRISC3 = 0;	// RC3 is SCK
	TRISCbits.TRISC5 = 0;	// RC5 is SDO
	SPI1out(0x00);
}

void SPI1out(char Code)
{
	SSP1BUF = Code;             // Transmit
	while(SSP1STATbits.BF==0);	// Wait for END (Receipt Buffer Full)
	Code = SSP1BUF;             // Just read to clear BF
}

