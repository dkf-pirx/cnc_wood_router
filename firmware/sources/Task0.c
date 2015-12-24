//******************************************************************************
//  Jose I Quinones
//  Texas Instruments Inc.
//  May 2011
//  Built with IAR Embedded Workbench Version: 5.1
//******************************************************************************
#include "config.h"

bool HighResolutionMode, UARTMode;
bool MS_Direction;
bool MoveSteps;
bool WithDisable;

unsigned long MS_Index;

char StatusLED;

unsigned int MS_Divider, OldMS_Divider, NewMS_Divider;
unsigned int SCurveAccelSpeed, SCurveDecelSpeed;
unsigned int StepPeriod;
unsigned int GPIODirection;
unsigned int AccelRate, DesiredStepperSpeed, StartStepperSpeed, AccelRateChange;
unsigned int tmpAccelTimeBase, AccelTimeBase;
unsigned int AccelerateState;
unsigned int StepsToMove;
unsigned int StepCounter, AccelSteps, DecelSteps, AtSpeedSteps, MoveStepsState;

void Task0(void)
{
  int *MyPointer;
  int Address;
  int Data;
  char BData;
  char * MyPtr;

  
if (MessageComplete)
    {
      SerialOutBuffer[0] = FIRMWARE_REVISION;
      SerialOutBuffer[1] = 0;
      SerialOutBuffer[2] = 0;
      switch(OPCODE)
      {
// Write GPIO Data [ OPCODE = 3 ] [ PHASE A GPIO ] [ PHASE B GPIO ] [ Not Used ] [ Not Used ]
// PHASE X GPIO DATA = PHASExI0/PHASExI1/PHASExI2/PHASExI3/PHASExI4/PHASEx/PHASExDECAY/PHASExENABLE        
      case (WRITE_GPIO):
            
            P2OUT = SerialBuffer[1];        //PHASE A Control Signals
            P5OUT = SerialBuffer[2];        //PHASE B Control Signals

            switch (SerialBuffer[3])
            {
            case 0:                         //Slow Decay Mode: Output LO
              P2DIR |= DECAYA;
              P2OUT &= ~DECAYA;
              break;
            case 1:                         //Fast Decay Mode: Output HI
              P2DIR |= DECAYA;
              P2OUT |= DECAYA;
              break;
            case 2:                         //Mixed Decay Mode: Input HIZ
              P2DIR &= ~DECAYA;
              break;
            }

            switch (SerialBuffer[4])
            {
            case 0:                         //Slow Decay Mode: Output LO
              P5DIR |= DECAYB;
              P5OUT &= ~DECAYB;
              break;
            case 1:                         //Fast Decay Mode: Output HI
              P5DIR |= DECAYB;
              P5OUT |= DECAYB;
              break;
            case 2:                         //Mixed Decay Mode: Input HIZ
              P5DIR &= ~DECAYB;
              break;
            }


            
            SerialOutBuffer[1] = ~SerialBuffer[1];
            break;

// DAC 0 Config and Write [ OPCODE = 1 ] [ Config HB ] [ Config LB ] [ Data HB ] [ Data LB ]
      case (DAC0_UPDATE):
            DAC12_0CTL = 0;
            DAC12_0CTL = (SerialBuffer[1] * 256) + SerialBuffer[2];
            DAC12_0DAT = (SerialBuffer[3] * 256) + SerialBuffer[4];
            break;
// DAC 1 Config and Write [ OPCODE = 2 ] [ Config HB ] [ Config LB ] [ Data HB ] [ Data LB ]
      case (DAC1_UPDATE):
            DAC12_1CTL = 0;
            DAC12_1CTL = (SerialBuffer[1] * 256) + SerialBuffer[2];
            DAC12_1DAT = (SerialBuffer[3] * 256) + SerialBuffer[4];
            break;            
            
// Pulse Timer Output [ OPCODE = 0x0F ] [Timer Used] [ Pulse Length Hi ] [ Pulse Length Lo ] [ Not Used ]
      case (PULSE_TMR):
        TBCTL &= ~TB_CNTL_08;                //Configure Timer to run up to FFFF (16 bit FRC)
        switch (SerialBuffer[1])
        {
        case 0:
            TBCCTL0 &= 0xFF1F;                                          // Clear OUTMODx bits; 3 MSB on lower byte; Timer configured to output mode
            TBCCTL0 |= TB_OUT_HIGH;                                     //Set the output (pulse start)
            TBCCTL0 |= TB_OUTMOD_RESET;                                 //Configure the timer to reset
            TBCCR0 = TBR + ((SerialBuffer[2] * 256) + SerialBuffer[3]); //Configure the pulse reset time (pulse end)
          break;
        case 1:
            TBCCTL1 &= 0xFF1F;                                          // Clear OUTMODx bits; 3 MSB on lower byte; Timer configured to output mode
            TBCCTL1 |= TB_OUT_HIGH;                                     //Set the output (pulse start)
            TBCCTL1 |= TB_OUTMOD_RESET;                                 //Configure the timer to reset
            TBCCR1 = TBR + ((SerialBuffer[2] * 256) + SerialBuffer[3]); //Configure the pulse reset time (pulse end)
          break;
        case 2:
            TBCCTL2 &= 0xFF1F;                                          // Clear OUTMODx bits; 3 MSB on lower byte; Timer configured to output mode
            TBCCTL2 |= TB_OUT_HIGH;                                     //Set the output (pulse start)
            TBCCTL2 |= TB_OUTMOD_RESET;                                 //Configure the timer to reset
            TBCCR2 = TBR + ((SerialBuffer[2] * 256) + SerialBuffer[3]); //Configure the pulse reset time (pulse end)
          break;
        case 3:
            TBCCTL3 &= 0xFF1F;                                          // Clear OUTMODx bits; 3 MSB on lower byte; Timer configured to output mode
            TBCCTL3 |= TB_OUT_HIGH;                                     //Set the output (pulse start)
            TBCCTL3 |= TB_OUTMOD_RESET;                                 //Configure the timer to reset
            TBCCR3 = TBR + ((SerialBuffer[2] * 256) + SerialBuffer[3]); //Configure the pulse reset time (pulse end)
          break;
        }
        break;

// CONFIG STEPPER [ OPCODE = 0x11 ] [ Start Step Period Hi ] [ Start Step Period Lo ] [ Desired Step Period HI] [ Desired Step Period Lo ]

      case (CONFIG_STEPPER):
            StartStepperSpeed = (SerialBuffer[1] * 256) + SerialBuffer[2];   //Configure the StepPeriod Rate
            DesiredStepperSpeed = (SerialBuffer[3] * 256) + SerialBuffer[4];   //Configure the StepPeriod Rate
            
            int StepperSpeedDelta;
            
            StepperSpeedDelta = StartStepperSpeed - DesiredStepperSpeed;
            StepperSpeedDelta = StepperSpeedDelta >> 2;                         //Divide by four to compute S Curve Quadrants
            
            SCurveAccelSpeed = StartStepperSpeed - StepperSpeedDelta;
            SCurveDecelSpeed = DesiredStepperSpeed + StepperSpeedDelta;
            break;        
        
        
// START STEPPER [ OPCODE = 0x12 ] [ Configuration ] [ Accel Rate ] [ TimeBase ]  [ Accel Change ]
        //[Configuration] = [Direction] [Decay Mode] [] [] [MS3] [MS2] [MS1] [MS0]        
      case (START_STEPPER):
            AccelRate = SerialBuffer[2];
            AccelTimeBase = SerialBuffer[3];
            AccelRateChange = SerialBuffer[4];
            tmpAccelTimeBase = AccelTimeBase;
            WithDisable = false;
            StepPeriod = StartStepperSpeed;                                       //Slowest Startup Speed
            TBCCR6 = StepPeriod;


            if (SerialBuffer[1] & BIT7)
              {
                MS_Direction = true;
              }
            else
              {
                MS_Direction = false;
              }
            
            if (SerialBuffer[1] & BIT6)                                         //Slow / Fast Decay Mode Selected
              {
                P2DIR |= DECAYA;
                P5DIR |= DECAYB;
              }
            else                                                                //Mixed Decay Mode Selected
              {
                P2DIR &= ~DECAYA;
                P5DIR &= ~DECAYB;
              }
            
            if ((SerialBuffer[1] & 0x0F) < 12)
              {
              MS_Divider = 1 << (SerialBuffer[1] & 0x0F);
              }
            else
              {
              MS_Divider = 1;
              }

            if (AccelRateChange > 0)
            {
              AccelerateState = S_CURVE_ACCELI;
            }
            else
            {
              AccelerateState = ACCEL;
            }
            PHASEA_PORT |= (ENABLEA + I4_BIT + I3_BIT + I2_BIT + I1_BIT + I0_BIT);                                              //Use maximum current on DRV8828/29 Ix bits
            PHASEB_PORT |= (ENABLEB + I4_BIT + I3_BIT + I2_BIT + I1_BIT + I0_BIT);                                              //Use maximum current on DRV8828/29 Ix bits
            TBCCTL6 &= 0xFF1F;                                          // Clear OUTMODx bits; 3 MSB on lower byte; Timer configured to output mode
            TBCCTL6 |= TB_OUTMOD_TOGGLE + CCIE;                         //Configure the timer to reset
            
            break;
// STOP STEPPER [ OPCODE = 0x13 ] [ StepPeriod Hi ] [ StepPeriod Lo ] [ Not Used ] [ Not Used ]
      case (STOP_STEPPER):
        
            DesiredStepperSpeed = (SerialBuffer[1] * 256) + SerialBuffer[2];   //Configure the StepPeriod Rate;                               //Speed to stop at
            
            if (SerialBuffer[3] != 0)
              {
              WithDisable = true;
              }
            
            if (SerialBuffer[4] != 0)
              {
              AccelerateState = S_CURVE_DECELI;  
              }
            else
              {
              AccelerateState = STOP;    
              }
            tmpAccelTimeBase = AccelTimeBase;
      break;
// STEPPER_SPEED [ OPCODE = 0x14 ] [ StepPeriod Hi ] [ StepPeriod Lo ] [ Accel Rate ] [ Accel Time Base ]
      case (STEPPER_SPEED):
            TBCCTL6 &= 0xFF1F;                                          // Clear OUTMODx bits; 3 MSB on lower byte; Timer configured to output mode
            TBCCTL6 |= TB_OUTMOD_TOGGLE + CCIE;                         //Configure the timer to reset
            DesiredStepperSpeed = (SerialBuffer[1] * 256) + SerialBuffer[2];   //Configure the Stepper Speed
            AccelRate = SerialBuffer[3];
            AccelTimeBase = SerialBuffer[4];
            tmpAccelTimeBase = AccelTimeBase;
            if ((unsigned int)DesiredStepperSpeed < (unsigned int)StepPeriod)
            {
                AccelerateState = ACCEL;
            }
            else
            {
                AccelerateState = DECEL;
            }
      break;

// ReConfig Stepper [ OPCODE = 0x15 ] [ CONFIGURATION ] [ Not Used ] [ Not Used ] [ Not Used ]
      case (RECONFIG_STEPPER):

            if (SerialBuffer[1] & BIT7)
              {
                MS_Direction = true;
              }
            else
              {
                MS_Direction = false;
              }
            
            if (SerialBuffer[1] & BIT6)                                         //Slow / Fast Decay Mode Selected
              {
                P2DIR |= DECAYA;
                P5DIR |= DECAYB;
              }
            else                                                                //Mixed Decay Mode Selected
              {
                P2DIR &= ~DECAYA;
                P5DIR &= ~DECAYB;
              }
          
            if ((SerialBuffer[1] & 0x0F) < 12)
              {
              OldMS_Divider =  MS_Divider;
              NewMS_Divider = 1 << (SerialBuffer[1] & 0x0F);

              if (NewMS_Divider != OldMS_Divider)
                  {
                  MS_Divider = NewMS_Divider;
                  MS_Index &= MS_Divider - 1;
                  }
              }
            else
              {
              MS_Divider = 1;
              }
        
        break;           

        // CONFIG MOVE_STEPS [ OPCODE = 0x16 ] [ STEPS Hi ] [ STEPS Lo ] [ ACCEL %] [Decel %]
      case (CONFIG_MOVE_STEPS):

            StepsToMove = ((SerialBuffer[1] * 256) + SerialBuffer[2]);      //Configure the Number Of Steps
            AccelSteps = (unsigned long) StepsToMove * SerialBuffer[3] / 100;               //Segregate total amount of steps into a small portion for ACCEL and DECEL and the rest for AtSpeed
            DecelSteps = (unsigned long) StepsToMove * SerialBuffer[4] / 100;               //Segregate total amount of steps into a small portion for ACCEL and DECEL and the rest for AtSpeed
            AtSpeedSteps = StepsToMove - AccelSteps - DecelSteps;
            MoveSteps = true;                                               //Engage Steps ISR
            MoveStepsState = STEPS_ACCEL;
            StepCounter = 0;                                                //Clear Step Counter
            
            //Debug
            //AccelIndex = 0;
            break;
        
// Read Memory [Opcode = 0xE0 ] [ Address Hi ] [ Address Lo ] [ Not Used ] [ Not Used ] [ Not Used ]
      case (READ_MEM):

       Address = (SerialBuffer[1]*256 + SerialBuffer[2]);
       MyPointer = (int *) Address;
       SerialOutBuffer[1] = (*MyPointer & 0xFF00) >> 8;
       SerialOutBuffer[2] = (*MyPointer & 0xFF);
      break;
// Write Memory [Opcode = 0xE1 ] [ Address Hi ] [ Address Lo ] [ Data Hi ] [ Data Lo ] [ Not Used ]
      case (WRITE_WMEM):

        Address = (SerialBuffer[1]*256 + SerialBuffer[2]);
        Data = (SerialBuffer[3]*256 + SerialBuffer[4]);
        MyPointer = (int *) Address;
        *MyPointer = Data;
        break;

// Write Byte Memory [Opcode = 0xE2 ] [ Address ] [ Address Lo ] [ Data ] [Not Used ] [ Not Used ]
      case (WRITE_BMEM):

        Address = (SerialBuffer[1]*256 + SerialBuffer[2]);
        BData = SerialBuffer[3];
        MyPtr = (char *) Address;
        *MyPtr = BData;
        break;

      case (RESET_MCU):
        WDTCTL = 0x00;
        break;
      case (SHOW_CLKS):
        P5SEL |= SerialBuffer[1] & 0xF0;
        break;
      }
    MessageComplete = false;
    SerialOutPointer = 0;
    UCA0TXBUF = SerialOutBuffer[SerialOutPointer];
    IE2 |= UCA0TXIE;
    }
}





