/*
 * high level EEPROM read and write functions
 */

#ifndef EEPROM_H_INCLUDED
#define EEPROM_H_INCLUDED

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "delay.h"
#include "exprom.h"

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

void ee_write_char(unsigned char address, const unsigned char chunk) {
    eepromWrite(address, chunk);
}

void writeEEPROM(unsigned char _data[]) {
    for (unsigned volatile char i=0; i<8; i++) {
        ee_write_char(i, _data+i);
        
        //PORTAbits.RA0 = !PORTAbits.RA0;
        delay_ms(20);
        if (_data[i] == '\0') {break;};
    }
}

void ee_read_char(unsigned char address, unsigned char *_data){
    *_data = eepromRead(address);
}

void readEEPROM(unsigned char _data[]) {
    for (int i=0;; i++) {
        ee_read_char(i, _data+i);
        if (_data[i] == '\0' || _data[i] == 0xff) {break;}
    }
}

#endif