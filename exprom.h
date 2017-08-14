// there are two EEPROm chips. One at port C6 and one at port D7

#define EEPROM_CS_TRIS	(TRISCbits.TRISC6)
///#define EEPROM_CS_IO	(LATCbits.LATC2)
#define EEPROM_CS_IO	(PORTCbits.RC6)

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

void eeWait() {
    while (EEPROM_SPISTATbits.BF == 0);
}

void eepromInit(void) {
	EEPROM_CS_IO = 1;
	EEPROM_CS_TRIS = 0;		// Drive SPI EEPROM chip select pin

	EEPROM_SCK_TRIS = 0;	// Set SCK pin as an output
	EEPROM_SDI_TRIS = 1;	// Make sure SDI pin is an input
	EEPROM_SDO_TRIS = 0;	// Set SDO pin as an output
    
    BAUDCON1 = 0x00;
    
    SPI1init();
    SPI1out(0x00); 
}

unsigned char eepromRead(
    unsigned char address
) {
    EEPROM_CS_IO = 0;
    
    SPI1out(READ);
	SPI1out(((address)>>8)&0xFF);
    SPI1out(address&0xFF);
            
    EEPROM_SSPBUF = 0x00;
    eeWait();
    
    unsigned char buffer = EEPROM_SSPBUF;
            
	EEPROM_CS_IO = 1;
    return buffer;
}

void eepromWrite(unsigned char address, unsigned char value) {
	// Save SPI state (clock speed)
	// SPICON1Save = EEPROM_SPICON1;
	// EEPROM_SPICON1 = PROPER_SPICON1;
    
	unsigned char sr = 0x00;
    
    while (1) {
        /*
        * we send write enable command to SPI
        * and get back status register value. if status
        * is acknowledge (0x02) it means the EEPROM is ready
        * to write.
        */
        
        //get acknowledgement for write enable operation
        // data flow is data to EEPROM
        EEPROM_CS_IO = 0;
        SPI1out(WREN);
        SPI1out(RDSR);
        
        // data flow is data from EEPROM
        EEPROM_CS_IO = 1;
        EEPROM_SSPBUF = 0x00;
        // wait for EEPROM status data to be loaded to SSPBUF
        eeWait();
        
        // READ status value sent by EEPROM
        sr = EEPROM_SSPBUF;
        // if status is ACK (acknowledge), break
        if (sr & 0x02 == 0x02) { break; } 
    }
	
    // tell EEPROM we're about to write data
    
    EEPROM_CS_IO = 0;
    SPI1out(WRITE);
	SPI1out(((address)>>8)&0xFF);
    SPI1out(address&0xFF);
    SPI1out(value);
    
    EEPROM_CS_IO = 1;
    delay_ms(30);
}