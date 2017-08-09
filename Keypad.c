//Keypad routines
#include <xc.h>

unsigned char process_key(unsigned char code);
unsigned char scan_code[16]={0xEB,0x77,0x7B,0x7D,
							 0xB7,0xBB,0xBD,0xD7,
							 0xDB,0xDD,0x7E,0xBE,
							 0xDE,0xEE,0xE7,0xED};

void KeyPadinit() {
    TRISD = 0xF0;
}

unsigned char Read_KeyPad()
{
	unsigned char code,Col,value;
	int i;
	value = 0xFF;
	Col = 0b11111110;
	for (i=0;i<4;i++)
	{
		LATD = Col;
		code = PORTD;//read PORTD
		code = code | 0b00001111;//mask to obtain RD7,6,5,4
		code = code & Col;//put back original RD3,2,1,0
		if (code!=Col) 
		{
			value = process_key(code);
			break;
		}
		Col = (Col << 1) | 0x01;
	}
    
	return(value);
}

unsigned char process_key(unsigned char code)
{
	unsigned char i;
	for (i=0;i<16;i++)
	{
		if (code==scan_code[i]) return i;
	}
	return(0xFF);
}
