//******************************************************************************
//  Jose I Quinones
//  Texas Instruments Inc.
//  May 2011
//  Built with IAR Embedded Workbench Version: 5.1
//******************************************************************************
#include "Config.h"

void Task7(void)
{
StatusLED += 1;
if (!StatusLED)
    {
    P3OUT ^= StatusLEDPin;
    }
if (CommTimeOut > 200)
    {
    CommTimeOut = 201;
    SerialPointer = 0;
    }
}
