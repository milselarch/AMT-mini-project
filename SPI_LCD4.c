#include <xc.h>
#define SPI_LCDEN LATCbits.LATC2

void delay_ms(int x);
void delay_us(int x);
void SPI1init(void);
void SPI1out(char Code);
void SPI_LCD4init(void);
void SPI_LCD4send(char c, char mode);

void SPI_LCD4send(char c, char mode)
{	
	SPI1out((c & 0xF0)| mode);
	SPI_LCDEN = 1;	
	//Delay10TCYx(1);
	delay_us(1);
	SPI_LCDEN = 0;
	//Delay10TCYx(2);	
	delay_us(2);
	SPI1out(((c<<4) & 0xF0) | mode);
	SPI_LCDEN = 1;	
	//Delay10TCYx(1);	
	delay_us(1);
	SPI_LCDEN = 0;
	//Delay10TCYx(5);
	//Delay1KTCYx(5);	// 5000 us in between commands or data	
	delay_ms(5);
}

void SPI_LCD4init(void)
{
	TRISCbits.TRISC2 = 0;	//LCD EN
	SPI_LCDEN = 0;
	
	//Delay1KTCYx(250);
	//Delay1KTCYx(250); 
	//Delay1KTCYx(250); 
	//Delay1KTCYx(250);
	//Delay1KTCYx(250);
	//Delay1KTCYx(250); 
	//Delay1KTCYx(250); 
	//Delay1KTCYx(250);
	delay_ms(500);

	SPI_LCD4send(0x33,0);
	SPI_LCD4send(0x33,0);
	SPI_LCD4send(0x32,0);
	SPI_LCD4send(0x28,0);
	SPI_LCD4send(0x10,0);
	SPI_LCD4send(0x0F,0);
	SPI_LCD4send(0x06,0);
	SPI_LCD4send(0x01,0);
	//Delay1KTCYx(250);
	delay_ms(10);
	SPI_LCD4send(0x8A,0);		

	SPI_LCD4send('O',1);
	SPI_LCD4send('k',1);
	SPI_LCD4send('a',1);
	SPI_LCD4send('y',1);
	SPI_LCD4send('.',1);

	SPI_LCD4send(0xC0,0);		
	SPI_LCD4send('#',1);	
}