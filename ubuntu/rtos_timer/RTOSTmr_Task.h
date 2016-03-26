#ifndef RTOSTMRTASK_H_INCLUDED

#define RTOSTMRTASK_H_INCLUDED

typedef void (*rtos_timer_task_callback)();
// typedef struct rtos_timer_task {
//   int id ;
//   rtos_timer_task_callback callback;
// } RTOS_TMR_TASK;

struct rtos_timer_task {
  int id ;
  rtos_timer_task_callback callback;
};


void __init_rtostmr_task__(struct rtos_timer_task *callback);

#endif /* RTOSTMRTASK_H_INCLUDED */
