#ifndef MONITOR_H
#define MONITOR_H

#include "global.h"
#include "includes.h"

#define IMAGE_STATUS_NO 0
#define IMAGE_STATUS_TAKE_SIMPLE 1

#define IMAGE_FIND_ARENA_NO 0
#define IMAGE_FIND_ARENA 1
#define IMAGE_FIND_ARENA_WAIT 2
#define IMAGE_FIND_ARENA_FAILED 3
#define IMAGE_FIND_ARENA_IS_FOUND 4

int image_status_set(int status);
void image_status_wait_for(int status);

int image_get_detect_area();
void image_set_detect_area(int ret);
void image_no_wait_detect_area();

#endif