#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


// RTOSTmrListEntries contains the current number
// of entries in the list. This variable is updated
// whenever timers are added or removed from the list.
RTOSTmrListEntries

// RTOSTmrListPtr contains a pointer to a doubly
// linked list of timers that the timer manager will
// need to update.
RTOSTmrListPtr // IMPLEMENT double LINKED LIST

// RTOSTmrTickCtr is incremented by RTOS_TmrTask() every
// time the tick ISR signals the task. This counter basically
// keeps track of the number of times the timer task has been
// signaled.
RTOSTmrTickCtr

RTOS_TMR *RTOSTmrCreate (INT32U dly,
                          INT32U period,
                          INT8U opt,
                          RTOS_TMR_CALLBACK callback,
                          void *callback_arg,
                          INT8U *pname,
                          INT8U *perr)

bool RTOSTmrDel (RTOS_TMR *ptmr,
                    INT8U *perr)

INT8U RTOSTmrNameGet (RTOS_TMR *ptmr,
                      INT8U **pdest,
                      INT8U *perr)

                      INT32U RTOSTmrRemainGet (RTOS_TMR *ptmr,
INT8U *perr)

INT8U RTOSTmrStateGet (RTOS_TMR *ptmr,
INT8U *perr)

BOOLEAN RTOSTmrStart (RTOS_TMR *ptmr,
INT8U *perr)

BOOLEAN RTOSTmrStop (RTOS_TMR *ptmr,
INT8U opt,
void *callback_arg,
INT8U *perr)

INT8U RTOSTmrSignal (void)
