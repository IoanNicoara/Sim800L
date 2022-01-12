/*
 * os.c
 *  Description: Operating system c source file
 *
 *  Created on: March 30, 2020
 *  Author: Ioan Nicoara
 */

/*#################################*/
/*         Include-Files           */
/*#################################*/
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <avr/interrupt.h>

#include "os.h"
/*Include project header files*/
#include "UartDrv.h"
#include "Sim800L.h"


/*#################################*/
/*         Local defines           */
/*#################################*/

/* Defines the time period between 2 messages. The value is defined in s (1 = 1s)*/
#define MESSAGE_FREQUENCY_IN_S	(60u)

#define MESSAGE_FREQUENCY_IN_MS	(MESSAGE_FREQUENCY_IN_S * 1000u)
/*#################################*/
/*        Local data types         */
/*#################################*/


/*#################################*/
/*        Global ROM data          */
/*#################################*/


/*#################################*/
/*        Global RAM data          */
/*#################################*/
volatile uint8_t  OS_u8Cnt10ms;
volatile uint8_t  OS_u8Cnt50ms;
volatile uint16_t OS_u16Cnt100ms;
volatile uint16_t OS_u16Cnt500ms;

/* Initial state of the SMS State Machine */
uint8_t state = SMS_TEXT_MODE_STATUS;

/* State Machine Status */
uint8_t ucTextModeStatus = 0u;
uint8_t ucSignalStatus = 0u;
uint8_t ucSmsRequestStatus = 0u;

uint8_t ucAllowSendSms = 0u;
uint32_t ulTimeCounter = 0u;

/* BUFFER - message text field (no of characters)*/
#define MESSAGE_LENGTH	(5u)
char ucMessageField[MESSAGE_LENGTH] = "Salut";

/*#################################*/
/*        Local ROM data           */
/*#################################*/


/*#################################*/
/*        Local RAM data           */
/*#################################*/


/*#################################*/
/*    Local function declaration   */
/*#################################*/

/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
uint8_t Sim800L_ucStringCompare(char * buffer, char * rcv, uint8_t ucLength);

/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
void Sim800L_vSendSMS(void);

/*#################################*/
/*  Global function implementation */
/*#################################*/
void OS_vTaskInitialization(void)
{
  UartDrv_vInit();
  Sim800L_vInit();
  /* Initialization of the time counter (the first message will be send after MESSAGE_FREQUENCY_IN_MS from uC startup) */
  ulTimeCounter = MESSAGE_FREQUENCY_IN_MS;
}

void OS_vTimerInit(void)
{
   /*Set 8 bit Timer0 on Clear Timer on Compare Match (CTC) Mode with clkIO/256 clock source
    * TCCR0 register: WGM01=1 WGM00=0       -> CTC Mode, 8-bit with OCR1C top
    *                 CS02=1 CS01=0 CS00=0  -> clkIO/256 from prescaler
    * Max value OCR1C = 0x9C
    * Timer set to generate interrupt at 5ms*/
   TCCR0 = (1<<WGM01) | (1<<CS02);
   OCR0 = 0x9C;
}

void OS_vCyclicTask5ms(void)
{
	
}

void OS_vCyclicTask10ms(void)
{

}

void OS_vCyclicTask50ms(void)
{
	
}

void OS_vCyclicTask100ms(void)
{
	static uint8_t ucCallPeriodCnt = 0u;
	
	/* Send an SMS once at a time period equal to MESSAGE_FREQUENCY_IN_S */
	if (ucAllowSendSms == 1u)
	{
		/* Call Sim800L_vSendSMS() function once at 200ms*/
		if(ucCallPeriodCnt < 2)
		{
			ucCallPeriodCnt++;
		}
		else
		{
			Sim800L_vSendSMS();
			ucCallPeriodCnt = 0u;
		}	
	}
}

void OS_vCyclicTask500ms(void)
{
	if (ulTimeCounter>501)
	{
		/* Decrease the time counter with 0.5s (500ms)*/
		ulTimeCounter = ulTimeCounter - 500u;
		/* Disable SMS sending */
		ucAllowSendSms  = 0;
	}
	else
	{
		/* Enable SMS sending */
		ucAllowSendSms = 1;
	}
}

void OS_vBackgroundTask(void)
{
	/* Put here your additional code */
}

void OS_vStartFreeRunTimer(void)
{
   /*Set Timer/Counter0 Compare Match interrupt enable*/
   TIMSK |= (1<<OCIE0);
   sei();
}

/*Interrupt is generated every 5ms*/
ISR(TIMER0_COMP_vect)
{
   OS_vCyclicTask5ms();

   /*Increase counters used for other OS cyclic functions*/
   OS_u8Cnt10ms++;
   OS_u8Cnt50ms++;
   OS_u16Cnt100ms++;
   OS_u16Cnt500ms++;
}

ISR(USART_RXC_vect)
{
	char ucRcvChar;
	
	/* Store the received value from the UDR register (RxVal) */
    ucRcvChar = UDR;  
	
	/* Send SMS state machine (validate the response of the Sim800L) */
	if(state == SMS_TEXT_MODE_STATUS)
	{
		/* Check if the SMSMode = 1 */
		ucTextModeStatus = Sim800L_ucReadSMSTextMode(ucRcvChar);
		if (ucTextModeStatus == 1)
		{
			/* Go to the next state */
			state = CHECK_SIGNAL_STATUS;
		}	
	}
	else if(state == CHECK_SIGNAL_STATUS)
	{
		/* Check if the network signal value is OK */
		ucSignalStatus = Sim800L_ucReadSignalStatus(ucRcvChar);
		if (ucSignalStatus == 1)
		{
			/* Go to the next state */
			state = SEND_SMS_STATUS;
		}		
	}
	else if(state == SEND_SMS_STATUS)
	{
		/* Check if the Sim800L allow the message sending to the inserted phone number */
		ucSmsRequestStatus = Sim800L_ucReadSMSRequestStatus(ucRcvChar);
		if (ucSmsRequestStatus == 1)
		{
			state = SEND_TEXT_STATUS;
			/* Send the text of the message */
			UartDrv_vSendString(ucMessageField, MESSAGE_LENGTH);
			/* Send the "Send message" cmd */
			UartDrv_vSendData(UART_CTRL_Z_CHAR);
			
			/* Reinitialization of the time counter */
			ulTimeCounter = MESSAGE_FREQUENCY_IN_MS;
		}
	}
	else
	{
		/* Do nothing */
	}
}


/*#################################*/
/*  Local function implementation  */
/*#################################*/

/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
void Sim800L_vSendSMS(void)
{
	/* Send SMS state machine (send the command for the Sim800L) */
	switch (state)
	{
		case SMS_TEXT_MODE_STATUS:
		{
			/* Set the Sim800L in the text message mode (SMSMode) */
			UartDrv_vSendString(SIM800L_CMD_SmsTextMode, SIM800L_CMD_TEXT_MODE_LGTH);
			break;
		}
		case CHECK_SIGNAL_STATUS:
		{
			/* Check the network signal value */
			UartDrv_vSendString(SIM800L_CMD_SignalValueReq, SIM800L_CMD_SIGNAL_VAL_REQ_LGTH);
			break;
		}
		case SEND_SMS_STATUS:
		{
			/* Set the phone number (sms destination number) */
			UartDrv_vSendString(SIM800L_CMD_ReqSmsToNumber, SIM800L_CMD_REQ_SMS_TO_NO_LGHT);
			break;
		}
		default:
		{
			state = SMS_TEXT_MODE_STATUS;
			break;
		}
	}
}