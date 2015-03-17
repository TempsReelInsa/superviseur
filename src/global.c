/*
 * File:   global.h
 * Author: pehladik
 *
 * Created on 21 avril 2011, 12:14
 */

#include "global.h"

RT_TASK task_thread_batterie_state;
RT_TASK task_thread_connect_robot;
RT_TASK task_thread_recv_monitor;
RT_TASK task_thread_send_monitor;
RT_TASK task_thread_move_robot;
RT_TASK task_thread_battery_state;
RT_TASK task_thread_image;

RT_MUTEX mutexEtat;
RT_MUTEX mutexMove;
RT_MUTEX mutexBattery;

RT_SEM semConnecterRobot;

RT_QUEUE queueMsgGUI;

int etatCommMoniteur = 0;
int etatCommRobot = 1;
DRobot *robot;
DMovement *move;
DServer *serveur;
DBattery *battery;
DCamera *camera;

int MSG_QUEUE_SIZE = 10;
