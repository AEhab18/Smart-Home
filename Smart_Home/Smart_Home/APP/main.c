/*



*/

//-------------------------------------------------------------
//-------------------------------------------------------------
#include "../LIB/STD_TYPE.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/DIO/DIO_interface.h"
#include "../MCAL/UART/UART_interface.h"
#include "../MCAL/ADC/ADC_interface.h"
#include "../MCAL/TIMER0/TIMER0_interface.h"
#include "../MCAL/GI/GI_interface.h"

#include "../HAL/LCD/LCD_interface.h"

#include "../FreeRTOS/FreeRTOS.h"
#include "../FreeRTOS/FreeRTOSConfig.h"
#include "../FreeRTOS/task.h"
#include "../FreeRTOS/semphr.h"
#include "../FreeRTOS//queue.h"
#include "../HAL/EEPROM/EXT_EEPROM_interface.h"

//-------------------------------------------------------------

#define F_CPU 16000000UL
#include <util/delay.h>

//-------------------------------------------------------------

SemaphoreHandle_t LCD;

QueueHandle_t Queue1;
QueueHandle_t Queue2;

//-------------------------------------------------------------

u8 PassCorrect = 0 ;
u8 F = 0;
//3175

u8 passwordIndex = 0;

//-------------------------------------------------------------
//-------------------------------------------------------------
void LM35(void){
	f32 adc = 0 ;
	f32 temp = 0 ;
	
	while (1)
	{
		if (xSemaphoreTake( LCD, 100 ) == HIGH && PassCorrect)
		{
			adc = ADC_u16ReadChannel(ADC_0) ;
			temp = adc*500/1024 ;
			xQueueSend( Queue1, &temp, 100 );
			
			LCD_voidGotoxy(0, 1);
			LCD_voidSendString("TEMP ");
			
			LCD_voidGotoxy(5, 1);
			LCD_voidSendNumber(temp);
		}
		xSemaphoreGive( LCD );
		vTaskDelay(100);
	}
}
//-------------------------------------------------------------
void Terminal(void) {
	u8 enteredPassword[4];
	u8 passwordIndex = 0;
	u8 loginRole = 0;
   

	while (1) {
		// Check if semaphore is available
		if (xSemaphoreTake(LCD, 100) == pdTRUE) {
			// Receive password from UART
			UART_SendString((u8 *)"\nEnter Password:");
			while (passwordIndex < 4) {
				enteredPassword[passwordIndex++] = UART_ReceiveChar();
				
			}
			passwordIndex = 0;

			// Check the login role
			//loginRole = 2;
			loginRole = VerifyAdminOrUser(enteredPassword);
			DebugAdminPassword();

			if (loginRole == 1) {
				// Admin mode
				LCD_voidGotoxy(0, 0);
				LCD_voidSendString("ADMIN MODE");
				AdminCommands(); // Call admin commands function
				
				} else if (loginRole == 2) {
				// User mode
				LCD_voidGotoxy(0, 0);
				LCD_voidSendString("USER MODE");
				PassCorrect = 1;
				xQueueSend(Queue2, &PassCorrect, 100);
				vTaskSuspend(NULL);
				
				} else {
				// Invalid password
				LCD_voidGotoxy(0, 0);
				LCD_voidSendNumber(loginRole);
				//LCD_voidSendString("Wrong Password");
			}

			// Release semaphore
			xSemaphoreGive(LCD);
		}
	}
}


//-------------------------------------------------------------
void Screen(void)
{
	u8 result;
	
	while (1)
	{
		if (xSemaphoreTake(LCD, 100) == pdTRUE)
		{
			
			
			if (xQueueReceive(Queue2, &result, 100) == pdTRUE)
			{
				if (result == 1)
				{
					PassCorrect = 1;
					DIO_voidSetPinVal(DIO_PORTD, DIO_PIN4, HIGH);

					LCD_voidGotoxy(0, 0);
					LCD_voidSendString("WELCOME  ");
				}
				else
				{
					LCD_voidGotoxy(0, 0);
					LCD_voidSendString("WRONG PASSWORD!");
					LCD_voidGotoxy(0, 1);
					LCD_voidSendString("               ");

					DIO_voidSetPinVal(DIO_PORTD, DIO_PIN5, HIGH);
				}
			}
			xSemaphoreGive(LCD);
			vTaskSuspend(NULL);
		}
		
	}
}

//-------------------------------------------------------------
void FAN(void)
{
	f32 T = 0;
	
	while (1)
	{
		if (xSemaphoreTake( LCD, 100 ) == HIGH && PassCorrect)
		{
			xQueueReceive( Queue1, &T, 100 );
			
			if (T>35)
			{
				TIMER0_voidFastPWM(100) ;
				
				LCD_voidGotoxy(7, 1);
				LCD_voidSendString(" FAN ");
				
				LCD_voidGotoxy(12, 1);
				LCD_voidSendNumber(100);
				LCD_voidSendData('%');
			}
			else if(T>30)
			{
				TIMER0_voidFastPWM(70)  ;
				
				LCD_voidGotoxy(7, 1);
				LCD_voidSendString(" FAN ");
				
				LCD_voidGotoxy(12, 1);
				LCD_voidSendNumber(70);
				LCD_voidSendString("% ");
			}
			else if(T>25)
			{
				TIMER0_voidFastPWM(30)  ;
				
				LCD_voidGotoxy(7, 1);
				LCD_voidSendString(" FAN ");
				
				LCD_voidGotoxy(12, 1);
				LCD_voidSendNumber(30);
				LCD_voidSendString("% ");
			}
			else
			{
				TIMER0_voidFastPWM(0)   ;
				
				LCD_voidGotoxy(7, 1);
				LCD_voidSendString(" FAN ");
				
				LCD_voidGotoxy(12, 1);
				LCD_voidSendNumber(0);
				LCD_voidSendString("%  ");
			}
		}
		xSemaphoreGive( LCD );
		vTaskDelay(100);
	}
	
}
//-------------------------------------------------------------
void LDR(void){
	u16 adc = 0 ;
	u32 V = 0 ;
	
	while (1)
	{
		if (xSemaphoreTake( LCD, 100 ) == HIGH && PassCorrect)
		{
			adc = ADC_u16ReadChannel(ADC_1) ;
			V = (adc * 5) / 1023;
			
			if (V > 3)
			{
				LCD_voidGotoxy(8, 0);
				LCD_voidSendString("NO LAMPs");
				
				DIO_voidSetPinVal(DIO_PORTC, DIO_PIN6, LOW);
				DIO_voidSetPinVal(DIO_PORTC, DIO_PIN7, LOW);
			}else if(V > 1.5)
			{
				LCD_voidGotoxy(8, 0);
				LCD_voidSendString("ONE LAMP");
				
				DIO_voidSetPinVal(DIO_PORTC, DIO_PIN6, HIGH);
				DIO_voidSetPinVal(DIO_PORTC, DIO_PIN7, LOW );
			}else
			{
				LCD_voidGotoxy(8, 0);
				LCD_voidSendString("TWO LAMP");
				
				DIO_voidSetPinVal(DIO_PORTC, DIO_PIN6, HIGH);
				DIO_voidSetPinVal(DIO_PORTC, DIO_PIN7, HIGH);
			}
		}
		xSemaphoreGive( LCD );
		vTaskDelay(100);
	}
}

//-------------------------------------------------------------
//-------------------------------------------------------------
int main(void)
{
	//Directions 
	DIO_voidSetPortDir(DIO_PORTA, 0xF0); // 4->7 output 0b11110000
	DIO_voidSetPortDir(DIO_PORTB, 0xFF); // all out
	DIO_voidSetPortDir(DIO_PORTC, 0xFF); // all out
	DIO_voidSetPortDir(DIO_PORTD, 0xF0); //4->7 output 0b11110000
	
	//Create Tasks
	xTaskCreate(LM35	, "LM35"    , 100, NULL, 5, NULL);
	xTaskCreate(Screen	, "Screen"  , 100, NULL, 6, NULL);
	xTaskCreate(Terminal, "Terminal", 100, NULL, 7, NULL);
	xTaskCreate(FAN		, "FAN"		, 100, NULL, 4, NULL);
	xTaskCreate(LDR		, "LDR"		, 100, NULL, 3, NULL);
	
	//Initialization of Peripherals
	GI_voidEnable();
	UART_Init(9600);
	LCD_voidInit();
	ADC_voidInit();
	TWI_Init();
	WriteAdminPassword();
	
	// Create Semaphore
	LCD = xSemaphoreCreateBinary();
	xSemaphoreGive(LCD);
	
	// Create Queues
	Queue1 = xQueueCreate( 1, sizeof( f32 ) ); 
	Queue2 = xQueueCreate( 1, sizeof( u8  ) );
	
	// Start Code
	//LCD_voidGotoxy(0, 0);
	//LCD_voidSendString("");
	
	LCD_voidGotoxy(0, 1);
	LCD_voidSendString("ENTER PASSWORD!");
	
	// Start Scheduler
	vTaskStartScheduler();
}
//-------------------------------------------------------------
//-------------------------------------------------------------
