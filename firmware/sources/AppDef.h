//******************************************************************************
//  Jose I Quinones
//  Texas Instruments Inc.
//  May 2011
//  Built with IAR Embedded Workbench Version: 5.1
//******************************************************************************
//***********************************************************************************
// App Extern Globals                                                               *
//***********************************************************************************
#include <stdbool.h>

extern unsigned int StepPeriod;
extern unsigned int SCurveAccelSpeed, SCurveDecelSpeed;
extern unsigned int AccelRate, AccelRateChange, StartStepperSpeed, DesiredStepperSpeed;
extern unsigned int tmpAccelTimeBase, AccelTimeBase;
extern unsigned int AccelerateState;
extern unsigned int StepsToMove;
extern unsigned int MS_Divider, OldMS_Divider, NewMS_Divider;
extern unsigned int StepCounter, AccelSteps, AtSpeedSteps, MoveStepsState;

extern bool MS_Direction;
extern bool MoveSteps;
extern bool HighResolutionMode, UARTMode;
extern bool WithDisable;

extern char StatusLED;

extern unsigned long MS_Index;

//Debug 
//extern int AccelIndex;

//***********************************************************************************
// Firmware Revision                                                                *
//***********************************************************************************
#define     FIRMWARE_REVISION   1
//***********************************************************************************
// Application Specific Hardware Definitions                                        *
//***********************************************************************************
#define     StatusLEDPin    BIT7
#define     PHASEA          BIT5    //PORT2.5
#define     PHASEB          BIT5    //PORT5.5
#define     DECAYA          BIT6
#define     DECAYB          BIT6
#define     ENABLEA         BIT7
#define     ENABLEB         BIT7

#define     I0_BIT          BIT0
#define     I1_BIT          BIT1
#define     I2_BIT          BIT2
#define     I3_BIT          BIT3
#define     I4_BIT          BIT4

#define     USM0_BIT        BIT1
#define     USM1_BIT        BIT2
#define     USM2_BIT        BIT3


#define     CNTL_PORT1      P1IN    //PORT1
#define     CNTL_PORT2      P4IN    //PORT4
#define     PHASEA_PORT     P2OUT   //PORT2
#define     PHASEB_PORT     P5OUT   //PORT5

#define     nUART_MODE      BIT7    //PORT1
#define     MCU_MODE      BIT6    //PORT2

#define     DIRBIT          BIT4    //PORT4
#define     ENABIT          BIT3    //PORT4
#define     RSTBIT          BIT2    //PORT4
#define     STPBIT          BIT1    //PORT4

#define     P1OUT_INIT  0
#define     P2OUT_INIT  0
#define     P3OUT_INIT  0
#define     P4OUT_INIT  0
#define     P5OUT_INIT  0
#define     P6OUT_INIT  0

#define     OPCODE      SerialBuffer[0]

#define     NOACC       0
#define     ACCEL       1
#define     DECEL       2
#define     STOP        3

#define     S_CURVE_ACCELI  4                         //Acceleration Increase
#define     S_CURVE_ACCSS   5                         //Acceleration Steady State
#define     S_CURVE_ACCELD  6                         //Acceleration Decrease  

#define     S_CURVE_DECELI  7                         //Deceleration Increase
#define     S_CURVE_DECSS   8                         //Deceleration Steady State
#define     S_CURVE_DECELD  9                         //Deceleration Decrease

#define     STEPS_NONE          0
#define     STEPS_ACCEL         1
#define     STEPS_AT_SPEED      2
#define     STEPS_DECEL         3
#define     STEPS_STOP          4

//***********************************************************************************
// Application Specific Definitions                                                 *
//***********************************************************************************
#define     TABLE_DEPTH     2048   //1024 Degrees of Microstepping
#define     PHASE_MASK      0x2000 //Bit 13 on the Sine Wave Form
#define     DECAY_MASK      0x1000 //Bit 12 on the Sine Wave Form
//***********************************************************************************
// Application Specific Opcodes                                                     *
//***********************************************************************************
#define     WRITE_GPIO      0x00
#define     DAC0_UPDATE     0x01
#define     DAC1_UPDATE     0x02
#define     PULSE_TMR       0x0F
#define     CONFIG_STEPPER  0x11
#define     START_STEPPER   0x12
#define     STOP_STEPPER    0x13
#define     STEPPER_SPEED   0x14
#define     RECONFIG_STEPPER  0x15

#define     CONFIG_MOVE_STEPS 0x16
#define     MOVE_STEPS      0x17

//Memory Access Opcodes
#define     READ_MEM        0xE0
#define     WRITE_WMEM      0xE1
#define     WRITE_BMEM      0xE2

// System Opcodes
#define     RESET_MCU       0xF0
#define     SHOW_CLKS       0xF1
