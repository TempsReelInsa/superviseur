#ifndef THREADS_H
#define	THREADS_H

#include "global.h"
#include "includes.h"

void thread_send_monitor(void *arg);
void thread_connect_robot(void * arg);
void thread_recv_monitor(void *arg);
void thread_move_robot(void *arg);
void thread_battery_state(void * args);
void thread_image(void *args);
void print_status(int status);
void images(void *args);

#endif

