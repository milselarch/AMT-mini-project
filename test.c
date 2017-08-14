#include <xc.h>
#include "amt.h"
#include <stdio.h>
#include "EEPROM.h"

#pragma config XINST = OFF
#pragma config FOSC = HS
#pragma config WDT = OFF

const int READ_MODE = 1;
const int WRITE_MODE = 0;
int counter = 0;
int sec = 0;

extern unsigned char scan_code[16];
unsigned char keyValues[16] = "0123456789ABCD*#";

void interrupt HighIsr(void) // High priority interrupt
{
	if (PIR3bits.TMR4IF==1)
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

void lcdWriteLine(char * data) {
    for (int i = 0; data[i] != 0; i++) {
        LCD8send(data[i], 1);
    }
}

void writeNum(unsigned char number) {
    char numStr[10];
    sprintf(numStr, "%i", number);
    lcdWriteLine(numStr);
}

void writeLCD(unsigned char address) {
    unsigned char *data;
    LCD8send(0x01, 0);
    ee_read_byte(address, data);
    
    lcdWriteLine("#");
    writeNum(address);

    lcdWriteLine(" V");
    writeNum(*data);
}

void main(void) {
    LCD8init();
    KeyPadinit();
    XEEInit();
    
    delay_ms(1000);
    
    TRISJ = 0x00;
    PORTJ = 0b0;
    
    unsigned char address = 0;
    unsigned char writeValue = 0;
    unsigned int mode = READ_MODE;
    unsigned char prevKey = 0xFF;
    
    while (1) {
        int keyNo = Read_KeyPad();
        unsigned char key = keyValues[keyNo];
        
        if (key != prevKey) {
            PORTJ++;
            
            if (keyNo >= 0 && 10 > keyNo) {
                if (mode == READ_MODE) {
                    address = address * 10 + keyNo;
                } else {
                    writeValue = writeValue * 10 + keyNo;
                }
                
            } else if (key == 'A') {
                if (mode == READ_MODE) { address = 0; }
                if (mode == WRITE_MODE) { writeValue = 0; }
                
            } else if (key == 'B') {
                if (mode == READ_MODE) { address++; }
                if (mode == WRITE_MODE) { writeValue++; }
                
            } else if (key == 'C') {
                if (mode == READ_MODE) { address--; }
                if (mode == WRITE_MODE) { writeValue--; }
                
            } else if (key == 'D') {
                if (mode == READ_MODE) { mode = WRITE_MODE; }
                else if (mode == WRITE_MODE) { mode = READ_MODE; }
            
            } else if (key == '*' && mode == WRITE_MODE) {
                eeWriteChar(address, writeValue);
            } 
            
            if (keyNo != 0xFF) {
                writeLCD(address);
            }
            
            LCD8send(0xC0, 0);
            LCD8send("WR"[mode], 1);
            lcdWriteLine(" key=");
            
            LCD8send(key, 1);
            lcdWriteLine(" val=");
            writeNum(writeValue);
                    
            prevKey = key;
        }
       
        delay_ms(100);
    }
    
    //LCD8send(0x01, 0);
    //LCD8send(0x80, 0);
    //LCD8send('o', 1);
}

