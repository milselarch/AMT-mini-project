#include <xc.h>

#define LCDEN LATHbits.LATH0
#define LCDRS LATHbits.LATH2

void delay_ms(int x);
void delay_us(int x);
void LCD8init(void);
void LCD8send(char c, char mode);

void LCD8init(void)
{
	TRISE = 0x00;
	TRISH = 0b11111000;
	LATHbits.LATH1 = 0;
	LCDEN = 0;
	LCDRS = 0;
	delay_ms(200);

	LCD8send(0x30,0);
	LCD8send(0x30,0);
	LCD8send(0x30,0);
	LCD8send(0x38,0);
	LCD8send(0x10,0);
	LCD8send(0x0F,0);
	LCD8send(0x06,0);
	LCD8send(0x01,0);
	delay_ms(100);
	
	LCD8send(0x8A,0);

	LCD8send('N',1);
	LCD8send('A',1);
	LCD8send('M',1);
	LCD8send('E',1);

	LCD8send(0xC0,0);
	LCD8send('#',1);
}

void LCD8send(char c, char mode)
{
	LCDRS = mode;
	delay_ms(1);

	LATE = c;
	LCDEN = 1;
	delay_us(1);
	LCDEN = 0;
	delay_us(50);  // 50 us in between commands or data
}


