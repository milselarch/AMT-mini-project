#ifndef EEPROM_H_INCLUDED
#define EEPROM_H_INCLUDED

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "delay.h"

//http://innovatelogics.com/internal-eeprom-of-pic18f4550/


char cmpEEPROM(unsigned char input[], unsigned char check[]) {
    for (unsigned int i = 0;; i++) {
        unsigned char inputChar = input[i];
        unsigned char checkChar = check[i];
        if (inputChar == 0xff) {inputChar = 0; }
        if (checkChar == 0xff) {checkChar = 0; }
        
        if (inputChar == '\0' && checkChar == '\0') { break; }
        else if (inputChar == '\0' && checkChar != '\0') { return 1; }
        else if (inputChar != '\0' && checkChar == '\0') { return -1; }
        else if (inputChar > checkChar) { return 1; }
        else if (inputChar < checkChar) { return -1; }
        else {}
    }
    return 0;
}

char cmpEEPROML(unsigned char input[], unsigned char check[], const unsigned int length) {
    for (unsigned int i = 0; i<length ; i++) {
        unsigned char inputChar = input[i];
        unsigned char checkChar = check[i];
        if (inputChar == 0xff) {inputChar = 0; }
        if (checkChar == 0xff) {checkChar = 0; }
        
        if (inputChar == '\0' && checkChar == '\0') { break; }
        else if (inputChar == '\0' && checkChar != '\0') { return 1; }
        else if (inputChar != '\0' && checkChar == '\0') { return -1; }
        else if (inputChar > checkChar) { return 1; }
        else if (inputChar < checkChar) { return -1; }
        else {}
    }
    return 0;
}

void ee_write(unsigned int address, unsigned char _data[]) {
    for (volatile int index = 0; _data[index] != '\0'; index++) {
        eeprom_write(address++, _data[index]); 
    }
}

void ee_write_byte(unsigned char address, unsigned char *_data) {
    //sendPacket("ADDR",&address,4,1);
    int ctr = 0; 
    
    while (_data[ctr]!=0) {    
        eeprom_write(address++,string[ctr]);    
        ctr++; 
    }
}

void ee_write_char(unsigned char address, const unsigned char chunk){
    /*
    EEDATA = chunk;
    EEADR = address;
    // start write sequence as described in datasheet, page 91
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1; // enable writes to data EEPROM
    INTCONbits.GIE = 0;  // disable interrupts
    EECON2 = 0x55;
    EECON2 = 0x0AA;
    EECON1bits.WR = 1;   // start writing
    
    while(EECON1bits.WR){Nop();}
    
    if (EECON1bits.WRERR) {
        printf("ERROR: writing to EEPROM failed!n");
    }
    
    EECON1bits.WREN = 0;
    INTCONbits.GIE = 1;  // enable interrupts
    */
    
    eeprom_write(address, chunk); 
}

void writeEEPROM(unsigned char _data[]) {
    for (unsigned volatile char i=0; i<8; i++) {
        ee_write_byte(i, _data+i);
        Nop();Nop();Nop();Nop();Nop();
        
        //PORTAbits.RA0 = !PORTAbits.RA0;
        delay_ms(20);
        if (_data[i] == '\0') {break;};
    }
}

void ee_read_byte(unsigned char address, unsigned char *_data){
    /*
    EEADR = address;
    EECON1bits.CFGS = 0;
    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1;
    *_data = EEDATA;
    */
    
    *_data = eeprom_read(address);
}

void readEEPROM(unsigned char _data[]) {
    for (int i=0;; i++) {
        ee_read_byte(i, _data+i);
        if (_data[i] == '\0' || _data[i] == 0xff) {break;}
    }
}

#endif