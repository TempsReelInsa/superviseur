/* 
 * File:   global.h
 * Author: pehladik
 *
 * Created on 12 janvier 2012, 10:11
 */

#ifndef GLOBAL_H
#define	GLOBAL_H

#include "includes.h"

/* @descripteurs des sempahore */
extern RT_SEM semConnecterRobot;
extern RT_SEM semLaunchWatchdog;
extern RT_SEM semStatusOk;
extern RT_SEM semMonitorStatusUpdated;

/* @variables partagées */
extern DServer *serveur;
extern DRobot *robot;
extern DMovement *move;
extern DBattery *battery;
extern DCamera *camera;

#endif	/* GLOBAL_H */

