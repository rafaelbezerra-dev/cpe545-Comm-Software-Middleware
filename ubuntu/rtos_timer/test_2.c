#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for sleep() and usleep()
#include <string.h>

typedef int (*_callback)();

typedef struct my_callback_struct {
  int id ;
  _callback cllbck;
} MY_CALLBACK;

int say_hello(){
  printf("Hello\n");
  return 0;
}

MY_CALLBACK __init_callback__(int id, _callback cllbck){
  // MY_CALLBACK *c = malloc(sizeof(MY_CALLBACK));
  MY_CALLBACK c = {.id = id, .cllbck = cllbck};
  return c;
}

int main() {
  MY_CALLBACK c = __init_callback__(2, say_hello);
  c.cllbck();
  return 0;
}
