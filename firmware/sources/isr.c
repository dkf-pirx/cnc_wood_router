//******************************************************************************
//  Jose I Quinones
//  Texas Instruments Inc.
//  May 2011
//  Built with IAR Embedded Workbench Version: 5.1
//******************************************************************************

#include "config.h"
#include "WaveForms.h"

int DebugVar;

/************************************************************
* Interrupt Vectors (offset from 0xFFC0)
*************************************************************
#define DAC12_VECTOR        (14 * 2u) 0xFFDC DAC12 
#define DMA_VECTOR          (15 * 2u) 0xFFDE DMA
#define USCIAB1TX_VECTOR    (16 * 2u) 0xFFE0 USCI A1/B1 Transmit
#define USCIAB1RX_VECTOR    (17 * 2u) 0xFFE2 USCI A1/B1 Receive
#define PORT1_VECTOR        (18 * 2u) 0xFFE4 Port 1
#define PORT2_VECTOR        (19 * 2u) 0xFFE6 Port 2
#define ADC12_VECTOR        (21 * 2u) 0xFFEA ADC
#define USCIAB0TX_VECTOR    (22 * 2u) 0xFFEC USCI A0/B0 Transmit
#define USCIAB0RX_VECTOR    (23 * 2u) 0xFFEE USCI A0/B0 Receive
#define TIMERA1_VECTOR      (24 * 2u) 0xFFF0 Timer A CC1-2, TA
#define TIMERA0_VECTOR      (25 * 2u) 0xFFF2 Timer A CC0
#define WDT_VECTOR          (26 * 2u) 0xFFF4 Watchdog Timer
#define COMPARATORA_VECTOR  (27 * 2u) 0xFFF6 Comparator A
#define TIMERB1_VECTOR      (28 * 2u) 0xFFF8 Timer B CC1-6, TB
#define TIMERB0_VECTOR      (29 * 2u) 0xFFFA Timer B CC0
#define NMI_VECTOR          (30 * 2u) 0xFFFC Non-maskable
#define RESET_VECTOR        (31 * 2u) 0xFFFE Reset [Highest Priority]*/

static void
__attribute__((__interrupt__(DAC12_VECTOR)))
DigitalToAnalogConverter (void)
//#pragma vector=DAC12_VECTOR
//__interrupt void DigitaltoAnalogConverter(void)
{

}

static void
__attribute__((__interrupt__(DMA_VECTOR)))
DigitalToAnalog_DMA (void)
//#pragma vector=DMA_VECTOR
//__interrupt void DigitalToAnalog_DMA(void)
{

}

static void
__attribute__((__interrupt__(USCIAB1TX_VECTOR)))
USCIAB1_Transmit (void)
//#pragma vector=USCIAB1TX_VECTOR
//__interrupt void USCIAB1_Transmit(void)
{

}

static void
__attribute__((__interrupt__(USCIAB1RX_VECTOR)))
USCIAB1_Receive (void)
//#pragma vector=USCIAB1RX_VECTOR
//__interrupt void USCIAB1_Receive(void)
{

}

static void
__attribute__((__interrupt__(PORT1_VECTOR)))
Port1_Change (void)
//#pragma vector=PORT1_VECTOR
//__interrupt void PORT1_Change(void)
{

}

static void
__attribute__((__interrupt__(PORT2_VECTOR)))
Port2_Change (void)
//#pragma vector=PORT2_VECTOR
//__interrupt void Port2_Change(void)
{

}

static void
__attribute__((__interrupt__(ADC12_VECTOR)))
AnalogToDigitalConverter (void)
//#pragma vector=ADC12_VECTOR
//__interrupt void AnalogToDigitalConverter(void)
{

}

static void
__attribute__((__interrupt__(TIMERA1_VECTOR)))
TimerA1 (void)
//#pragma vector=TIMERA1_VECTOR
//__interrupt void TimerA1(void)
{
  
//******************************************************************************
// TACCRx interrupts handle the microstep selection while on input control mode 
// These interrupts are disabled during Serial Control Mode
//******************************************************************************    
  switch (TAIV)
    {
    case TACCR1_CCIFG_SET:

    OldMS_Divider =  MS_Divider;
    NewMS_Divider = (CNTL_PORT1 & (USM2_BIT + USM1_BIT + USM0_BIT)) >> 1;
    NewMS_Divider = 1 << NewMS_Divider;

    if (NewMS_Divider != OldMS_Divider)
      {
      MS_Divider = NewMS_Divider;
      MS_Index &= MS_Divider - 1;
      }     
     
    break;
    case TACCR2_CCIFG_SET:

    OldMS_Divider =  MS_Divider;
    NewMS_Divider = (CNTL_PORT1 & (USM2_BIT + USM1_BIT + USM0_BIT)) >> 1;
    NewMS_Divider = 1 << NewMS_Divider;

    if (NewMS_Divider != OldMS_Divider)
      {
      MS_Divider = NewMS_Divider;
      MS_Index &= MS_Divider - 1;
      }    
      
    break;
    case TAIFG_SET:
    break;
    }
}

static void
__attribute__((__interrupt__(WDT_VECTOR)))
Watchdog_Timer (void)
//#pragma vector=WDT_VECTOR
//__interrupt void Watchdog_Timer(void)
{

}

static void
__attribute__((__interrupt__(COMPARATORA_VECTOR)))
ComparatorA (void)
//#pragma vector=COMPARATORA_VECTOR
//__interrupt void ComparatorA(void)
{

}

static void
__attribute__((__interrupt__(TIMERB1_VECTOR)))
TimerB1 (void)
//#pragma vector=TIMERB1_VECTOR
//__interrupt void TimerB1(void)
{
switch (TBIV)
{
//******************************************************************************
// TBCCR1 handles the Stepping Actuation through the STEP input on any rising edge
//******************************************************************************    
case TBCCR1_CCIFG_SET:

 
  if (MS_Direction) //MS Direction determines whether we move forward (+) or backwards (-) on the lookup table
    {
    MS_Index += MS_Divider;  
    }
  else
    {
    MS_Index -= MS_Divider;  
    }

  int TempPhaseDecay, TempPhasePort;
  
  TempPhaseDecay = SineWave[MS_Index & (TABLE_DEPTH - 1)] >> 8;    //Extract the Decay and Phase information from the Most Significant Nibble

  TempPhaseDecay &= 0x60;                                                       //Extract Only DECAY (BIT14) and PHASE (BIT13) information
  TempPhasePort = PHASEA_PORT;
  TempPhasePort &= 0x9F;
  TempPhasePort |= TempPhaseDecay;
  if (!MS_Direction)                                                            // If the direction is reverse, the DECAY information must be inverted
    {
      TempPhasePort ^= DECAYA;
    }
  PHASEA_PORT = TempPhasePort;                                                  

  TempPhaseDecay = SineWave[(MS_Index + (TABLE_DEPTH / 4)) & (TABLE_DEPTH - 1)] >> 8;    //Extract the Decay and Phase information from the Most Significant Nibble
  TempPhaseDecay &= 0x60;                                                       //Extract Only DECAY (BIT14) and PHASE (BIT13) information
  TempPhasePort = PHASEB_PORT;
  TempPhasePort &= 0x9F;
  TempPhasePort |= TempPhaseDecay;
    if (!MS_Direction)                                                          // If the direction is reverse, the DECAY information must be inverted
    {
      TempPhasePort ^= DECAYB;
    }
  PHASEB_PORT = TempPhasePort; 
  
  DAC12_0DAT = SineWave[MS_Index & (TABLE_DEPTH - 1)];
  DAC12_1DAT = SineWave[(MS_Index + (TABLE_DEPTH / 4)) & (TABLE_DEPTH - 1)];
  
  
  break;
//******************************************************************************
// TBCCR2 handles the Reset Index Input on both rising and falling edges
//******************************************************************************  
case TBCCR2_CCIFG_SET:
  
    if (CNTL_PORT2 & RSTBIT)
    {
      MS_Index = 0;
      TBCCTL1 &= ~CCIE;                     //Disable STEP INTERRUPT
    }
  else
    {
      TBCCTL1 |= CCIE;                      //ENABLE STEP INTERRUPT
    }
  break;
//******************************************************************************
// TBCCR3 handles the ENABLE Stepper Input on both rising and falling edges
// A rising edge enables the stepper. A falling edge disables the stepper.
//******************************************************************************  
case TBCCR3_CCIFG_SET:
   if (CNTL_PORT2 & ENABIT)
    {
      PHASEA_PORT |= ENABLEA;
      PHASEB_PORT |= ENABLEB;
      TBCCTL1 |= CCIE;                      //ENABLE Stepper and STEP INTERRUPT
    }
  else
    {
      PHASEA_PORT &= ~ENABLEA;
      PHASEB_PORT &= ~ENABLEB;
      TBCCTL1 &= ~CCIE;                     //Disable Stepper and STEP INTERRUPT
    } 
  break;
//******************************************************************************
// TBCCR4 handles the Direction Input on both rising and falling edges
// A rising edge sets forward motion. A falling sets reverse motion.
//******************************************************************************  
case TBCCR4_CCIFG_SET:

  if (CNTL_PORT2 & DIRBIT)
    {
    MS_Direction = true;
    }
  else
    {
    MS_Direction = false;
    }
  break;

case TBCCR5_CCIFG_SET:
  break;
//******************************************************************************
// TBCCR6 handles the Stepping Actuation for serial communuications through the
// timer option.
//******************************************************************************  
case TBCCR6_CCIFG_SET:
 // PHASEA_PORT ^= I0_BIT;

  if (MoveSteps)
    {
      switch (MoveStepsState)
      {
      case STEPS_ACCEL:
        if (StepCounter >= AccelSteps)
          {
            MoveStepsState = STEPS_AT_SPEED;
            AccelerateState = NOACC;
          }
        break;
      case STEPS_AT_SPEED:
        if (StepCounter >= (AccelSteps + AtSpeedSteps))
          {
            MoveStepsState = STEPS_DECEL;
            DesiredStepperSpeed = StartStepperSpeed;
            AccelerateState = DECEL;            
          }
        break;
      case STEPS_DECEL:
        {
          if (StepCounter >= (StepsToMove))
          {
            MoveStepsState = STEPS_NONE;
            AccelerateState = NOACC;            
            TBCCTL6 &= 0xFF0F;                                          //Disable CC and Interrupt
            MoveSteps = false;
          }
        }
        break;
      }
      StepCounter += 1;
    }
 
  TBCCR6 += StepPeriod;
  
  if (MS_Direction) //MS Direction determines whether we move forward (+) or backwards (-) on the lookup table
    {
    MS_Index += MS_Divider;  
    }
  else
    {
    MS_Index -= MS_Divider;  
    }

  TempPhaseDecay = SineWave[MS_Index & (TABLE_DEPTH - 1)] >> 8;    //Extract the Decay and Phase information from the Most Significant Nibble

  TempPhaseDecay &= 0x60;                                                       //Extract Only DECAY (BIT14) and PHASE (BIT13) information
  TempPhasePort = PHASEA_PORT;
  TempPhasePort &= 0x9F;
  TempPhasePort |= TempPhaseDecay;
  if (!MS_Direction)                                                            // If the direction is reverse, the DECAY information must be inverted
    {
      TempPhasePort ^= DECAYA;
    }
  PHASEA_PORT = TempPhasePort;                                                  

  TempPhaseDecay = SineWave[(MS_Index + (TABLE_DEPTH / 4)) & (TABLE_DEPTH - 1)] >> 8;    //Extract the Decay and Phase information from the Most Significant Nibble
  TempPhaseDecay &= 0x60;                                                       //Extract Only DECAY (BIT14) and PHASE (BIT13) information
  TempPhasePort = PHASEB_PORT;
  TempPhasePort &= 0x9F;
  TempPhasePort |= TempPhaseDecay;
    if (!MS_Direction)                                                          // If the direction is reverse, the DECAY information must be inverted
    {
      TempPhasePort ^= DECAYB;
    }
  PHASEB_PORT = TempPhasePort; 
  
  DAC12_0DAT = SineWave[MS_Index & (TABLE_DEPTH - 1)];
  DAC12_1DAT = SineWave[(MS_Index + (TABLE_DEPTH / 4)) & (TABLE_DEPTH - 1)];

 // PHASEA_PORT ^= I0_BIT;  
  break;
case TBIFG_SET:
  break;
}
}

static void
__attribute__((__interrupt__(TIMERB0_VECTOR)))
TimerB0 (void)
//#pragma vector=TIMERB0_VECTOR
//__interrupt void TimerB0(void)
{
  
}

 static void
__attribute__((__interrupt__(NMI_VECTOR)))
NonMaskableInterrupt (void)
//#pragma vector=NMI_VECTOR
//__interrupt void NonMaskableInterrupt(void)
{

}

