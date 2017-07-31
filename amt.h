#ifndef _AMT_HEADER_FILE
#define _AMT_HEADER_FILE

void delay_ms(int x);
void delay_us(int x);
//SPI routines
void SPI1init(void);
void SPI1out(char Code);
//parallel port LCD routines
void LCD8init(void);
//serial port LCD routines
void LCD8send(char c, char mode);
void SPI_LCD4init(void);
void SPI_LCD4send(char c, char mode);
//keypad routines
void KeyPadinit();
unsigned char Read_KeyPad();
//Timer interrupt routines
void T2intr(void);
void T4intr(void);
//uart routines
void uart_init(void);
void uart_send_char (unsigned char ucChar);
void clr_screen(void);
void cursor_home(void);


#endif