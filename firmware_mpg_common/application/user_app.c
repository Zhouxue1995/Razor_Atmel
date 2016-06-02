/**********************************************************************************************************************
File: user_app.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app as a template:
 1. Copy both user_app.c and user_app.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserAppInitialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserAppRunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserAppFlags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp_StateMachine;            /* The state machine function pointer */
static u32 UserApp_u32Timeout;   
void delay_ms(u16 u16itime)
{
 while(u16itime--);
}
/* Timeout counter used across states */


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
Function: UserAppInitialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserAppInitialize(void)
{
   PWMAudioSetFrequency(BUZZER1, 500);
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp_StateMachine = UserAppSM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp_StateMachine = UserAppSM_FailedInit;
  }

} /* end UserAppInitialize() */


/*----------------------------------------------------------------------------------------------------------------------
Function UserAppRunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserAppRunActiveState(void)
{
  UserApp_StateMachine();

} /* end UserAppRunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void UserAppSM_Idle(void)
{
  static u16 u16Counter = 490;
  
  u16Counter++;
  /* if(IsButtonPressed(BUTTON0))
  {
    PWMAudioOn(BUZZER1);
  }
  else
  {
    PWMAudioOff(BUZZER1);
  } */
  if(u16Counter < 6000 || (u16Counter > 6500 &&u16Counter < 8000)||(u16Counter > 8500&& u16Counter < 14000)||u16Counter > 14500 && u16Counter < 16000)
  {
    PWMAudioOn(BUZZER1);
  }
  
  //if(WasButtonPressed(BUTTON0))
  
  if(u16Counter == 500)
  {
    PWMAudioSetFrequency(BUZZER1, 1046);
    
  }

  if(u16Counter == 1000)
  {
    PWMAudioSetFrequency(BUZZER1, 1175);
    
    
  }
   if(u16Counter == 1500)
  {
    PWMAudioSetFrequency(BUZZER1, 1318);
    
  }
   if(u16Counter == 2000)
  {
    PWMAudioSetFrequency(BUZZER1, 1046);
    
  }
  
  ///////////////////////////
  if(u16Counter == 2500)
  {
    PWMAudioSetFrequency(BUZZER1, 1046);
  }
  if(u16Counter == 3000)
  {
    PWMAudioSetFrequency(BUZZER1, 1175);
  }
  if(u16Counter == 3500)
  {
    PWMAudioSetFrequency(BUZZER1, 1318);
  }
  if(u16Counter == 4000)
  {
    PWMAudioSetFrequency(BUZZER1, 1046);
  }
  
  /////////////////////
  if(u16Counter == 4500)
  {
    PWMAudioSetFrequency(BUZZER1, 1318);
  }
  if(u16Counter == 5000)
  {
    PWMAudioSetFrequency(BUZZER1, 1397);
  }
  if(u16Counter == 5500)
  {
    PWMAudioSetFrequency(BUZZER1, 1568);
  }
  
   if(u16Counter == 6000)
      PWMAudioOff(BUZZER1);
  
  if(u16Counter == 6500)
  {
    PWMAudioSetFrequency(BUZZER1, 1318);
  }
  if(u16Counter == 7000)
  {
    PWMAudioSetFrequency(BUZZER1, 1397);
  }
  if(u16Counter == 7500)
  {
    PWMAudioSetFrequency(BUZZER1, 1568);
  }
  
   if(u16Counter == 8000)
      PWMAudioOff(BUZZER1);
  ////////////////////////
  if(u16Counter == 8500)
  {
    PWMAudioSetFrequency(BUZZER1, 1568);
  }
  if(u16Counter == 8750)
  {
    PWMAudioSetFrequency(BUZZER1, 1760);
  }
  if(u16Counter == 9000)
  {
    PWMAudioSetFrequency(BUZZER1, 1568);
  }
  if(u16Counter == 9250)
  {
    PWMAudioSetFrequency(BUZZER1, 1397);
  }
  if(u16Counter == 9500)
  {
    PWMAudioSetFrequency(BUZZER1, 1318);
  }
  if(u16Counter == 10000)
  {
    PWMAudioSetFrequency(BUZZER1, 1046);
  }
  
  
  if(u16Counter == 10500)
  {
    PWMAudioSetFrequency(BUZZER1, 1568);
  }
  if(u16Counter == 10750)
  {
    PWMAudioSetFrequency(BUZZER1, 1760);
  }
  if(u16Counter == 11000)
  {
    PWMAudioSetFrequency(BUZZER1, 1568);
  }
  if(u16Counter == 11250)
  {
    PWMAudioSetFrequency(BUZZER1, 1397);
  }
  if(u16Counter == 11500)
  {
    PWMAudioSetFrequency(BUZZER1, 1318);
  }
  if(u16Counter == 12000)
  {
    PWMAudioSetFrequency(BUZZER1, 1046);
  }
 /////////////////////// 
  
  if(u16Counter == 12500)
  {
    PWMAudioSetFrequency(BUZZER1, 1175);
  }
    if(u16Counter == 13000)
  {
    PWMAudioSetFrequency(BUZZER1, 784);
  }
  if(u16Counter == 13500)
  {
    PWMAudioSetFrequency(BUZZER1, 1046);
  }
  
  if(u16Counter == 14000)
     PWMAudioOff(BUZZER1);
  
  if(u16Counter == 14500)
  {
    PWMAudioSetFrequency(BUZZER1, 1175);
  }
    if(u16Counter == 15000)
  {
    PWMAudioSetFrequency(BUZZER1, 784);
  }
  if(u16Counter == 15500)
  {
    PWMAudioSetFrequency(BUZZER1, 1046);
  }

   /* Tone is on as long as button is pressed */
  /*
  if( IsButtonPressed(BUTTON0) || IsButtonPressed(BUTTON1) ||
      IsButtonPressed(BUTTON2) || IsButtonPressed(BUTTON3) )
  {
    PWMAudioOn(BUZZER1);
  }
  else
  {
    PWMAudioOff(BUZZER1);    
  } 
  */
  if(u16Counter == 16000)
  {
    PWMAudioOff(BUZZER1);
     
  }
  if(u16Counter == 16500)
  {
    u16Counter = 490;
  }
   
} /* end UserAppSM_Idle() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserAppSM_Error(void)          
{
  
} /* end UserAppSM_Error() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserAppSM_FailedInit(void)          
{
    
} /* end UserAppSM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
