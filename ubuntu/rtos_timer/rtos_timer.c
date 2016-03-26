//this is the rtos timer
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>



/*  STATES:
 *    - UNUSED
 *    - STOPPED
 *    - RUNNING
 *    - COMPLETED
 *
 *
 *  METHODS RELATED TO THE EVENTS:
 */

 void OSTmrCreate(){}
 void OSTmrDelete(){}
 void OSTmrStart(){}
 void OSTmrStop(){}



/*  METHODS THAT CAN BE ACCESSED FROM
 *  OTHER TASKS
 */
// find out the state of a timer
void RTOSTmrStateGet() {}
// find out how much time remains before the
// timer reaches zero (0)
void RTOSTmrRemainGet() {}





typedef struct os_tmr {
  unsigned short int RTOSTmrType; /* Should always be set to RTOS_TMR_TYPE for timers */
  // RTOS_TMR_CALLBACK RTOSTmrCallback; /* Function to call when timer expires */
  void (*RTOSTmrCallback)();
  void *RTOSTmrCallbackArg; /* Argument to callback function */
  void *RTOSTmrNext; /* Double link list pointers */
  void *RTOSTmrPrev;
  unsigned long int RTOSTmrMatch; /* Timer expires when RTOSTmrTickCtr = RTOSTmrMatch*/
  unsigned long int RTOSTmrDly; /* Delay time before periodic update starts */
  unsigned long int RTOSTmrPeriod; /* Period to repeat timer */
  unsigned short int *RTOSTmrName; /* Name to give the timer */
  unsigned short int RTOSTmrOpt; /* Options (see RTOS_TMR_OPT_xxx) */
  unsigned short int RTOSTmrState; /* Indicates the state of the timer:*/
  /* RTOS_TMR_STATE_UNUSED */
  /* RTOS_TMR_STATE_RUNNING */
  /* RTOS_TMR_STATE_STOPPED */
} RTOS_TMR;


int main(){return 1;}
