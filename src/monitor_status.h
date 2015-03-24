#ifndef MONITOR_H
#define MONITOR_H

#include "global.h"
#include "includes.h"

#define MONITOR_STATUS_OK 1
#define MONITOR_STATUS_NOT_OK 0

void monitor_status_set(int v);
int monitor_status_get();
int monitor_status_check();

#endif