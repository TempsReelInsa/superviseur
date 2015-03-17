#ifndef MUTEX_H
#define	MUTEX_H

#include "includes.h"
#include <errno.h>

void mutex_robot_acquire();
void mutex_robot_release();

void mutex_state_acquire();
void mutex_state_release();

void mutex_battery_acquire();
void mutex_battery_release();
#endif	/* MUTEX_H */