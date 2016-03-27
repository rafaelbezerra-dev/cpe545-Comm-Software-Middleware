#include <unistd.h> // for sleep() and usleep()
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
// #include "DoublyLinkedList.h"
// #include "RTOSTmr_Ticker.h"

void log_m(char msg[]) {
  printf("%s\n", msg);
}

void log_v(char name[], long int value) {
  printf("%s: %li\n", name, value);
}

/*  TIME TICKER
 ********************************************************/
pthread_t thr_ticker;
pthread_t thr_listnr;
pthread_cond_t rtostmr_ticker_cond;
pthread_mutex_t rtostmr_ticker_mutex;

static void *run_rtostmr_ticker(void *args){
  while(1)
  {
    // printf("tick \n");
    pthread_mutex_lock(&rtostmr_ticker_mutex);
    pthread_cond_signal(&rtostmr_ticker_cond);
    pthread_mutex_unlock(&rtostmr_ticker_mutex);

    usleep(1000000);
  }
}

static void *run_tock(void *args){
  while(1)
  {
    pthread_cond_wait(&rtostmr_ticker_cond, &rtostmr_ticker_mutex);
    printf("tock \n");
  }
}

void __init_time_ticker__(){
  log_m("Initializing time ticker...");
  pthread_create(&thr_ticker, NULL, run_rtostmr_ticker, NULL);
}


/********************************************************/

/*    TIMER
 ********************************************************/
typedef void (*RTOS_TMR_CALLBACK)(void *args);
// void MyCallback (RTOS_TMR *ptmr, void *p_arg);
typedef enum {
  RTOS_TMR_STATE_UNUSED,
  RTOS_TMR_STATE_RUNNING,
  RTOS_TMR_STATE_STOPPED,
  RTOS_TMR_STATE_COMPLETED
} RTOS_TMR_STATE;

typedef enum {
  RTOS_TMR_OPT_ONE_SHOT,
  RTOS_TMR_OPT_PERIODIC
} RTOS_TMR_TYPE;

typedef struct os_tmr {
  unsigned short int RTOSTmrType; /* Should always be set to RTOS_TMR_TYPE for timers */
  // void (*RTOSTmrCallback)();
  RTOS_TMR_CALLBACK RTOSTmrCallback; /* Function to call when timer expires */
  void *RTOSTmrCallbackArg; /* Argument to callback function */
  void *RTOSTmrNext; /* Double link list pointers */
  void *RTOSTmrPrev;
  unsigned long int RTOSTmrMatch; /* Timer expires when RTOSTmrTickCtr = RTOSTmrMatch*/
  unsigned long int RTOSTmrDly; /* Delay time before periodic update starts */
  unsigned long int RTOSTmrPeriod; /* Period to repeat timer */
  unsigned short int *RTOSTmrName; /* Name to give the timer */
  unsigned short int RTOSTmrOpt; /* Options (see RTOS_TMR_OPT_xxx) */
  unsigned short int RTOSTmrState; /* Indicates the state of the timer:*/
  RTOS_TMR_STATE state;
  /* RTOS_TMR_STATE_UNUSED */
  /* RTOS_TMR_STATE_RUNNING */
  /* RTOS_TMR_STATE_STOPPED */
} RTOS_TMR;

typedef void (*RTOS_TMR_CALLBACK_PTR) (RTOS_TMR *ptmr, void *p_arg);

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

/********************************************************/

/*  TIMER MANAGER
 ********************************************************/

/* RTOSTmrListEntries contains the current number
 * of entries in the list. This variable is updated
 * whenever timers are added or removed from the list.
 */
unsigned int RTOSTmrListEntries; //aa

/* RTOSTmrListPtr contains a pointer to a doubly
 * linked list of timers that the timer manager will
 * need to update.
 */
RTOS_TMR *RTOSTmrListPtr; // IMPLEMENT double LINKED LIST

/* RTOSTmrTickCtr is incremented by RTOS_TmrTask() every
 * time the tick ISR signals the task. This counter basically
 * keeps track of the number of times the timer task has been
 * signaled.
 */
unsigned int RTOSTmrTickCtr;

RTOS_TMR* RTOSTmrCreate (unsigned long int dly,
                        unsigned long int period,
                        unsigned short int opt,
                        RTOS_TMR_CALLBACK callback,
                        void *callback_arg,
                        unsigned short int *pname,
                        unsigned short int *perr){
  // struct os_tmr* tmr = (struct os_tmr*)malloc(sizeof(struct os_tmr));
  log_m("\n\n### RTOSTmrCreate ###");
  log_v("RTOSTmrCreate::RTOSTmrTickCtr", RTOSTmrTickCtr);
  log_v("RTOSTmrCreate::dly", dly);
  RTOS_TMR tmr = (RTOS_TMR){
    .RTOSTmrDly = dly,
    .RTOSTmrMatch = RTOSTmrTickCtr + dly,
    .RTOSTmrPeriod = period,
    .RTOSTmrType = opt,
    .RTOSTmrCallback = callback,
    .RTOSTmrCallbackArg = callback_arg,
    .RTOSTmrName = pname
  };
  log_v("RTOSTmrCreate::tmr::RTOSTmrMatch", tmr.RTOSTmrMatch);

  RTOS_TMR *ptr = &tmr;

  log_v("RTOSTmrCreate::ptr::RTOSTmrDly", ptr->RTOSTmrDly);
  log_v("RTOSTmrCreate::ptr::RTOSTmrMatch", ptr->RTOSTmrMatch);

  log_m("\n### RTOSTmrCreate (end) ###\n\n");
  return ptr;
};

bool RTOSTmrDel (RTOS_TMR *ptmr, unsigned short int *perr);

unsigned short int RTOSTmrNameGet(RTOS_TMR *ptmr,
                       unsigned short int **pdest,
                       unsigned short int *perr);

/* find out how much time remains before the
 * timer reaches zero (0)
 *  METHODS THAT CAN BE ACCESSED FROM OTHER TASKS
 */
unsigned long int RTOSTmrRemainGet(RTOS_TMR *ptmr,
                        unsigned short int *perr);

/*  find out the state of a timer
 *  METHODS THAT CAN BE ACCESSED FROM OTHER TASKS
 */
RTOS_TMR_STATE RTOSTmrStateGet (RTOS_TMR *ptmr,
                          unsigned short int *perr){
  return RTOSTmrListPtr->state;
}

bool RTOSTmrStart (RTOS_TMR *ptmr, unsigned short int *perr) {
  if (RTOSTmrListPtr == NULL) { // pointer is null
    RTOSTmrListPtr = ptmr;
    RTOSTmrListEntries = 1;
  }
  else { // pointer is not null
    RTOS_TMR *temp = RTOSTmrListPtr;

    while (ptmr->RTOSTmrDly > temp->RTOSTmrDly) {
      ptmr->RTOSTmrDly -= temp->RTOSTmrDly;
      temp = RTOSTmrListPtr->RTOSTmrNext;
    }

    RTOSTmrListPtr->RTOSTmrPrev = ptmr;
    ptmr->RTOSTmrNext = RTOSTmrListPtr;
    RTOSTmrListPtr = ptmr;

    RTOSTmrListEntries++;
  }
  return true;
}

bool RTOSTmrStop (RTOS_TMR *ptmr,
                  unsigned short int opt,
                  void *callback_arg,
                  unsigned short int *perr);

unsigned short int RTOSTmrSignal (void);

pthread_t thr_timer_manager;

static void *run_timer_manager(void *args){
  while(1)
  {
    pthread_cond_wait(&rtostmr_ticker_cond, &rtostmr_ticker_mutex);
    RTOSTmrTickCtr++;
    log_m("Comparing Tick Counter to List head:");
    log_v("RTOSTmrTickCtr", RTOSTmrTickCtr);
    log_v("RTOSTmrListPtr->RTOSTmrMatch", RTOSTmrListPtr->RTOSTmrMatch);

    if (RTOSTmrListPtr->RTOSTmrMatch == RTOSTmrTickCtr){
      log_m("IT'S A MATCH!!!");
    }
  }
}

void __init_timer_manager__() {
  log_m("Initializing timer...");
  RTOSTmrTickCtr = 0;
  pthread_create(&thr_timer_manager, NULL, run_timer_manager, NULL);

}
/********************************************************/

typedef struct foo_struct {
  int bar;
  bool active;
} FOO;

void MyTmrCallbackFnct1 (void *p_arg) {
/* Do something when timer #1 expires */
}


int main(){
  log_m("Initializing main...");

  __init_time_ticker__();
  __init_timer_manager__();

  unsigned short int pname = 1;
  unsigned short int perr = 1;
  RTOS_TMR *tmr = RTOSTmrCreate(10,
    0,
    0,
    (RTOS_TMR_CALLBACK) MyTmrCallbackFnct1,
    (void *) 1234567,
    (unsigned short int*) &pname,
    (unsigned short int*) &perr);

  log_v("RTOSTmrDly", tmr->RTOSTmrDly);
  log_v("RTOSTmrMatch", tmr->RTOSTmrMatch);

  RTOSTmrStart(tmr, (unsigned short int*) &perr);


  // printf("%li\n", tmr->RTOSTmrDly);
  // printf("%li\n", tmr->RTOSTmrMatch);
  // pthread_create(&thr_listnr, NULL, run_tock, NULL);
  // pthread_join(thr_listnr, NULL);


  sleep(5000);
  // free(foo);
  return 1;
}
