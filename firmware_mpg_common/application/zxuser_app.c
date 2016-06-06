/**********************************************************************************************************************
File: zxuser_app.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this zxuser_app as a template:
 1. Copy both zxuser_app.c and zxuser_app.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "zxuser_app" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "ZxUserApp" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "ZX_USER_APP" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a zxuser_app.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void ZxUserAppInitialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void ZxUserAppRunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32ZxUserAppFlags;                       /* Global state flags */
extern volatile u8 u8n=0;

/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "ZxUserApp_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type ZxUserApp_StateMachine;            /* The state machine function pointer */
static u32 ZxUserApp_u32Timeout;                      /* Timeout counter used across states */

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: ZxUserAppInitialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void ZxUserAppInitialize(void)
{
   PWMAudioSetFrequency(BUZZER1, 500);
  
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    ZxUserApp_StateMachine = ZxUserAppSM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    ZxUserApp_StateMachine = ZxUserAppSM_FailedInit;
  }

} /* end ZxUserAppInitialize() */


/*----------------------------------------------------------------------------------------------------------------------
Function ZxUserAppRunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void ZxUserAppRunActiveState(void)
{
  ZxUserApp_StateMachine();

} /* end ZxUserAppRunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void ZxUserAppSM_Idle(void)
{
  static u16 u16count=0;
  
  u16count++;
  if((u8n%7==0)&&u16count==5000)  
  {
    LedBlink(BLUE,LED_2HZ);
    PWMAudioOn(BUZZER1);
  }
  if(u16count==10000)
  {
    LedOff(BLUE);
    PWMAudioOff(BUZZER1);
  
  }
} /* end ZxUserAppSM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void ZxUserAppSM_Error(void)          
{
  
} /* end ZxUserAppSM_Error() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void ZxUserAppSM_FailedInit(void)          
{
    
} /* end ZxUserAppSM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
