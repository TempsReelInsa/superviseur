/*
 * File:   global.h
 * Author: pehladik
 *
 * Created on 21 avril 2011, 12:14
 */

#include "global.h"

RT_SEM semConnecterRobot;
RT_SEM semLaunchWatchdog;

int etatCommRobot = 1;
DRobot *robot;
DMovement *move;
DServer *serveur;
DBattery *battery;
DCamera *camera;
