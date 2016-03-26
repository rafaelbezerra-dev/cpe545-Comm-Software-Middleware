#ifndef RTOSTMRTICKER_H_INCLUDED

#define RTOSTMRTICKER_H_INCLUDED

void __init_rtostmr_ticker__();
void __start_rtostmr_ticker__();
void __sync_cond_sem__(pthread_cond_t *cond, pthread_mutex_t *mutex);

// void __sync_cond_sem__(pthread_cond_t *cond, pthread_mutex_t *mutex);

#endif /* RTOSTMRTICKER_H_INCLUDED */
