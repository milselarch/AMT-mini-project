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

void ee_write_char(unsigned char address, const unsigned char chunk) {
    eepromWrite(address, chunk);
}

void ee_write_string(unsigned int address, unsigned char _data[]) {
    for (volatile int index = 0; _data[index] != '\0'; index++) {
        ee_write_char(address++, _data[index]); 
    }
}

void ee_read_char(unsigned char address, unsigned char *_data){
    *_data = eepromRead(address);
}

void ee_read_string(unsigned char _data[]) {
    for (int i=0;; i++) {
        ee_read_char(i, _data+i);
        if (_data[i] == '\0' || _data[i] == 0xff) {break;}
    }
}

void ee_clear(unsigned long length) {
    for (int i=0; i<length; i++) { 
        ee_write_char(i, 0xFF);
    }
}

void ee_write_num(unsigned char addr, volatile unsigned long number) {
    volatile unsigned char i;
    
    for (i = 0; number != 0; i++) {
        ee_write_char(addr + i, number % 10);
        number /= 10;
    }
    
    ee_write_char(addr + i, 255);
}


unsigned long ee_read_num(unsigned char addr) {
    unsigned long i = 0;
    unsigned long number = 0;
    unsigned char byte = 0;
    
    do {
        number = number * 10 + byte;
        byte = eepromRead(addr + i);
        i++;
        
    } while (byte != 0xFF);
    
    return number;
}


#endif