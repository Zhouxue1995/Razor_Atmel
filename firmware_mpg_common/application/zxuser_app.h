/**********************************************************************************************************************
File: zx_user_app.h                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this zx_user_app as a template:
1. Follow the instructions at the top of zx_user_app.c
2. Use ctrl-h to find and replace all instances of "zx_user_app" with "yournewtaskname"
3. Use ctrl-h to find and replace all instances of "ZxUserApp" with "YourNewTaskName"
4. Use ctrl-h to find and replace all instances of "ZX_USER_APP" with "YOUR_NEW_TASK_NAME"
5. Add #include yournewtaskname.h" to configuration.h
6. Add/update any special configurations required in configuration.h (e.g. peripheral assignment and setup values)
7. Delete this text (between the dashed lines)
----------------------------------------------------------------------------------------------------------------------

Description:
Header file for yournewtaskname.c

**********************************************************************************************************************/

#ifndef __ZX_USER_APP_H
#define __ZX_USER_APP_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
#define USER_INPUT_BUFFER_SIZE  (u16)(DEBUG_SCANF_BUFFER_SIZE + 1)   


/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void ZxUserAppInitialize(void);
void ZxUserAppRunActiveState(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void ZxUserAppSM_Idle(void);    

static void ZxUserAppSM_Error(void);         
static void ZxUserAppSM_FailedInit(void);        

#endif /* __ZX_USER_APP_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
