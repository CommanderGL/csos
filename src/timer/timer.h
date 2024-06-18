#ifndef TIMER_H
#define TIMER_H

typedef struct {
  int interval; // In Milliseconds
  void (*handler)();
} interval_handler;

void timer_install(void);
int add_interval_handler(double interval, void (*handler)());

#endif
