/*
 * EXT_EEPROM_prog.c
 *
 * Created: 7/7/2024 5:17:25 AM
 *  Author: ahmed
 */ 

#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPE.h"
#include "EEPROM_REG.h"
#include "EXT_EEPROM_interface.h"
#include "string.h"

#define F_CPU 16000000UL
#define F_SCL 100000UL
 #define Prescaler 1 
u8 adminPassword[4] = {'1', '2', '3', '4'};  // Default admin password
u8 userPasswords[MAX_USERS][4];             // Array to store user passwords in memory
u8 userCount = 0;                           // Count of registered users




/********** Function To Write/Read Data on EEPROM **********/
void DebugAdminPassword(void) {
	char storedPass[4];

	UART_SendString((u8 *)"Reading Admin Password: ");
	for (u8 i = 0; i < 4; i++) {
		storedPass[i] = EEPROM_Read(EEPROM_ADDR, ADMIN_PASSWORD_ADDR + i);

		// Debug: Print the raw byte value
		UART_SendString((u8 *)"Byte ");
		UART_SendChar(i + '0');
		UART_SendString((u8 *)": ");
		UART_SendNumber(storedPass[i]); // Implement UART_SendNumber if necessary
		UART_SendChar(' ');
	}
	UART_SendChar('\n');
}



void WriteAdminPassword(void) {
	u8 defaultAdminPass[PASSWORD_LENGTH] = {'1', '2', '3', '4'}; // Admin password

	// Write each byte of the password to EEPROM
	for (u8 i = 0; i < PASSWORD_LENGTH; i++) {
		EEPROM_Write(EEPROM_ADDR, ADMIN_PASSWORD_ADDR + i, defaultAdminPass[i]);
		
		// Debug: Print the data being written
		UART_SendString((u8 *)"Writing: ");
		UART_SendChar(defaultAdminPass[i]);
		UART_SendChar('\n');
	}
}



void TWI_Init(void) {
	TWBR = ((F_CPU / F_SCL) - 16) / (2 * Prescaler); // Set bit rate
	TWSR = 0x00; // Prescaler set to 1
	TWCR = (1 << TWEN); // Enable TWI
}


void EEPROM_Write(u8 deviceAddress, u16 memoryAddress, u8 data) {
	// Start Condition
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT))); // Wait for TWINT flag

	// Send Device Address
	TWDR = deviceAddress & 0xFE; // Write mode (LSB = 0)
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT))); // Wait for ACK

	// Send High Memory Address Byte
	TWDR = (memoryAddress >> 8);
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT))); // Wait for ACK

	// Send Low Memory Address Byte
	TWDR = (memoryAddress & 0xFF);
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT))); // Wait for ACK

	// Send Data
	TWDR = data;
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT))); // Wait for ACK

	// Stop Condition
	TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
}


u8 EEPROM_Read(u8 deviceAddress, u16 memoryAddress) {
	u8 data;

	// Start Condition
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT))); // Wait for TWINT flag

	// Send Device Address (Write Mode)
	TWDR = deviceAddress & 0xFE;
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT))); // Wait for ACK

	// Send High Memory Address Byte
	TWDR = (memoryAddress >> 8);
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT))); // Wait for ACK

	// Send Low Memory Address Byte
	TWDR = (memoryAddress & 0xFF);
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT))); // Wait for ACK

	// Repeated Start Condition
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT))); // Wait for TWINT flag

	// Send Device Address (Read Mode)
	TWDR = deviceAddress | 0x01;
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT))); // Wait for ACK

	// Read Data
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT))); // Wait for TWINT flag
	data = TWDR;

	// Stop Condition
	TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);

	return data;
}



//******************Admin Functions***************//


u8 AddUser(u8 userId, u8 *password) {
	if (userCount >= MAX_USERS) {
		return 0; // Database is full
	}
	u16 addr = USER_DB_START_ADDR + userCount * USER_ENTRY_SIZE;
	EEPROM_Write(EEPROM_ADDR, addr, userId); // Write User ID
	for (u8 i = 0; i < 4; i++) {
		EEPROM_Write(EEPROM_ADDR, addr + 1 + i, password[i]); // Write password
	}
	userCount++;
	return 1; // Success
}


u8 DeleteUser(u8 userId) {
	for (u8 i = 0; i < userCount; i++) {
		u16 addr = USER_DB_START_ADDR + i * USER_ENTRY_SIZE;
		if (EEPROM_Read(EEPROM_ADDR, addr) == userId) {
			// Shift remaining users up
			for (u8 j = i; j < userCount - 1; j++) {
				for (u8 k = 0; k < USER_ENTRY_SIZE; k++) {
					EEPROM_Write(EEPROM_ADDR, USER_DB_START_ADDR + j * USER_ENTRY_SIZE + k,
					EEPROM_Read(EEPROM_ADDR, USER_DB_START_ADDR + (j + 1) * USER_ENTRY_SIZE + k));
				}
			}
			userCount--;
			return 1; // Success
		}
	}
	return 0; // User not found
}


u8 VerifyAdminOrUser(u8 *enteredPassword) {
	u8 storedPass[4];

	// Check Admin Password
	for (u8 i = 0; i < 4; i++) {
		storedPass[i] = EEPROM_Read(EEPROM_ADDR, ADMIN_PASSWORD_ADDR + i);
	}
	if (memcmp(storedPass, enteredPassword, 4) == 0) {
		return 1; // Admin login
	}

	// Check User Passwords
	for (u8 i = 0; i < MAX_USERS; i++) {
		u16 addr = USER_DB_START_ADDR + i * USER_ENTRY_SIZE + 1; // Password starts after user ID
		for (u8 j = 0; j < 4; j++) {
			storedPass[j] = EEPROM_Read(EEPROM_ADDR, addr + j);
		}
		if (memcmp(storedPass, enteredPassword, 4) == 0) {
			return 2; // User login
		}
	}

	return 0; // Invalid password
}

void AdminCommands(void) {
	u8 command;
	u8 userId;
	u8 password[4];

	while (1) {
		UART_SendString((u8 *)"1. Add User\n2. Delete User\n3. Exit Admin Mode\n");
		command = UART_ReceiveChar();

		if (command == '1') {
			UART_SendString((u8 *)"Enter User ID: ");
			userId = UART_ReceiveChar();

			UART_SendString((u8 *)"Enter Password (4 characters): ");
			for (u8 i = 0; i < 4; i++) {
				password[i] = UART_ReceiveChar();
			}

			if (AddUser(userId, password)) {
				UART_SendString((u8 *)"User Added Successfully\n");
				} else {
				UART_SendString((u8 *)"User Database Full\n");
			}
			} else if (command == '2') {
			UART_SendString((u8 *)"Enter User ID to Delete: ");
			userId = UART_ReceiveChar();

			if (DeleteUser(userId)) {
				UART_SendString((u8 *)"User Deleted Successfully\n");
				} else {
				UART_SendString((u8 *)"User Not Found\n");
			}
			} else if (command == '3') {
			UART_SendString((u8 *)"Exiting Admin Mode...\n");
			break; // Exit admin commands loop
		}
	}
}


