/*
 * UartDrv.h
 *  Description: Header file for Sim800L control
 *
 *  Created on: April 07, 2020
 *  Author: Ioan Nicoara
 */

#ifndef SIM800L_H_
#define SIM800L_H_

/*#################################*/
/*       Include-Files             */
/*#################################*/
#include <stdio.h>
#include <avr/io.h>
#include <stddef.h>


/*#################################*/
/*       Global defines            */
/*#################################*/
//#define SIM800L_CMD_NETWORK_TYPE_LGTH	(13u)
#define SIM800L_CMD_SIGNAL_VAL_REQ_LGTH	(6u)
#define SIM800L_CMD_TEXT_MODE_LGTH		(9u)
#define SIM800L_CMD_REQ_SMS_TO_NO_LGHT	(22u)

#define SIM800L_RCV_SIGNAL_VAL_REQ_LGTH	(15u)
#define SIM800L_RCV_TEXT_MODE_LGTH		(16u)
#define SIM800L_RCV_REQ_SMS_TO_NO_LGHT	(2u)

/*#################################*/
/*      Global data types          */
/*#################################*/
typedef enum
{
	SMS_TEXT_MODE_STATUS = 0u,
	CHECK_SIGNAL_STATUS,
	SEND_SMS_STATUS,
	SEND_TEXT_STATUS,	
	UNKN_STATE,
}eMachineStateEnum;
/*#################################*/
/*       Global ROM data           */
/*#################################*/
//char SIM800L_CMD_NetworkType[SIM800L_CMD_NETWORK_TYPE_LGTH];
char SIM800L_CMD_SignalValueReq[SIM800L_CMD_SIGNAL_VAL_REQ_LGTH];
char SIM800L_CMD_SmsTextMode[SIM800L_CMD_TEXT_MODE_LGTH];
char SIM800L_CMD_ReqSmsToNumber[SIM800L_CMD_REQ_SMS_TO_NO_LGHT];

char SIM800L_RCV_SignalValueReq[SIM800L_RCV_SIGNAL_VAL_REQ_LGTH];
char SIM800L_RVC_SmsTextMode[SIM800L_RCV_TEXT_MODE_LGTH];
char SIM800L_RCV_ReqSmsToNumber[SIM800L_RCV_REQ_SMS_TO_NO_LGHT];




/*#################################*/
/*       Global RAM data           */
/*#################################*/

char ucBuffer[50];

/*#################################*/
/*   Global function declaration   */
/*#################################*/
/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
extern void Sim800L_vInit(void); 



/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
extern uint8_t Sim800L_ucReadSMSTextMode(char ucRcvChar);

/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
extern uint8_t Sim800L_ucReadSignalStatus(char ucRcvChar);

/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
extern uint8_t Sim800L_ucReadSMSRequestStatus(char ucRcvChar);

#endif /* Sim800L_H_ */

