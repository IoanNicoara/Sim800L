/*
 * UartDrv.h
 *  Description: Header file for UART control
 *
 *  Created on: April 07, 2020
 *  Author: Ioan Nicoara
 */

#ifndef UARTDRV_H_
#define UARTDRV_H_

/*#################################*/
/*       Include-Files             */
/*#################################*/
#include <stdio.h>
#include <avr/io.h>
#include <stddef.h>

/*#################################*/
/*       Global defines            */
/*#################################*/
#define UART_CR_CHAR		(0x0D) /*CR character*/
#define UART_CTRL_Z_CHAR	(0x1A) /*Ctrl+Z character*/

/*#################################*/
/*      Global data types          */
/*#################################*/


/*#################################*/
/*       Global ROM data           */
/*#################################*/

/*#################################*/
/*       Global RAM data           */
/*#################################*/

/*#################################*/
/*   Global function declaration   */
/*#################################*/
/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
extern void UartDrv_vInit(void); 

/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
extern void UartDrv_vSendData(uint8_t ucByte);

/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
extern void UartDrv_vSendString(char * ucString, uint8_t ucLength);

#endif /* UARTDRV_H_ */

