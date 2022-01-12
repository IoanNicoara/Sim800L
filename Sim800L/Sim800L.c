/*
 * UartDrv.c
 *  Description: source file for Sim800L driver module
 *
 *  Created on: April 07, 2020
 *  Author: Ioan Nicoara
 */

/*#################################*/
/*         Include-Files           */
/*#################################*/
#include "Sim800L.h"
#include "UartDrv.h"

/*#################################*/
/*         Local defines           */
/*#################################*/


/*#################################*/
/*        Local data types         */
/*#################################*/


/*#################################*/
/*        Global ROM data          */
/*#################################*/

/* List: Send CMD pattern */
char SIM800L_CMD_SignalValueReq[SIM800L_CMD_SIGNAL_VAL_REQ_LGTH] = "AT+CSQ";
char SIM800L_CMD_SmsTextMode[SIM800L_CMD_TEXT_MODE_LGTH] = "AT+CMGF=1";
char SIM800L_CMD_ReqSmsToNumber[SIM800L_CMD_REQ_SMS_TO_NO_LGHT] = "AT+CMGS=\"+40770176491\"";

/* List: Received Response pattern */
char SIM800L_RCV_SignalValueReq[SIM800L_RCV_SIGNAL_VAL_REQ_LGTH] = "AT+CSQ\r\r\n+CSQ: ";
char SIM800L_RVC_SmsTextMode[SIM800L_RCV_TEXT_MODE_LGTH] = "AT+CMGF=1\r\r\nOK\r\n";
char SIM800L_RCV_ReqSmsToNumber[SIM800L_RCV_REQ_SMS_TO_NO_LGHT] = "> ";
/*#################################*/
/*        Global RAM data          */
/*#################################*/
char ucBuffer[50] = " ";
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
uint8_t Sim800L_ucCompareString(char * buffer, char * rcv, uint8_t ucLength);


/*#################################*/
/*  Global function implementation */
/*#################################*/
/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
void Sim800L_vInit(void)
{

}


/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
uint8_t Sim800L_ucReadSMSTextMode(char ucRcvChar)
{
  static uint8_t ucIdx= 0u;
  uint8_t bReturnValue = 0;
  
  if(ucIdx < SIM800L_RCV_TEXT_MODE_LGTH)
  {
    if(ucRcvChar == SIM800L_RVC_SmsTextMode[ucIdx])
    {
      ucBuffer[ucIdx] = ucRcvChar;
      ucIdx++;
    }
    else
    {
      ucIdx = 0u;
    }
  }
  else
  {
    bReturnValue = Sim800L_ucCompareString(ucBuffer, SIM800L_RVC_SmsTextMode, SIM800L_RCV_TEXT_MODE_LGTH);
	ucIdx = 0u;
  }
  
  return bReturnValue;
}

/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
uint8_t Sim800L_ucReadSignalStatus(char ucRcvChar)
{
  static uint8_t ucIdx= 0u;
  uint8_t bReturnValue = 0;
  
  /* Check if the received character is equal with the expected character */
  if(ucIdx < SIM800L_RCV_SIGNAL_VAL_REQ_LGTH)
  {
	  if(ucRcvChar == SIM800L_RCV_SignalValueReq[ucIdx])
	  {
		  /* Add data (char) to buffer */
		  ucBuffer[ucIdx] = ucRcvChar;
		  ucIdx++;
	  }
	  else
	  {
		  ucIdx = 0u;
	  }
  }
  else
  {
	  /* Compare received result with the expected result*/
	  bReturnValue = Sim800L_ucCompareString(ucBuffer, SIM800L_RCV_SignalValueReq, SIM800L_RCV_SIGNAL_VAL_REQ_LGTH);
	  /* If the received response is POSITIVE */
	  if(bReturnValue == 1u)
	  {
		  /*ucRcvChar - means the signal value, if signal value > 0u, return true */
		  if(ucRcvChar > 0u)
		  {
			  /*Signal RESPONSE > 1 */
			  bReturnValue = 1u;
		  }
		  else
		  {
			  bReturnValue = 0u;
		  }
		  ucIdx = 0u;
	  }
  }
  
  return bReturnValue;
}


/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
uint8_t Sim800L_ucReadSMSRequestStatus(char ucRcvChar)
{
  static uint8_t ucIdx= 0u;
  uint8_t bReturnValue = 0;
  
  if(ucIdx < SIM800L_RCV_REQ_SMS_TO_NO_LGHT)
  {
    if(ucRcvChar == SIM800L_RCV_ReqSmsToNumber[ucIdx])
    {
      ucBuffer[ucIdx] = ucRcvChar;
      ucIdx++;
    }
    else
    {
      ucIdx = 0u;
    }
  }
  else
  {
    bReturnValue = Sim800L_ucCompareString(ucBuffer, SIM800L_RCV_ReqSmsToNumber, SIM800L_RCV_REQ_SMS_TO_NO_LGHT);
	ucIdx = 0u;
  }
  
  return bReturnValue;
}

/*#################################*/
/*  Local function implementation  */
/*#################################*/

/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
uint8_t Sim800L_ucCompareString(char * buffer, char * rcv, uint8_t ucLength)
{
  uint8_t ucIdx = 0u;
  uint8_t ucReturnValue = 0x01u;
  
  for(ucIdx=0u; ucIdx<ucLength; ucIdx++)
  {
    
    if(buffer[ucIdx] == rcv[ucIdx])
    {
      ucReturnValue = ucReturnValue & 1u ;
    }
    else
    {
      ucReturnValue = ucReturnValue & 0u ;
    }
  }
  
  return ucReturnValue;
}
