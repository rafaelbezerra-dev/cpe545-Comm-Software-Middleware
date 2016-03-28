/*	CODED BY: RAFAEL NASCIMENTO BEZERRA	*/

  #include <unistd.h> // for sleep() and usleep()
  #include <stdlib.h>
  #include <stdio.h>
  #include <pthread.h>
  #include <stdbool.h>
  #include <time.h>
  // #include "DoublyLinkedList.h"
  // #include "RTOSTmr_Ticker.h"

  #define RTOS_CFG_TMR_TASK_RATE_HZ 10

  void log_msg(char msg[]) {
    printf("%s\n", msg);
  }

  void log_str(char name[], char value[]) {
    printf("%s: %s\n", name, value);
  }

  void log_int(char name[], long int value) {
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

      usleep(1000000/RTOS_CFG_TMR_TASK_RATE_HZ);
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
    log_msg("Initializing time ticker...");
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
    // log_msg("\n\n### RTOSTmrCreate ###");
    // log_int("RTOSTmrCreate::RTOSTmrTickCtr", RTOSTmrTickCtr);
    // log_int("RTOSTmrCreate::*pname", *pname);
    // log_int("RTOSTmrCreate::dly", dly);
    RTOS_TMR* tmr = (RTOS_TMR*)malloc(sizeof(RTOS_TMR));
    tmr->RTOSTmrDly = dly;
    tmr->RTOSTmrMatch = 0;
    tmr->RTOSTmrPeriod = period;
    tmr->RTOSTmrOpt = opt;
    tmr->RTOSTmrCallback = callback;
    tmr->RTOSTmrCallbackArg = callback_arg;
    tmr->RTOSTmrName = pname;
    tmr->RTOSTmrNext = NULL;
    tmr->RTOSTmrPrev = NULL;
    tmr->RTOSTmrState = RTOS_TMR_STATE_STOPPED;

    // RTOS_TMR *ptr = &tmr;

    // log_int("RTOSTmrCreate::tmr::RTOSTmrName", tmr->RTOSTmrName);
    // log_int("RTOSTmrCreate::tmr::RTOSTmrDly", tmr->RTOSTmrDly);
    // log_int("RTOSTmrCreate::tmr::RTOSTmrMatch", tmr->RTOSTmrMatch);
    //
    // log_msg("\n### RTOSTmrCreate (end) ###\n\n");
    log_int("Timer", (long int) (*tmr->RTOSTmrName));
    time_t ltime=time(NULL);
    log_str("Created at",asctime(localtime(&ltime)));
    return tmr;
  };

  bool RTOSTmrDel (RTOS_TMR *ptmr, unsigned short int *perr){
    // log_int("Deleting timer", (long int) (*ptmr->RTOSTmrName));

    ptmr->RTOSTmrState = RTOS_TMR_STATE_UNUSED;

    if(ptmr->RTOSTmrPrev != NULL){
      ((RTOS_TMR*)ptmr->RTOSTmrPrev)->RTOSTmrNext = ptmr->RTOSTmrNext;
    }

    if (ptmr->RTOSTmrNext != NULL) {
      ((RTOS_TMR*)ptmr->RTOSTmrNext)->RTOSTmrPrev = ptmr->RTOSTmrPrev;
    }

    // free(ptmr->RTOSTmrCallback);
    // log_msg("ptmr->RTOSTmrCallback free");
    // free(ptmr->RTOSTmrCallbackArg);
    // log_msg("ptmr->RTOSTmrCallbackArg free");
    // free(ptmr->RTOSTmrName);
    // log_msg("ptmr->RTOSTmrName free");
    // free(ptmr);
    // log_msg("ptmr free");

    RTOSTmrListEntries--;
    return true;
  }

  unsigned short int RTOSTmrNameGet(RTOS_TMR *ptmr,
                         unsigned short int **pdest,
                         unsigned short int *perr);

  /* find out how much time remains before the
   * timer reaches zero (0)
   *  METHODS THAT CAN BE ACCESSED FROM OTHER TASKS
   */
  unsigned long int RTOSTmrRemainGet(RTOS_TMR *ptmr,
                          unsigned short int *perr){
    return ptmr->RTOSTmrMatch - RTOSTmrTickCtr;
  }

  /*  find out the state of a timer
   *  METHODS THAT CAN BE ACCESSED FROM OTHER TASKS
   */
  RTOS_TMR_STATE RTOSTmrStateGet (RTOS_TMR *ptmr,
                            unsigned short int *perr){
    return ptmr->RTOSTmrState;
  }

  bool RTOSTmrStart (RTOS_TMR *ptmr, unsigned short int *perr) {
      log_int("Starting timer", (long int) (*ptmr->RTOSTmrName));
      ptmr->RTOSTmrMatch = RTOSTmrTickCtr + ptmr->RTOSTmrDly;
      ptmr->RTOSTmrState = RTOS_TMR_STATE_RUNNING;

      if (RTOSTmrListPtr == NULL) { // pointer is null
        RTOSTmrListPtr = ptmr;
        RTOSTmrListEntries = 1;
        log_msg("The list was empty. Setting timer as head...");
        log_msg("");
      }
      else { // pointer is not null
        int pos = 1;
        RTOS_TMR *temp = RTOSTmrListPtr;

        while (temp->RTOSTmrNext != NULL && ptmr->RTOSTmrMatch >= temp->RTOSTmrMatch) {
            temp = temp->RTOSTmrNext;
            pos++;
        }

        ptmr->RTOSTmrNext = temp->RTOSTmrNext;
        if (ptmr->RTOSTmrNext != NULL){
          ((RTOS_TMR *)ptmr->RTOSTmrNext)->RTOSTmrPrev = ptmr;
        }
        ptmr->RTOSTmrPrev = temp;
        temp->RTOSTmrNext = ptmr;

        RTOSTmrListEntries++;
        log_int("Timer added to position", (long int) pos);
        log_msg("");
      }
      return true;
    }


  bool RTOSTmrReload (RTOS_TMR *ptmr, unsigned short int *perr) {
    log_int("Reloading timer", (long int) (*ptmr->RTOSTmrName));
    ptmr->RTOSTmrMatch = RTOSTmrTickCtr + ptmr->RTOSTmrPeriod;
    // log_int("Next run in", RTOSTmrRemainGet(ptmr, NULL));
    RTOS_TMR *temp = ptmr;
    if (ptmr->RTOSTmrNext == NULL) {
      log_msg("ptmr->RTOSTmrNext == NULL");
      return false;
    }

    int pos = 0;
    while (ptmr->RTOSTmrMatch >= ((RTOS_TMR *)ptmr->RTOSTmrNext)->RTOSTmrMatch) {
      ptmr = ptmr->RTOSTmrNext;
      pos++;
    }

    // log_int("Timer will be moved n positions", pos);

    if(temp == ptmr){
      log_msg("temp == ptmr");
      return false;
    }

    if(temp->RTOSTmrPrev != NULL){
      ((RTOS_TMR*)temp->RTOSTmrPrev)->RTOSTmrNext = temp->RTOSTmrNext;
    }

    if (temp->RTOSTmrNext != NULL) {
      ((RTOS_TMR*)temp->RTOSTmrNext)->RTOSTmrPrev = temp->RTOSTmrPrev;
    }

    temp->RTOSTmrNext = ptmr->RTOSTmrNext;
    if (temp->RTOSTmrNext != NULL){
      ((RTOS_TMR *)temp->RTOSTmrNext)->RTOSTmrPrev = temp;
    }
    temp->RTOSTmrPrev = ptmr;
    ptmr->RTOSTmrNext = temp;

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

      if (RTOSTmrListPtr != NULL){
        // log_msg("Comparing Tick Counter to List head:");
        // log_int("RTOSTmrTickCtr", RTOSTmrTickCtr);
        // log_int("RTOSTmrListPtr->RTOSTmrMatch", RTOSTmrListPtr->RTOSTmrMatch);

        // RTOS_TMR *head = RTOSTmrListPtr;

        while(RTOSTmrListPtr != NULL &&
          RTOSTmrListPtr->RTOSTmrMatch == RTOSTmrTickCtr)
        {
          // log_msg("IT'S A MATCH!!!");
          RTOSTmrListPtr->RTOSTmrCallback(RTOSTmrListPtr->RTOSTmrCallbackArg);

          RTOS_TMR *temp = RTOSTmrListPtr;
          unsigned short int perr = 1;

          switch(temp->RTOSTmrOpt) {
            case RTOS_TMR_OPT_PERIODIC:
              if (RTOSTmrReload(temp, (unsigned short int*) &perr)) {
                RTOSTmrListPtr = RTOSTmrListPtr->RTOSTmrNext;
              }
              break;

            case RTOS_TMR_OPT_ONE_SHOT:
              RTOSTmrListPtr = RTOSTmrListPtr->RTOSTmrNext;
              temp->RTOSTmrState = RTOS_TMR_STATE_COMPLETED;
              RTOSTmrDel(temp, (unsigned short int*) &perr);
              break;

            default : continue;
          }
        }

      }
      else{
        log_msg("The List is empty.");
      }
    }
  }

  void __init_timer_manager__() {
    log_msg("Initializing timer...");
    RTOSTmrListPtr = NULL;
    RTOSTmrTickCtr = 0;
    pthread_create(&thr_timer_manager, NULL, run_timer_manager, NULL);

  }
  /********************************************************/

  void MyTmrCallbackFnct1 (void *p_arg) {
    time_t ltime;
    ltime=time(NULL);
    log_str("this is function 1 and time is",asctime(localtime(&ltime)));
  }

  void MyTmrCallbackFnct2 (void *p_arg) {
    time_t ltime;
    ltime=time(NULL);
    log_str("this is function 2 and time is",asctime(localtime(&ltime)));
  }


  void MyTmrCallbackFnct3 (void *p_arg) {
    time_t ltime;
    ltime=time(NULL);
    log_str("this is function 3 and time is",asctime(localtime(&ltime)));
  }


  int main(){
    log_msg("Initializing main...");

    __init_time_ticker__();
    __init_timer_manager__();


    unsigned short int name_value = 1;
    unsigned short int* pname = &name_value;
    unsigned short int perr = 1;
    RTOS_TMR *tmr_1 = RTOSTmrCreate(RTOS_CFG_TMR_TASK_RATE_HZ * 5,
      5,
      RTOS_TMR_OPT_PERIODIC,
      (RTOS_TMR_CALLBACK) MyTmrCallbackFnct1,
      (void *) 1234567,
      pname,
      (unsigned short int*) &perr);
    RTOSTmrStart(tmr_1, (unsigned short int*) &perr);

    unsigned short int name_value_2 = 2;
    unsigned short int* pname_2 = &name_value_2;
    RTOS_TMR *tmr_2 = RTOSTmrCreate(RTOS_CFG_TMR_TASK_RATE_HZ * 3,
      3,
      RTOS_TMR_OPT_PERIODIC,
      (RTOS_TMR_CALLBACK) MyTmrCallbackFnct2,
      (void *) 1234567,
      pname_2,
      (unsigned short int*) &perr);
    RTOSTmrStart(tmr_2, (unsigned short int*) &perr);

    unsigned short int name_value_3 = 3;
    unsigned short int* pname_3 = &name_value_3;
    RTOS_TMR *tmr_3 = RTOSTmrCreate(RTOS_CFG_TMR_TASK_RATE_HZ * 10,
      0,
      RTOS_TMR_OPT_ONE_SHOT,
      (RTOS_TMR_CALLBACK) MyTmrCallbackFnct3,
      (void *) 1334567,
      pname_3,
      (unsigned short int*) &perr);
    RTOSTmrStart(tmr_3, (unsigned short int*) &perr);


    sleep(5000);
    // printf("%li\n", tmr->RTOSTmrDly);
    // printf("%li\n", tmr->RTOSTmrMatch);
    // pthread_create(&thr_listnr, NULL, run_tock, NULL);
    // pthread_join(thr_listnr, NULL);


    // free(foo);
    return 1;
  }
