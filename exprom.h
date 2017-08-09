// This is a guard condition so that contents of this file are not included
// more than once.  

/*
  EEPROM_SPICON1 = SSPCON1
 */

//#define PROPER_SPICON1	(0x21)
#define PROPER_SPICON1	EEPROM_SPICON1		/* SSPEN bit is set, SPI in master mode, FOSC/16, IDLE state is low level */

///#define EEPROM_CS_TRIS	(TRISCbits.TRISC2)
#define EEPROM_CS_TRIS	(TRISDbits.TRISD7)
///#define EEPROM_CS_IO	(LATCbits.LATC2)
#define EEPROM_CS_IO	(PORTDbits.RD7)

#define EEPROM_SCK_TRIS	(TRISCbits.TRISC3)
#define EEPROM_SDI_TRIS	(TRISCbits.TRISC4)
#define EEPROM_SDO_TRIS	(TRISCbits.TRISC5)

#define EEPROM_SPI_IF	(PIR1bits.SSPIF)
#define EEPROM_SSPBUF	(SSP1BUF)
#define EEPROM_SPICON1	(SSP1CON1)
#define EEPROM_SPICON1bits	(SSP1CON1bits)
#define EEPROM_SPICON2	(SSP1CON2)
#define EEPROM_SPISTAT	(SSP1STAT)
#define EEPROM_SPISTATbits	(SSP1STATbits)

//http://hades.mech.northwestern.edu/index.php/Interfacing_PIC_with_SPI_memory
#define READ	0x03	// Read data from memory array beginning at selected address
#define WRITE	0x02	// Write data to memory array beginning at selected address
#define WRDI	0x04	// Reset the write enable latch (disable write operations)
#define WREN	0x06	// Set the write enable latch (enable write operations)
#define RDSR	0x05	// Read Status register
#define WRSR	0x01	// Write Status register

const unsigned long EEDELAY = 30;

void wait() {
    //delay_ms(EEDELAY);
    //while (!EEPROM_SPI_IF);
    while(EEPROM_SPISTATbits.BF == 0);
}

void XEEInit(void) {
	EEPROM_CS_IO = 1;
	EEPROM_CS_TRIS = 0;		// Drive SPI EEPROM chip select pin

	EEPROM_SCK_TRIS = 0;	// Set SCK pin as an output
	EEPROM_SDI_TRIS = 1;	// Make sure SDI pin is an input
	EEPROM_SDO_TRIS = 0;	// Set SDO pin as an output
    
    BAUDCON1 = 0x00;
    
    /*
    SSP1STATbits.CKE = 1;	// CKE=1, (rising edge for shift register)
	TRISCbits.TRISC3 = 0;	// RC3 is SCK
	TRISCbits.TRISC5 = 0;	// RC5 is SDO
    */
    
    SPI1init();
    SPI1out(0x00); 
}


unsigned char XEERead(
    unsigned char address
) {
    EEPROM_CS_IO = 0;
    
    SPI1out(READ);
	SPI1out(((address)>>8)&0xFF);
    SPI1out(address&0xFF);
            
    EEPROM_SSPBUF = 0x00;
    wait();
    
    unsigned char buffer = EEPROM_SSPBUF;
            
	EEPROM_CS_IO = 1;
    return buffer;
}

static void eeWriteChar(unsigned char EEPROMAddress, unsigned char value) {
    unsigned long SPICON1Save;
    
	// Save SPI state (clock speed)
	SPICON1Save = EEPROM_SPICON1;
	EEPROM_SPICON1 = PROPER_SPICON1;
    
	unsigned char sr = 0x00;
    
    while(1) {
        EEPROM_CS_IO = 0;
        SPI1out(WREN);
        SPI1out(RDSR);
        EEPROM_CS_IO = 1;
        EEPROM_SSPBUF = 0x00;
        wait();
        
        sr = EEPROM_SSPBUF;
        if (sr & 0x02 == 0x02) { break; } 
    }
	
    wait();
    SPI1out(WRITE);
	SPI1out(((EEPROMAddress)>>8)&0xFF);
    SPI1out(EEPROMAddress&0xFF);
    // Send the byte to write
    EEPROM_SSPBUF = value;
    ///while(!EEPROM_SPI_IF);
    wait();
    
    int Dummy = EEPROM_SSPBUF;
    
    EEPROM_SPI_IF = 0;
	// Begin the write
	EEPROM_CS_IO = 1;
	// Restore SPI State
	EEPROM_SPICON1 = SPICON1Save;

	// Wait for write to complete
	while(XEEIsBusy());
    //delay_ms(EEDELAY*10);
}

int XEEIsBusy() {
	unsigned char result;
	unsigned long SPICON1Save;
    
	// Save SPI state (clock speed)
	SPICON1Save = EEPROM_SPICON1;
	EEPROM_SPICON1 = PROPER_SPICON1;

	EEPROM_CS_IO = 0;
	// Send RDSR - Read Status Register opcode
	EEPROM_SSPBUF = RDSR;
	wait();
    
    unsigned long val;
    val = EEPROM_SSPBUF;
	EEPROM_SPI_IF = 0;
    
	// Get register contents
	EEPROM_SSPBUF = 0;
	wait();
            
	val = EEPROM_SSPBUF;
	EEPROM_SPI_IF = 0;
	EEPROM_CS_IO = 1;

	// Restore SPI State
	EEPROM_SPICON1 = SPICON1Save;

	return val & 1;
}