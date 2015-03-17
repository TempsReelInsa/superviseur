/* 
 * File:   global.h
 * Author: pehladik
 *
 * Created on 12 janvier 2012, 10:11
 */

#ifndef GLOBAL_H
#define	GLOBAL_H

#include "includes.h"

/* @descripteurs des tâches */
extern RT_TASK task_thread_send_monitor;
extern RT_TASK task_thread_connect_robot;
extern RT_TASK task_thread_recv_monitor;
extern RT_TASK task_thread_move_robot;
extern RT_TASK task_thread_battery_state;
extern RT_TASK task_thread_image;

/* @descripteurs des mutex */
extern RT_MUTEX mutexEtat;
extern RT_MUTEX mutexMove;
extern RT_MUTEX mutexBattery;

/* @descripteurs des sempahore */
extern RT_SEM semConnecterRobot;

/* @descripteurs des files de messages */
extern RT_QUEUE queueMsgGUI;

/* @variables partagées */
extern int etatCommMoniteur;
extern int etatCommRobot;
extern DServer *serveur;
extern DRobot *robot;
extern DMovement *move;
extern DBattery *battery;
extern DCamera *camera;

/* @constantes */
extern int MSG_QUEUE_SIZE;
extern int PRIORITY_RECV_MONITOR;
extern int PRIORITY_CONNECT_ROBOT;
extern int PRIORITY_MOVE_ROBOT;
extern int PRIORITY_SEND_MONITOR;
extern int PRIORITY_BATTERY_STATE; 
extern int PRIORITY_IMAGE;


#endif	/* GLOBAL_H */

