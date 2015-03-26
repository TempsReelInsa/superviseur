/*
 * File:   global.h
 * Author: pehladik
 *
 * Created on 21 avril 2011, 12:14
 */

#include "global.h"

RT_SEM semConnecterRobot;
RT_SEM semLaunchWatchdog;
RT_SEM semStatusOk;	
RT_SEM semMonitorStatusUpdated;
RT_SEM semDetectArena;
RT_SEM semMissionOk;

DRobot *robot;
DMovement *move;
DServer *serveur;
DBattery *battery;
DCamera *camera;
DArena * arena;
DMission *mission;
DPosition *position;