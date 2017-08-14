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

void interrupt low_priority LowIsr(void) {

}

void lcdWriteLine(char * data) {
    for (int i = 0; data[i] != 0; i++) {
        LCD8send(data[i], 1);
    }
}

void writeNum(unsigned long number) {
    char numStr[20];
    sprintf(numStr, "%i", number);
    lcdWriteLine(numStr);
}

void writeLCD(unsigned char address, unsigned int valMode) {
    LCD8send(0x01, 0);
    unsigned char *data;

    if (valMode == 0) {
        ee_read_char(address, data);
        lcdWriteLine("#");
        writeNum(address);
    
        lcdWriteLine(" V");
        writeNum(*data);
    
    } else if (valMode == 1) {
        unsigned long num = ee_read_num(address);
        lcdWriteLine("#");
        writeNum(address);
    
        lcdWriteLine(" N");
        writeNum(num);
    };

}

void main() {
    LCD8init();
    KeyPadinit();
    eepromInit();
    
    delay_ms(1000);
    
    TRISJ = 0x00;
    PORTJ = 0b0;
    
    unsigned char address = 250;
    unsigned long writeValue = 250;
    unsigned int mode = READ_MODE;
    unsigned char prevKey = 0xFF;
    unsigned int valMode;
    
    while (1) {
        int keyNo = Read_KeyPad();
        unsigned char key = keyValues[keyNo];
        valMode = 0;
        
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
            
            } else if (key == '#') {
                if (mode == WRITE_MODE) {
                    ee_write_num(address, writeValue);
                    delay_ms(100);
                }
                
                valMode = 1;
                
            } else if (key == '*' && mode == WRITE_MODE) {
                ee_write_char(address, writeValue++);
                delay_ms(100);
            
            }
            
            //if (keyNo != 0xFF) {
            writeLCD(address, valMode);
            //}
            
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
};