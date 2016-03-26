#define _BSD_SOURCE

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for sleep() and usleep()
#include "RTOSTmr_Task.h"


static void *run(void *args){
  printf("Initalizing\n");
  int id = ((struct rtos_timer_task *) args)->id;

  printf("%d\n", id);
  while(1)
  {
    // counter++;
    // printf("Counter on thread says: %d \n", counter);
    // if (task.callback != NULL){
    //   task.callback();
    // }
    // else{
      printf("No-Callback \n");
    // }

    usleep(1000000);
  }
}

void __init_rtostmr_task__(struct rtos_timer_task *callback){
  pthread_t thr1;
  pthread_create(&thr1, NULL, run, (void *) callback);
  // pthread_join(thr1, NULL);
}
