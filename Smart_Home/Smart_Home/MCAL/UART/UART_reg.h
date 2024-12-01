/*
 * UART_reg.h
 *
 * Created: 9/20/2024 4:36:22 PM
 *  Author: AMIT
 */ 
#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPE.h"

#ifndef UART_REG_H_
#define UART_REG_H_


// UART Registers
#define UDR     (*((volatile u8 *) 0x2C))  // UART Data Register
#define UCSRA   (*((volatile u8 *) 0x2B))  // UART Control and Status Register A
#define UCSRB   (*((volatile u8 *) 0x2A))  // UART Control and Status Register B
#define UCSRC   (*((volatile u8 *) 0x40))  // UART Control and Status Register C
#define UBRRH   (*((volatile u8 *) 0x40))  // UART Baud Rate High Register
#define UBRRL   (*((volatile u8 *) 0x29))  // UART Baud Rate Low Register

// UCSRA Bits
#define RXC     7  // USART Receive Complete
#define TXC     6  // USART Transmit Complete
#define UDRE    5  // USART Data Register Empty
#define FE      4  // Frame Error
#define DOR     3  // Data OverRun
#define PE      2  // Parity Error
#define U2X     1  // Double the USART Transmission Speed
#define MPCM    0  // Multi-processor Communication Mode

// UCSRB Bits
#define RXCIE   7  // RX Complete Interrupt Enable
#define TXCIE   6  // TX Complete Interrupt Enable
#define UDRIE   5  // USART Data Register Empty Interrupt Enable
#define RXEN    4  // Receiver Enable
#define TXEN    3  // Transmitter Enable
#define UCSZ2   2  // Character Size (bit 2)
#define RXB8    1  // Receive Data Bit 8
#define TXB8    0  // Transmit Data Bit 8

// UCSRC Bits
#define URSEL   7  // Register Select (UCSRC or UBRRH)
#define UMSEL   6  // USART Mode Select (0 = Asynchronous, 1 = Synchronous)
#define UPM1    5  // Parity Mode Bit 1
#define UPM0    4  // Parity Mode Bit 0
#define USBS    3  // Stop Bit Select
#define UCSZ1   2  // Character Size Bit 1
#define UCSZ0   1  // Character Size Bit 0
#define UCPOL   0  // Clock Polarity (Synchronous mode only)

#endif /* UART_REG_H_ */