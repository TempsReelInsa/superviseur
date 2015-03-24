#ifndef THREADS_H
#define	THREADS_H

#include "global.h"
#include "includes.h"

void threads_init();
void threads_start();
void threads_stop();

void thread_send_monitor(void *arg);
void thread_connect_robot(void * arg);
void thread_recv_monitor(void *arg);
void thread_move_robot(void *arg);
void thread_battery_state(void * args);
void thread_image_normal(void *args);
void thread_image_compute(void *args);
void thread_watchdog(void * args);
void thread_mission(void * args);
void print_status(int status);

#endif

