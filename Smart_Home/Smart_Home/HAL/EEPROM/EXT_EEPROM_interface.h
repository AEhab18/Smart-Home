/*
 * EXT_EEPROM_interface.h
 *
 * Created: 7/7/2024 5:17:02 AM
 *  Author: ahmed
 */ 
#include "../../LIB/STD_TYPE.h"

#ifndef EXT_EEPROM_INTERFACE_H_
#define EXT_EEPROM_INTERFACE_H_

void TWI_Init(void);
void EEPROM_Write(u8 deviceAddress, u16 memoryAddress, u8 data);
u8 EEPROM_Read(u8 deviceAddress, u16 memoryAddress);
u8 VerifyAdminOrUser(u8 *enteredPassword);
u8 AddUser(u8 userId, u8 *password);
u8 DeleteUser(u8 userId);
void AdminCommands(void);
void WriteAdminPassword(void);
void DebugReadAdminPassword(void);

#endif /* EXT_EEPROM_INTERFACE_H_ */