#define _BSD_SOURCE

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for sleep() and usleep()
#include "RTOSTmr_Ticker.h"

pthread_cond_t rtostmr_ticker_cond;
pthread_mutex_t rtostmr_ticker_mutex;


static void *run(void *args){
  while(1)
  {
    printf("tick \n");
    pthread_mutex_lock(&rtostmr_ticker_mutex);
    pthread_cond_signal(&rtostmr_ticker_cond);
    pthread_mutex_unlock(&rtostmr_ticker_mutex);

    usleep(1000000);
  }
}

void __init_rtostmr_ticker__(){
  rtostmr_ticker_cond = PTHREAD_COND_INITIALIZER;
  rtostmr_ticker_mutex = PTHREAD_MUTEX_INITIALIZER;
}

void __start_rtostmr_ticker__(){
  pthread_t thr1;
  pthread_create(&thr1, NULL, run, NULL);
  pthread_join(thr1, NULL);
}

// void __sync_cond_sem__(pthread_cond_t &cond, pthread_mutex_t &mutex){
void __sync_cond_sem__(pthread_cond_t *cond, pthread_mutex_t *mutex){
  (cond) = &rtostmr_ticker_cond;
  (mutex) =  &rtostmr_ticker_mutex;
}
