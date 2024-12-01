

#ifndef UART_INTERFACE_H_
#define UART_INTERFACE_H_
void UART_Init(u16 baudrate);
void UART_SendString(char *str);
void UART_ReceiveString(char *str, u8 maxLength);
u8 UART_ReceiveChar(void);
void UART_SendChar(u8 data);
void UART_SendNumber(u16 number);
#endif /* UART_INTERFACE_H_ */