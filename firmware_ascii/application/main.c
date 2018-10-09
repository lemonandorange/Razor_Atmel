/***********************************************************************************************************************
File: main.c                                                                

Description:
Container for the EiE firmware.  
***********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32SystemFlags = 0;                     /* Global system flags */
volatile u32 G_u32ApplicationFlags = 0;                /* Global applications flags: set when application is successfully initialized */

/*--------------------------------------------------------------------------------------------------------------------*/
/* External global variables defined in other files (must indicate which file they are defined in) */
extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

static u16 u16BlinkCount = 0;

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "Main_" and be declared as static.
***********************************************************************************************************************/


/***********************************************************************************************************************
Main Program
Main has two sections:

1. Initialization which is run once on power-up or reset.  All drivers and applications are setup here without timing
contraints but must complete execution regardless of success or failure of starting the application. 

2. Super loop which runs infinitely giving processor time to each application.  The total loop time should not exceed
1ms of execution time counting all application execution.  SystemSleep() will execute to complete the remaining time in
the 1ms period.
***********************************************************************************************************************/

void main(void)
{
  G_u32SystemFlags |= _SYSTEM_INITIALIZING;

  /* Low level initialization */
  WatchDogSetup(); /* During development, does not reset processor if timeout */
  GpioSetup();
  ClockSetup();
  InterruptSetup();
  SysTickSetup();

  /* Driver initialization */
  MessagingInitialize();
  UartInitialize();
  DebugInitialize();

  /* Debug messages through DebugPrintf() are available from here */

  ButtonInitialize();
  TimerInitialize();  
  SspInitialize();
  TWIInitialize();
  Adc12Initialize();
  
  LcdInitialize();
  LedInitialize();
  AntInitialize();
  AntApiInitialize();
  SdCardInitialize();

  /* Application initialization */

  UserApp1Initialize();
  UserApp2Initialize();
  UserApp3Initialize();

  
  /* Exit initialization */
  SystemStatusReport();
  G_u32SystemFlags &= ~_SYSTEM_INITIALIZING;
    
  /* Super loop */  
  while(1)
  {
    WATCHDOG_BONE();
    
    /* Drivers */
    LedUpdate();
    ButtonRunActiveState();
    UartRunActiveState();
    TimerRunActiveState(); 
    SspRunActiveState();
    TWIRunActiveState();
    Adc12RunActiveState();
    MessagingRunActiveState();
    DebugRunActiveState();
    LcdRunActiveState();
    AntRunActiveState();
    AntApiRunActiveState();
    SdCardRunActiveState();

    /* Applications */
    UserApp1RunActiveState();
    UserApp2RunActiveState();
    UserApp3RunActiveState();
    
    /* System sleep*/
    HEARTBEAT_OFF();
    SystemSleep();
    HEARTBEAT_ON();
    
    
    
    LedOff(WHITE);
    LedOff(PURPLE);
    LedOff(BLUE);
    LedOff(CYAN);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOff(ORANGE);
    LedOff(RED);
    
    u16BlinkCount++; 
    if(u16BlinkCount>=0 && u16BlinkCount <900) 
    {   
      LedPWM(WHITE, LED_PWM_100);
    } 
    if(u16BlinkCount>=900 && u16BlinkCount <1700) 
    {  
      LedOff(WHITE);
      LedOn(PURPLE);
      LedPWM(PURPLE, LED_PWM_90);
    } 
    if(u16BlinkCount>=1700 && u16BlinkCount <2300) 
    { 
      LedOff(PURPLE);
      LedOn(BLUE);
      LedPWM(BLUE, LED_PWM_80);
    }
    if(u16BlinkCount>=2300 && u16BlinkCount <2800) 
    {  
      LedOff(BLUE);
      LedOn(CYAN);
      LedPWM(CYAN, LED_PWM_70);
    }
    if(u16BlinkCount>=2800 && u16BlinkCount <3200) 
    { 
      LedOff(CYAN);
      LedOn(GREEN);
      LedPWM(GREEN, LED_PWM_60);
    }
    if(u16BlinkCount>=3200 && u16BlinkCount <3500) 
    { 
      LedOff(GREEN);
      LedOn(YELLOW);
      LedPWM(YELLOW, LED_PWM_50);
    }
    if(u16BlinkCount>=3500 && u16BlinkCount <3700) 
    { 
      LedOff(YELLOW);
      LedOn(ORANGE);
      LedPWM(ORANGE, LED_PWM_40);
    }
    if(u16BlinkCount>=3700 && u16BlinkCount <3800) 
    { 
      LedOff(ORANGE);
      LedOn(RED);
      LedPWM(RED, LED_PWM_30);
      u16BlinkCount = 0; 
    }
    
  } /* end while(1) main super loop */
  
} /* end main() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File */
/*--------------------------------------------------------------------------------------------------------------------*/
