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
extern RT_SEM semDetectArena;
extern RT_SEM semMonitorStatusOk;
extern RT_SEM semMissionOk;

/* @variables partagées */
extern DServer *serveur;
extern DRobot *robot;
extern DMovement *move;
extern DBattery *battery;
extern DCamera *camera;
extern DArena *arena;
extern DMission *mission;

#endif	/* GLOBAL_H */

