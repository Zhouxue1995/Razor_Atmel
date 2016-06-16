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

static u32 u32DefaultCounter = 0;                   /* Debug variable */

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/
extern AntSetupDataType G_stAntSetupData;                         /* From ant.c */

extern u32 G_u32AntApiCurrentDataTimeStamp;                       /* From ant_api.c */
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    /* From ant_api.c */
extern u8 G_au8AntApiCurrentData[ANT_APPLICATION_MESSAGE_BYTES];  /* From ant_api.c */

static u8 UserApp_au8MyName[] = "Listen to songs ";
extern u8 G_au8DebugScanfBuffer[];   
extern u8 G_u8DebugScanfCharCount; 

static u8 au8UserInputBuffer[USER_INPUT_BUFFER_SIZE];
static u8 u8namebuffer[20];

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
  /* Configure ANT for this application */
  G_stAntSetupData.AntChannel          = ANT_CHANNEL_USERAPP;
  G_stAntSetupData.AntSerialLo         = ANT_SERIAL_LO_USERAPP;
  G_stAntSetupData.AntSerialHi         = ANT_SERIAL_HI_USERAPP;
  G_stAntSetupData.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  G_stAntSetupData.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  G_stAntSetupData.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  G_stAntSetupData.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;
  G_stAntSetupData.AntFrequency        = ANT_FREQUENCY_USERAPP;
  G_stAntSetupData.AntTxPower          = ANT_TX_POWER_USERAPP;
  
  LCDMessage(LINE1_START_ADDR, UserApp_au8MyName);
  LCDClearChars(LINE1_START_ADDR +15 , 5);
  
  LedOn(LCD_RED);
  LedOff(LCD_GREEN);
  LedOn(LCD_BLUE);
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    AntChannelConfig(ANT_MASTER);
    AntOpenChannel();
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
  static bool bButton0Flag=FALSE; 
  static bool bButton1Flag=FALSE; 
  static bool bButton2Flag=FALSE; 
  static bool bButton3Flag=FALSE;
  static bool bButton0CountdownFlag=FALSE; 
  static bool bButton1CountdownFlag=FALSE; 
  static bool bButton2CountdownFlag=FALSE; 
  static bool bButton3CountdownFlag=FALSE; 
  static bool bsendhelp=FALSE;
  static bool bStringTrue[10]="TRUE";
  static bool bStringFalse[10]="FALSE";
  static u16 u16Counter = 480;
  static u8 u8SoundCount = 0;
  static u8 u8time=0;
  static u8 helpcount=0;
  static u8 u8helpcounter=0;
  static u8 HelpAnswerCount=0;
  static u8 counter=0;
  static u8 u8CharCount=0;
  static u8 u8CountReally=0;
  static u8 answer0[]="stars,bells,paint,dream,";
  static u8 answer1[]="bells";
  static u8 answer2[]="paint";
  static u8 answer3[]="dream";
  static u8 u8stringhelp[8]="help";
  static u8 u8strinagain[8]="again";
  static u8 u8sendagain=0;
  static u8 u8adjsong=0;
  
 
   if( AntReadData() )
   {
     /* New data message: check what it is */
      if(G_eAntApiCurrentMessageClass == ANT_DATA)
     {
        LCDClearChars(LINE1_START_ADDR,20);
      /* We got some data: parse it into au8DataContent[] */
       LCDMessage(LINE1_START_ADDR, G_au8AntApiCurrentData);
      /* compare off-site call help answer if true */
       u8adjsong=0;
       for(u8 k=0;k<4;k++)
       {
         u8 i=0;
         for(u8 t=0;t<5;t++)
        {
            if(G_au8AntApiCurrentData[t]==answer0[k*6+t])
            {
              i++;
              if(i==5)
              {
                u8adjsong=1;
              }
              
            }
            else
            {
               u8adjsong=0; 
            }
        }
       if(u8adjsong==1)
       {
          break;
       }
       } 
       if(u8adjsong==1)
       {
          LCDMessage (LINE2_START_ADDR,bStringTrue);
          LCDClearChars(LINE2_START_ADDR+4,16);
       }
       else
       {
          LCDMessage (LINE2_START_ADDR,bStringFalse);
          LCDClearChars(LINE2_START_ADDR+5,15);
       }
       u8sendagain=1;
     }
     else if(G_eAntApiCurrentMessageClass == ANT_TICK)
     {
       /* when call for help ,send it to slave */
     if(bsendhelp) 
      {
        AntQueueBroadcastMessage(u8stringhelp);
        bsendhelp=FALSE;
      }
      if( u8sendagain==1)
      {
         AntQueueBroadcastMessage(u8strinagain);
         u8sendagain=0;
      }
     }
   }

   /* Here a total of 4 songs, the code of each song basically similar,
   I think you only look at the first song to know the code of other songs!*/
    /*Press button0 .The first song.Little Star */
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    bButton0Flag=TRUE;
    u8CountReally=0;
    counter=0;
    LCDClearChars(LINE2_START_ADDR,20); 
  }
  if(bButton0Flag)
  { /* music */
    char music0[100] = "1111011110555505555066660666605555500444404444033330333302222022220111110";
    u16Counter++; 
  
    if((u16Counter-500)%100 == 0 && u16Counter < 7701)
    {
      
          switch(music0[u8SoundCount])
        {
          case '1': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 533);
                  LedOn(WHITE);
                  break;
          case '2': PWMAudioOn(BUZZER2);
                  PWMAudioSetFrequency(BUZZER2, 587);
                  LedOn(PURPLE);
                  break;
          case '3': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 659);
                  LedOn(BLUE);
                  break;
          case '4': PWMAudioOn(BUZZER2);
                  PWMAudioSetFrequency(BUZZER2, 698);
                  LedOn(CYAN);
                  break;
          case '5': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 784);
                  LedOn(GREEN);
                  break;
          case '6': PWMAudioOn(BUZZER2);
                  PWMAudioSetFrequency(BUZZER2, 880);
                  LedOn(YELLOW);
                  break; 
          case '7': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 988);
                  LedOn(ORANGE);
                  break; 
                  /* 5 slow */
          case '8': PWMAudioOn(BUZZER2);
                  PWMAudioSetFrequency(BUZZER2, 392);
                  break; 
          case '0': PWMAudioOff(BUZZER1);
                    PWMAudioOff(BUZZER2);
                    LedOff(WHITE);
                    LedOff(PURPLE);
                    LedOff(BLUE);
                    LedOff(CYAN);
                    LedOff(GREEN);
                    LedOff(YELLOW);
                    LedOff(ORANGE);
                    break; 
        default:
                u32DefaultCounter++;
                break;
        }
        u8SoundCount++;
     }      /* the song end and the red led on */
    if(u16Counter == 7700)
    {
      PWMAudioOff(BUZZER1);
      PWMAudioOff(BUZZER2);
      LedBlink(RED, LED_2HZ);
      LCDClearChars(LINE1_START_ADDR,20);
    }
    
     /* 15 seconds countdown.Print the song name in debug */
    if( (7700 < u16Counter) &&(u16Counter < 22700)  )
    {
      u8time++;
      if(u8time==10)
      {
       u8time=0;
       bButton0CountdownFlag=TRUE;
      }
      else
      {
       bButton0CountdownFlag=FALSE;
      }
      if(bButton0CountdownFlag)
      {
        /* Read the buffer and print the contents */
        u8CharCount = DebugScanf(au8UserInputBuffer);
        au8UserInputBuffer[u8CharCount] = '\0';

        u8adjsong=0;
        for(u8 j=0;j<u8CharCount;j++)
        {
         LCDMessage (LINE1_START_ADDR+u8CountReally,au8UserInputBuffer);
         u8namebuffer[j]=au8UserInputBuffer[j];
         u8CountReally++;
          if(u8CountReally>5)
          {
            LCDMessage (LINE2_START_ADDR,bStringFalse);
            LCDClearChars(LINE2_START_ADDR+5,15);
            u8CountReally=0;
          }
          
          /* Compare the input answer to  really answer */
          if (answer0[counter]==au8UserInputBuffer[j])
          {
            counter++;
            /* the input answer is true */
            if(counter==5)
            {
              u8adjsong=1;
              LCDMessage (LINE2_START_ADDR,bStringTrue);
              LCDClearChars(LINE2_START_ADDR+4,16);
            }
          }
          else
          {
            if(u8stringhelp[helpcount]==au8UserInputBuffer[j])
            { 
              helpcount++;
              if(helpcount==4)
              {
                bsendhelp=TRUE;
                helpcount=0;
              }
            }
            else
            {
              LCDMessage(LINE2_START_ADDR,bStringFalse);
              LCDClearChars(LINE2_START_ADDR+5,15);
            }
   
          }
        }
        bButton0CountdownFlag=FALSE;
     }      
 
   }
    /* 15 seconds countdown is over */
    if(u16Counter == 22700)
    {
     LedOff(RED);
     bButton0Flag=FALSE;
     u16Counter=480;
     u8SoundCount = 0;
    }
  }
 
 
  /*Press button1 .The second song.Jingle Bells  */
  if(WasButtonPressed(BUTTON1))
  {
    ButtonAcknowledge(BUTTON1);
    bButton1Flag=TRUE;
    u8CountReally=0;
    counter=0;
    LCDClearChars(LINE2_START_ADDR,20); 
  }
  if(bButton1Flag)
  { 
    char music1[100] = "3303303300003303303300003305501100223300000044044044044044033033003330330220220110222225550";
    u16Counter++; 
   
  
    if((u16Counter-500)%100 == 0 && u16Counter < 9701)
    {
        
          switch(music1[u8SoundCount])
        {
          case '1': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 533);
                  LedOn(WHITE);
                  break;
          case '2': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 587);
                  LedOn(PURPLE);
                  break;
          case '3': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 659);
                  LedOn(BLUE);
                  LedOff(PURPLE);
                  break;
          case '4': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 698);
                  LedOn(CYAN);
                  LedOff(BLUE);
                  break;
          case '5': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 784);
                  LedOn(GREEN);
                  LedOff(PURPLE);
                  break;
          case '6': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 880);
                  LedOn(YELLOW);
                  break; 
          case '7': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 988);
                  LedOn(ORANGE);
                  break; 
          case '8': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 392);
                  break; 
          case '0': PWMAudioOff(BUZZER1);
                  LedOff(WHITE);
                  LedOff(PURPLE);
                  LedOff(BLUE);
                  LedOff(CYAN);
                  LedOff(GREEN);
                  LedOff(YELLOW);
                  LedOff(ORANGE);
                  break;  
         default:
                 u32DefaultCounter++;
                 break;
        }
        u8SoundCount++;
      }
      /* the song end and red led on */ 
    if(u16Counter == 9700)
    {
      PWMAudioOff(BUZZER1);
      LedBlink(RED, LED_2HZ);
      LCDClearChars(LINE1_START_ADDR,20);
    }
      /* 15 seconds countdown. Print the song name in ddebug */
    if( (9700 < u16Counter) &&(u16Counter < 24700)  )
    {
      u8time++;
      if(u8time==10)
      {
       u8time=0;
       bButton1CountdownFlag=TRUE;
      }
      else
      {
       bButton1CountdownFlag=FALSE;
      }
      if(bButton1CountdownFlag)
      {
        /* Read the buffer and print the contents */
        u8CharCount = DebugScanf(au8UserInputBuffer);
        au8UserInputBuffer[u8CharCount] = '\0';
        for(u8 j=0;j<u8CharCount;j++)
        {
         LCDMessage (LINE1_START_ADDR+u8CountReally,au8UserInputBuffer);
         u8namebuffer[j]=au8UserInputBuffer[j];
         u8CountReally++;
          if(u8CountReally==20)
          {
            LCDClearChars(LINE1_START_ADDR,20); 
            LCDMessage (LINE1_START_ADDR,au8UserInputBuffer); 
            u8CountReally=0;
          }
 
          /* Compare the input answer to  really answer */
          if( answer1[counter]==au8UserInputBuffer[j])
          {
            counter++;
            /* the input answer is true */
            if(counter==5)
            {
              LCDMessage (LINE2_START_ADDR,bStringTrue);
              LCDClearChars(LINE2_START_ADDR+4,16);
            }
          }
          else
          {
            /* if the input answer is help */
            if(u8stringhelp[helpcount]==au8UserInputBuffer[j])
            {
              helpcount++;
              if(helpcount==4)
              {
                bsendhelp=TRUE;
                helpcount=0;
              }
            }
            /* the in put answer is false */
            else
            {
              LCDMessage (LINE2_START_ADDR,bStringFalse);
              LCDClearChars(LINE2_START_ADDR+5,15);
            }
          }
      } 
        bButton1CountdownFlag=FALSE; 
     }
      
    }
    /* 15 seconds countdown is over */
    if(u16Counter == 24700)
    {
     LedOff(RED);
     bButton1Flag=FALSE;
     u16Counter=480;
     u8SoundCount = 0;
    }
   
  }
  
  /*Press button2 .The third song.Painter */
  if(WasButtonPressed(BUTTON2))
  {
    ButtonAcknowledge(BUTTON2);
    bButton2Flag=TRUE;
    u8CountReally=0;
    counter=0;
    LCDClearChars(LINE2_START_ADDR,20); 
  }
  if(bButton2Flag)
  { 
    char music2[100] ="555333555333555333111110222444333222555550";
    u16Counter++; 
  
    if((u16Counter-500)%100 == 0 && u16Counter < 4701)
    {
        
          switch(music2[u8SoundCount])
        {
          case '1': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 533);
                  LedOn(WHITE);
                  LedOff(BLUE);
                  break;
          case '2': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 587);
                  LedOn(PURPLE);
                  LedOff(BLUE);
                  break;
          case '3': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 659);
                  LedOn(BLUE);
                  LedOff(GREEN);
                  LedOff(CYAN);
                  break;
          case '4': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 698);
                  LedOn(CYAN);
                  LedOff(PURPLE);
                  break;
          case '5': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 784);
                  LedOn(GREEN);
                  LedOff(BLUE);
                  LedOff(PURPLE);
                  break;
          case '6': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 880);
                  LedOn(YELLOW);
                  break; 
          case '7': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 494);
                  LedOn(ORANGE);
                  break; 
          case '8': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 392);
                  break; 
          case '0': PWMAudioOff(BUZZER1);
                  LedOff(WHITE);
                  LedOff(PURPLE);
                  LedOff(BLUE);
                  LedOff(CYAN);
                  LedOff(GREEN);
                  LedOff(YELLOW);
                  LedOff(ORANGE);
                  break; 
          default:
                u32DefaultCounter++;
                break;
        }
        u8SoundCount++;
      }
      /* the song end and red led on */ 
    if(u16Counter == 4700)
    {
      PWMAudioOff(BUZZER1);
      LedBlink(RED, LED_2HZ);
      LCDClearChars(LINE1_START_ADDR,20);
    }
      /* 15 seconds countdown. Print the song name in ddebug */
    if( (4700 < u16Counter) &&(u16Counter < 19700)  )
    {
      u8time++;
      if(u8time==10)
      {
       u8time=0;
       bButton2CountdownFlag=TRUE;
      }
      else
      {
       bButton2CountdownFlag=FALSE;
      }
      if(bButton2CountdownFlag)
      {
        /* Read the buffer and print the contents */
        u8CharCount = DebugScanf(au8UserInputBuffer);
        au8UserInputBuffer[u8CharCount] = '\0';
        for(u8 j=0;j<u8CharCount;j++)
        {
         LCDMessage (LINE1_START_ADDR+u8CountReally,au8UserInputBuffer);
         u8namebuffer[j]=au8UserInputBuffer[j];
         u8CountReally++;
         
          if(u8CountReally>5)
          {
            /* the in put answer is false */
            LCDMessage (LINE2_START_ADDR,bStringFalse);
            LCDClearChars(LINE2_START_ADDR+5,15);

            u8CountReally=0;
          }
 
          /* Compare the input answer to  really answer */
          if(answer2[counter]==au8UserInputBuffer[j])
          {
            counter++;
            /* the input answer is true */
            if(counter==5)
            {
              LCDMessage (LINE2_START_ADDR,bStringTrue);
              LCDClearChars(LINE2_START_ADDR+4,16);
            }
            
          }
         
          /* if the input answer is help */
          if(u8stringhelp[helpcount]==au8UserInputBuffer[j])
          {
            helpcount++;
            if(helpcount==4)
            {
              bsendhelp=TRUE;
              helpcount=0;
            }
          }
  
      } 
        bButton2CountdownFlag=FALSE; 
     }
      
    }
    /* 15 seconds countdown is over */
     if(u16Counter == 19700)
    {
     LedOff(RED);
     bButton2Flag=FALSE;
     u16Counter=480;
     u8SoundCount = 0;
    }
   
  }
  /*Press button3 .The fourth song.Dream Wedding   */
  if(WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    bButton3Flag=TRUE;
    u8CountReally=0;
    counter=0;
    LCDClearChars(LINE2_START_ADDR,20); 
  }
  if(bButton3Flag)
  { 
    char music3[100] = "30813230813230814340814340434450565630030813230813230814340814340434450565630";
    u16Counter++; 
  
    if((u16Counter-500)%300 == 0 && (u16Counter < 23601) )
    {
        
          switch(music3[u8SoundCount])
        {
          case '1': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 1046);
                  LedOn(WHITE);
                  LedOff(YELLOW);
                  break;
          case '2': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 1175);
                  LedOn(PURPLE);
                  LedOff(BLUE);
                  break;
          case '3': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 1318);
                  LedOn(BLUE);
                  LedOff(WHITE);
                  LedOff(PURPLE);
                  LedOff(CYAN);
                  LedOff(YELLOW);
                  break;
          case '4': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 1397);
                  LedOn(CYAN);
                  LedOff(WHITE);
                  LedOff(BLUE);
                  break;
          case '5': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 1568);
                  LedOn(GREEN);
                  LedOff(CYAN);
                  LedOff(YELLOW);
                  break;
          case '6': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 1760);
                  LedOn(YELLOW);
                  LedOff(GREEN);
                  break; 
          case '7': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 1796);
                  LedOn(ORANGE);
                  break; 
                  /* low 6 */
          case '8': PWMAudioOn(BUZZER1);
                  PWMAudioSetFrequency(BUZZER1, 880);
                  LedOn(YELLOW);
                  break; 
          case '0': PWMAudioOff(BUZZER1);
                  LedOff(WHITE);
                  LedOff(PURPLE);
                  LedOff(BLUE);
                  LedOff(CYAN);
                  LedOff(GREEN);
                  LedOff(YELLOW);
                  LedOff(ORANGE);
                  break; 
          default:
                u32DefaultCounter++;
                break;
        }
        u8SoundCount++;
      }
     /* the song end and red led on */
    if(u16Counter == 23600)
    {
      PWMAudioOff(BUZZER1);
      LedBlink(RED, LED_2HZ);
      LCDClearChars(LINE1_START_ADDR,20);
    }
      /* 15 seconds countdown. Print the song name in ddebug */
    if( (23600 < u16Counter) &&(u16Counter < 38600)  )
    {
      u8time++;
      if(u8time==10)
      {
       u8time=0;
       bButton3CountdownFlag=TRUE;
      }
      else
      {
       bButton3CountdownFlag=FALSE;
      }
      if(bButton3CountdownFlag)
      {
        /* Read the buffer and print the contents */
        u8CharCount = DebugScanf(au8UserInputBuffer);
        au8UserInputBuffer[u8CharCount] = '\0';
        for(u8 j=0;j<u8CharCount;j++)
        {
         LCDMessage (LINE1_START_ADDR+u8CountReally,au8UserInputBuffer);
         u8namebuffer[j]=au8UserInputBuffer[j];
         u8CountReally++;
          if(u8CountReally==20)
          {
            LCDClearChars(LINE1_START_ADDR,20); 
            LCDMessage (LINE1_START_ADDR,au8UserInputBuffer); 
            u8CountReally=0;
          }
 
         /* Compare the input answer to  really answer */
          if( answer3[counter]==au8UserInputBuffer[j])
          {
            counter++;
            /* the input answer is true */
            if(counter==5)
            {
              LCDMessage (LINE2_START_ADDR,bStringTrue);
              LCDClearChars(LINE2_START_ADDR+4,16);
            }
          }
          else
          {
            /* if the input answer is help */
            if(u8stringhelp[helpcount]==au8UserInputBuffer[j])
            {
              helpcount++;
              if(helpcount==4)
              {
                bsendhelp=TRUE;
                helpcount=0;
              }
            }
            /* the in put answer is false */
            else
            {
              LCDMessage (LINE2_START_ADDR,bStringFalse);
              LCDClearChars(LINE2_START_ADDR+5,15);
            }
     
          }
      } 
        bButton3CountdownFlag=FALSE; 
     }
      
    }
    /* 15 seconds countdown is over */
     if(u16Counter == 38600)
    {
     LedOff(RED);
     bButton3Flag=FALSE;
     u16Counter=480;
     u8SoundCount = 0;
    } 
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
