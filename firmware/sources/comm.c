//******************************************************************************
//  Jose I Quinones
//  Texas Instruments Inc.
//  May 2011
//  Built with IAR Embedded Workbench Version: 5.1
//******************************************************************************
#include "config.h"

char SerialPointer;
char SerialBuffer[SERIAL_BUFFER_LENGTH];
char SerialOutBuffer[3];
char SerialOutPointer;
bool MessageComplete;
int  CommTimeOut;

static void
__attribute__((__interrupt__(USCIAB0TX_VECTOR)))
USCIAB0_Transmit (void)
//#pragma vector=USCIAB0TX_VECTOR
//__interrupt void USCIAB0_Transmit(void)
{
  SerialOutPointer += 1;
  UCA0TXBUF = SerialOutBuffer[SerialOutPointer];
  if ( SerialOutPointer == SERIAL_OUT_LENGTH - 1 )
    {
      IE2 &= ~UCA0TXIE;
    }
}

static void
__attribute__((__interrupt__(USCIAB0RX_VECTOR)))
USCIAB0_Receive (void)
//#pragma vector=USCIAB0RX_VECTOR
//__interrupt void USCIAB0_Receive(void)
{
  SerialBuffer[SerialPointer] = UCA0RXBUF;
  SerialPointer += 1;
  CommTimeOut = 0;
  if (SerialPointer == SERIAL_BUFFER_LENGTH)
    {
      SerialPointer = 0;
      MessageComplete = true;
    }
}
