//******************************************************************************
//  Jose I Quinones
//  Texas Instruments Inc.
//  May 2011
//  Built with IAR Embedded Workbench Version: 5.1
//******************************************************************************
#include "config.h"

void AppInit(void)
{
  DAC12_0CTL = 0x4140;
  DAC12_1CTL = 0x4140;
  MS_Index = 0;
  PHASEA_PORT |= (I4_BIT + I3_BIT + I2_BIT + I1_BIT + I0_BIT);                                              //Use maximum current on DRV8828/29 Ix bits
  PHASEB_PORT |= (I4_BIT + I3_BIT + I2_BIT + I1_BIT + I0_BIT);                                              //Use maximum current on DRV8828/29 Ix bits

  //******************************************************************************
  // Microcontroller Mode Configuration - Serial vs Parallel Mode
  //******************************************************************************
  if ( CNTL_PORT1 & nUART_MODE )
  {
    //******************************************************************************
    // Configure for Parallel Interfacing Mode. Disable the UART Engine
    //******************************************************************************
    
    IE2 &= ~( UCA0RXIE + UCA0TXIE);     //Disable UART0 Interrupt
  
    TBCCTL2 |= CCIE;                //Enable RESET INDEX Interrupt
    TBCCTL3 |= CCIE;                //Enable ENABLE STEPPER Interrupt
    TBCCTL4 |= CCIE;                //Enable DIRECTION Interrupt

    TACCTL0 |= CCIE;                //ENABLE USM0 Interrupt
    TACCTL1 |= CCIE;                //ENABLE USM1 Interrupt
    TACCTL2 |= CCIE;                //ENABLE USM2 Interrupt

    P2DIR &= ~DECAYA;
    P5DIR &= ~DECAYB;
                
    // ENABLE_COMPUTE
    if (CNTL_PORT2 & ENABIT)
    {
      PHASEA_PORT |= ENABLEA;
      PHASEB_PORT |= ENABLEB;
      TBCCTL1 |= CCIE;                        //ENABLE STEP INTERRUPT
    }
    else
    {
      PHASEA_PORT &= ~ENABLEA;
      PHASEB_PORT &= ~ENABLEB;
      TBCCTL1 &= ~CCIE;                      //DISABLE STEP INTERRUPT
    }
  
    //RESET INDEX COMPUTE
    if (CNTL_PORT2 & RSTBIT)
    {
      TBCCTL1 &= ~CCIE;                     //Disable STEP INTERRUPT
    }
    else
    {
      TBCCTL1 |= CCIE;                      //ENABLE STEP INTERRUPT
    }
  
    //DIRECTION SELECT
    if (CNTL_PORT2 & DIRBIT)
    {
      MS_Direction = true;
    }
    else
    {
      MS_Direction = false;
    }

    // Microstepping Degrees Select
    // 7:FF00:512STEPS, 6:FF80:256STEPS; 5:FFC0:128STEPS; 4:FFE0:64STEPS; 3:FFF0:32STEPS; 2:FFF8:16STEPS; 1:FFFC:8STEPS; 0:FFFE:4STEPS

    NewMS_Divider = (CNTL_PORT1 & (USM2_BIT + USM1_BIT + USM0_BIT)) >> 1;
    MS_Divider = 1 << NewMS_Divider;
  
    UARTMode = false;

  }
  else
  {
    //******************************************************************************
    //Configure for Serial Interfacing Mode. Disable Parallel Control Interrupts
    //******************************************************************************

    IE2 |= UCA0RXIE + UCA0TXIE;     //ENABLE UARTA0 Interrupt
  
    TBCCTL1 &= ~CCIE;                //Disable STEP Interrupt
    TBCCTL2 &= ~CCIE;                //Disable RESET INDEX Interrupt
    TBCCTL3 &= ~CCIE;                //Disable ENABLE STEPPER Interrupt
    TBCCTL4 &= ~CCIE;                //Disable DIRECTION Interrupt

    TACCTL0 |= CCIE;                 //ENABLE TASK TIMER Interrupt
    TACCTL1 &= ~CCIE;                //Disable USM1 Interrupt
    TACCTL2 &= ~CCIE;                //Disable USM2 Interrupt
  
    TACCTL0 &= ~TA_CAP_CAPT;         //Timer A0 is Output Compare
  
    UARTMode = true;
  }

  //Micro present or HiZ
  if ( !( P1IN & MCU_MODE ) )
  {
    //MCU is HiZ All GPIO Configured as Inputs and MCU completely disabled
    P1DIR = 0;
    P2DIR = 0;
    P3DIR = 0;
    P4DIR = 0;
    P5DIR = 0;
    P6DIR = 0;

    __bic_SR_register( LPM4_bits );   //MCU is completely disabled. Only NMI available.
  }
}
