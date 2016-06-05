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
static u32 UserApp_u32Timeout;                      /* Timeout counter used across states */

static u8 UserApp_au8MyName[] = "A3.ZhouXue";
extern u8 G_au8DebugScanfBuffer[];   
extern u8 G_u8DebugScanfCharCount; 

static u8 au8UserInputBuffer[USER_INPUT_BUFFER_SIZE];
static u8 u8namebuffer[200];

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
  LCDMessage(LINE1_START_ADDR, UserApp_au8MyName);
  LCDClearChars(LINE1_START_ADDR +10 , 11);
  
   /* Backlight to purple */  
  LedOn(LCD_RED);
  LedOff(LCD_GREEN);
  LedOn(LCD_BLUE); 
  
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
  bool flag=FALSE;
  static u8 u8CharCount=0;
  static u8 u8time=0;
  static u8 u8i=0;
  static u8 u8CountReally=0;
  static u8 u8NumberCharacters=0;
  static u8 u8String[] = "\n\rTotal number of charactersis: \n\r";
  static u8 u8String1[] = "\n\rcharacter count cleared! \n\r";
  static u8 u8String2[] = "\n\rname buffer is: \n\r";
  static u8 u8MyName[] = "zhouxue";
  static u8 u8NameCount = 0;
  static u8 u8j=0;
  static u8 u8t=0;
  static u8 u8n=0;
  
  u8time++;
  if(u8time==10)
  {
   u8time=0;
   flag=TRUE;
  }
  else
  {
   flag=FALSE;
  }
  if(flag=TRUE)
  {
    /* Read the buffer and print the contents */
    u8CharCount = DebugScanf(au8UserInputBuffer);
    au8UserInputBuffer[u8CharCount] = '\0';
    for(u8i=0;u8i<u8CharCount;u8i++)
    {
     LCDMessage (LINE2_START_ADDR+u8CountReally,au8UserInputBuffer); 
     
      if(u8CountReally==20)
      {
        LCDClearChars(LINE2_START_ADDR,20); 
        LCDMessage (LINE2_START_ADDR,au8UserInputBuffer); 
        u8CountReally=0;
      }
     
     
      u8CountReally++;
      u8NumberCharacters++;
    }
    flag=FALSE; 
  }
   
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    LCDClearChars(LINE2_START_ADDR,20); 
    u8CountReally=0;
  }
  
  if(WasButtonPressed(BUTTON1))
  {
    ButtonAcknowledge(BUTTON1);
    DebugPrintf(u8String);
    DebugPrintNumber(u8NumberCharacters); 
  }
  
  if(WasButtonPressed(BUTTON2))
  {
    ButtonAcknowledge(BUTTON2);
    LCDClearChars(LINE2_START_ADDR,20); 
    DebugPrintf(u8String1);
    u8CountReally=0;
    u8NumberCharacters=0;
  }
 

    if(u8MyName[u8j]==au8UserInputBuffer[u8t]||u8MyName[u8j]==au8UserInputBuffer[u8t]+32 )
    {
      u8namebuffer[u8n++]=au8UserInputBuffer[u8t];
      u8j++; 
      if(u8j==7)
      {
        u8j=0;
        u8NameCount++;
      } 
    } 
   
  
  
  if(WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    DebugPrintf(u8String2);
    DebugPrintf(u8namebuffer);  
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
