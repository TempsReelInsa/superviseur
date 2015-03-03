/*
 * File:   global.h
 * Author: pehladik
 *
 * Created on 21 avril 2011, 12:14
 */

#include "global.h"

RT_TASK tServeur;
RT_TASK tconnect;
RT_TASK tmove;
RT_TASK tenvoyer;
RT_TASK tbatterie_state;
RT_TASK tImages;

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

int PRIORITY_TSERVEUR = 30;
int PRIORITY_TCONNECT = 20;
int PRIORITY_TMOVE = 10;
int PRIORITY_TENVOYER = 25;
int PRIORITY_TBATTERIE = 25; 
int PRIORITY_IMAGES = 25;
