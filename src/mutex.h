#ifndef MUTEX_H
#define	MUTEX_H

#include "includes.h"
#include <errno.h>

void mutex_init();

void mutex_robot_acquire();
void mutex_robot_release();

void mutex_state_acquire();
void mutex_state_release();

void mutex_battery_acquire();
void mutex_battery_release();

void mutex_image_status_acquire();
void mutex_image_status_release();

void mutex_arena_acquire();
void mutex_arena_release();

void mutex_mission_acquire();
void mutex_mission_release();

void mutex_position_acquire();
void mutex_position_release();

#endif	/* MUTEX_H */