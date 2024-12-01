/*
 * EEPROM_REG.h
 *
 * Created: 11/26/2024 11:17:02 PM
 *  Author: aehab
 */ 


#ifndef EEPROM_REG_H_
#define EEPROM_REG_H_

// TWI Bit Rate Register (TWBR)
#define TWBR    (*((volatile u8 *) 0x20)) // Sets the bit rate for SCL

// TWI Control Register (TWCR)
#define TWCR    (*((volatile u8 *) 0x56)) // Enables TWI and controls operations

// TWI Status Register (TWSR)
#define TWSR    (*((volatile u8 *) 0x21)) // Contains TWI status and prescaler bits

// TWI Data Register (TWDR)
#define TWDR    (*((volatile u8 *) 0x23)) // Holds the data to be transmitted or received

// TWI Address Register (TWAR)
#define TWAR    (*((volatile u8 *) 0x22)) // Sets the slave address and enables general call
#define TWINT   7  // TWI Interrupt Flag
#define TWEA    6  // TWI Enable Acknowledge Bit
#define TWSTA   5  // TWI Start Condition Bit
#define TWSTO   4  // TWI Stop Condition Bit
#define TWWC    3  // TWI Write Collision Flag
#define TWEN    2  // TWI Enable Bit
#define TWIE    0  // TWI Interrupt Enable Bit
//**********************ADMIN and USER Part****************************//
#define EEPROM_ADDR 0x50     // I2C Address of the EEPROM
#define MAX_USERS   6        // Maximum number of users
#define PASSWORD_LENGTH 4    // Fixed password length
#define ADMIN_PASSWORD_ADDR  0x00  // Admin password starts at this EEPROM address
#define USER_DB_START_ADDR   0x10  // User database starts here
#define USER_ENTRY_SIZE      5     // Each user entry takes 5 bytes (1 byte for ID, 4 bytes for password)




#endif /* EEPROM_REG_H_ */