
#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPE.h"
#include "UART_reg.h" 

#define F_CPU 16000000UL



void UART_Init(u16 baudrate) {
	u16 ubrr = F_CPU / 16 / baudrate - 1;  // Calculate UBRR value
	UBRRH = (u8)(ubrr >> 8);              // Set the higher byte of UBRR
	UBRRL = (u8)ubrr;                     // Set the lower byte of UBRR
	UCSRB = (1 << RXEN) | (1 << TXEN);         // Enable RX and TX
	UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1); // 8-bit data, 1 stop bit, no parity
}



void UART_SendString(char *str) {
	while (*str) {                             // Loop until null terminator is reached
		while (!(UCSRA & (1 << UDRE)));        // Wait for the UART Data Register to be empty
		UDR = *str;                            // Send the current character
		str++;                                 // Move to the next character
	}
}
void UART_ReceiveString(char *str, u8 maxLength) {
	u8 i = 0;                             // Index to track string position
	char receivedChar;

	while (i < maxLength - 1) {                // Leave space for null terminator
		while (!(UCSRA & (1 << RXC)));         // Wait for a character to be received
		receivedChar = UDR;                    // Read the received character

		if (receivedChar == '\n' || receivedChar == '\r') {
			break;                             // Stop reading on newline or carriage return
		}

		str[i] = receivedChar;                 // Store the received character
		i++;                                   // Increment the index
	}

	str[i] = '\0';                             // Null-terminate the string
}
void UART_SendNumber(u16 number) {
	char buffer[10]; // Buffer to hold the ASCII representation of the number
	u8 index = 0;

	// Convert the number to a string (ASCII)
	if (number == 0) {
		UART_SendChar('0'); // Send '0' directly if the number is 0
		return;
	}

	while (number > 0) {
		buffer[index++] = (number % 10) + '0'; // Get the last digit and convert to ASCII
		number /= 10; // Remove the last digit
	}

	// Send the digits in reverse order (since we processed them from the least significant digit)
	while (index > 0) {
		UART_SendChar(buffer[--index]);
	}
}


void UART_SendChar(u8 data) {
	while (!(UCSRA & (1 << UDRE))); // Wait for the Data Register to be empty
	UDR = data;                    // Write the data to be transmitted
}
u8 UART_ReceiveChar(void) {
	while (!(UCSRA & (1 << RXC))); // Wait for a character to be received
	return UDR;                    // Return the received character
}
