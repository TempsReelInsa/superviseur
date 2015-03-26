#ifndef UTILS_H
#define UTILS_H

#include "global.h"
#include "includes.h"

void restart_robot();
void restart_all();

DImage *get_image();
DJpegimage *get_jpeg_from_image(DImage *i);
DJpegimage *get_jpeg();

#endif