//******************************************************************************
//  Jose I Quinones
//  Texas Instruments Inc.
//  May 2011
//  Built with IAR Embedded Workbench Version: 5.1
//******************************************************************************
#include "config.h"

//int AccelIndex;
//unsigned int AccelArray[2][750];

void Task1(void)
{

  switch (AccelerateState)
  {
  case (NOACC):
    break;
  case (ACCEL):
    if (tmpAccelTimeBase > 0)
        {
        tmpAccelTimeBase -= 1;                  //Wait until delay expires
        }
    else
        {
        if (((unsigned int) StepPeriod - (unsigned int) AccelRate) < DesiredStepperSpeed)
        {
          StepPeriod = DesiredStepperSpeed;
          AccelerateState = NOACC;
          
          //Debug
          //AccelIndex = 0;
          
        }
        else
        {
          StepPeriod -= AccelRate;
          
          //Debug
          //AccelArray[0][AccelIndex] = StepCounter;
          //AccelIndex += 1;
        }
        tmpAccelTimeBase = AccelTimeBase;
        }
    break;

  case (DECEL):
    if (tmpAccelTimeBase > 0)
        {
        tmpAccelTimeBase -= 1;                  //Wait until delay expires
        }
    else
        {
        if (((unsigned int) StepPeriod + (unsigned int) AccelRate) > DesiredStepperSpeed)
        {
          StepPeriod = DesiredStepperSpeed;
          AccelerateState = NOACC;
        }
        else
        {
          StepPeriod += AccelRate;

          //Debug
          //AccelArray[1][AccelIndex] = StepCounter - (AccelSteps + AtSpeedSteps);
          //AccelIndex += 1;
        }
        tmpAccelTimeBase = AccelTimeBase;
        }
    break;

  case (STOP):
    if (tmpAccelTimeBase > 0)
        {
        tmpAccelTimeBase -= 1;                  //Wait until delay expires
        }
    else
        {
        if (((unsigned int) StepPeriod + (unsigned int) AccelRate) < DesiredStepperSpeed)
        {
          StepPeriod += AccelRate;
        }
        else
        {
          TBCCTL6 &= 0xFF0F;                                          //Disable CC and Interrupt
          AccelerateState = NOACC;
          if (WithDisable)
            {
              PHASEA_PORT = 0;
              PHASEB_PORT = 0;
            }
        }
        tmpAccelTimeBase = AccelTimeBase;
        }
    break;

//*************************    S CURVE ACELERATION       ***********************    
    
//******************************************************************************
// S Curve Acceleration Increase
// Changes the Acceleration Rate by increasing it until the  SCurveAccelSpeed is met.
//******************************************************************************    
      case (S_CURVE_ACCELI):
    if (tmpAccelTimeBase > 0)
        {
        tmpAccelTimeBase -= 1;                  //Wait until delay expires
        }
    else
        {
        if ((unsigned int) StepPeriod < (unsigned int)SCurveAccelSpeed)
          {
          AccelerateState = S_CURVE_ACCSS;
          }
        else
          {
          AccelRate += AccelRateChange;
          }
        StepPeriod -= AccelRate;
        tmpAccelTimeBase = AccelTimeBase;
        }
    break;
//******************************************************************************
// S Curve Acceleration Steady State
// Acceleration Rate is unmodified until the SCurveDecelSpeed is met.
//******************************************************************************    
      case (S_CURVE_ACCSS):
    if (tmpAccelTimeBase > 0)
        {
        tmpAccelTimeBase -= 1;                  //Wait until delay expires
        }
    else
        {
        if ((unsigned int) StepPeriod < (unsigned int)SCurveDecelSpeed)
          {
          AccelerateState = S_CURVE_ACCELD;
          AccelRate -= AccelRateChange;
          }
        StepPeriod -= AccelRate;
        tmpAccelTimeBase = AccelTimeBase;
        }
    break;
//******************************************************************************
// S Curve Acceleration Decrease
// Changes the Acceleration Rate by decreasing it until the DesiredSpeed is met.
//******************************************************************************      
      case (S_CURVE_ACCELD):
    if (tmpAccelTimeBase > 0)
        {
        tmpAccelTimeBase -= 1;                  //Wait until delay expires
        }
    else
        {
        if ((unsigned int) StepPeriod < (unsigned int)DesiredStepperSpeed)
          {
          StepPeriod = DesiredStepperSpeed;
          AccelerateState = NOACC;
          }
        else
          {
          StepPeriod -= AccelRate;
          AccelRate -=AccelRateChange;
          }
        tmpAccelTimeBase = AccelTimeBase;
        }
    break;

//*************************    S CURVE DECCELERATION     ***********************    
    
//******************************************************************************
// S Curve Deceleration Increase
// Changes the Acceleration Rate by increasing it until the SCurveDecelSpeed is met.
//******************************************************************************    
      case (S_CURVE_DECELI):
    if (tmpAccelTimeBase > 0)
        {
        tmpAccelTimeBase -= 1;                  //Wait until delay expires
        }
    else
        {
        if ((unsigned int) StepPeriod > (unsigned int)SCurveDecelSpeed)
          {
          AccelerateState = S_CURVE_DECSS;
          }
        else
          {
          AccelRate += AccelRateChange;
          }
        StepPeriod += AccelRate;
        tmpAccelTimeBase = AccelTimeBase;
        }
    break;
//******************************************************************************
// S Curve Acceleration Steady State
// Acceleration Rate is unmodified until the SCurveAccelSpeed is met.
//******************************************************************************    
      case (S_CURVE_DECSS):
    if (tmpAccelTimeBase > 0)
        {
        tmpAccelTimeBase -= 1;                  //Wait until delay expires
        }
    else
        {
        if ((unsigned int) StepPeriod > (unsigned int)SCurveAccelSpeed)
          {
          AccelerateState = S_CURVE_DECELD;
          AccelRate -= AccelRateChange;
          }
        StepPeriod += AccelRate;
        tmpAccelTimeBase = AccelTimeBase;
        }
    break;
//******************************************************************************
// S Curve Acceleration Decrease
// Changes the Acceleration Rate by decreasing it until the DesiredSpeed is met.
//******************************************************************************      
      case (S_CURVE_DECELD):
    if (tmpAccelTimeBase > 0)
        {
        tmpAccelTimeBase -= 1;                  //Wait until delay expires
        }
    else
        {
        if ((unsigned int) StepPeriod > (unsigned int)DesiredStepperSpeed)
        {
          StepPeriod = DesiredStepperSpeed;
          AccelerateState = NOACC;
          TBCCTL6 &= 0xFF0F;                                          //Disable CC and Interrupt
        }
        else
        {
          StepPeriod += AccelRate;
          AccelRate -=AccelRateChange;
        }
        tmpAccelTimeBase = AccelTimeBase;
        }
    break;    
    
    
    
  }
}
