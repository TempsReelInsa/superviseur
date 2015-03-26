#include "mutex.h"

RT_MUTEX mutexEtat;
RT_MUTEX mutexMove;
RT_MUTEX mutexBattery;
RT_MUTEX mutexImageStatus;
RT_MUTEX mutexArena;

void mutex_init(){
    int err;
    /* Creation des mutex */
    if ((err = rt_mutex_create(&mutexEtat, NULL))) {
        rt_printf("Error mutex create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }
    if ((err = rt_mutex_create(&mutexMove, NULL))) {
        rt_printf("Error mutex create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }
    if ((err = rt_mutex_create(&mutexBattery, NULL))) {
        rt_printf("Error mutex create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }
    if ((err = rt_mutex_create(&mutexImageStatus, NULL))) {
        rt_printf("Error mutex create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }
    if ((err = rt_mutex_create(&mutexArena, NULL))) {
        rt_printf("Error mutex create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }
}

void mutex_acquire(RT_MUTEX mutex){
	if(rt_mutex_acquire(&mutex, TM_INFINITE)!=0){
		perror("rt_mutex_acquire");
		exit(EXIT_FAILURE);
	}
}

void mutex_release(RT_MUTEX mutex){
	if(rt_mutex_release(&mutex)!=0){
		perror("rt_mutex_release");
		exit(EXIT_FAILURE);
	}
}

void mutex_robot_acquire(){
	mutex_acquire(mutexMove);
}

void mutex_robot_release(){
	mutex_release(mutexMove);
}

void mutex_state_acquire(){
	mutex_acquire(mutexEtat);
}

void mutex_state_release(){
	mutex_release(mutexEtat);
}

void mutex_battery_acquire(){
	mutex_acquire(mutexBattery);
}

void mutex_battery_release(){
	mutex_release(mutexBattery);
}

void mutex_image_status_acquire(){
	mutex_acquire(mutexImageStatus);
}

void mutex_image_status_release(){
	mutex_release(mutexImageStatus);
}

void mutex_arena_acquire(){
    mutex_acquire(mutexArena);
}

void mutex_arena_release(){
    mutex_release(mutexArena);
}