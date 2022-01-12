/*
 * UartDrv.c
 *  Description: source file for Uart driver module
 *
 *  Created on: April 07, 2020
 *  Author: Ioan Nicoara
 */

/*#################################*/
/*         Include-Files           */
/*#################################*/
#include "UartDrv.h"

/*#################################*/
/*         Local defines           */
/*#################################*/
#define F_CPU 8000000UL		/* Define frequency here its 8MHz */
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

/*#################################*/
/*        Local data types         */
/*#################################*/


/*#################################*/
/*        Global ROM data          */
/*#################################*/


/*#################################*/
/*        Global RAM data          */
/*#################################*/

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


/*#################################*/
/*  Global function implementation */
/*#################################*/
void UartDrv_vInit(void)
{
  	UCSRB |= (1 << RXCIE) | (1 << RXEN) | (1 << TXEN);	/* Turn on transmission and reception */
  	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);/* Use 8-bit char size */
  	UBRRL = BAUD_PRESCALE;			/* Load lower 8-bits of the baud rate */
  	UBRRH = (BAUD_PRESCALE >> 8);		/* Load upper 8-bits*/
}

void UartDrv_vSendData(uint8_t ucByte)
{
	while (! (UCSRA & (1<<UDRE)));  /* Wait for empty transmit buffer */
	UDR = ucByte;
}


void UartDrv_vSendString(char * ucString, uint8_t ucLength)
{
	uint8_t ucIdx = 0u;
	
	for(ucIdx=0u; ucIdx<ucLength; ucIdx++)
	{
		UartDrv_vSendData(ucString[ucIdx]);	
	}	
	    
	/* Send CR character */
	UartDrv_vSendData(UART_CR_CHAR);
}


/*#################################*/
/*  Local function implementation  */
/*#################################*/

