#include "threads.h"

#define PRIORITY_RECV_MONITOR 30
#define PRIORITY_CONNECT_ROBOT 20
#define PRIORITY_MOVE_ROBOT 10
#define PRIORITY_SEND_MONITOR 25
#define PRIORITY_BATTERY_STATE 25
#define PRIORITY_IMAGE 25

RT_TASK *threads_tasks_tab[] = {
    &task_thread_send_monitor,
    &task_thread_connect_robot,
    &task_thread_recv_monitor,
    &task_thread_move_robot,
    &task_thread_battery_state,
    &task_thread_image,
    NULL
};

void (*threads_functions_tab[])(void *) = {
    &thread_send_monitor,
    &thread_connect_robot,
    &thread_recv_monitor,
    &thread_move_robot,
    &thread_battery_state,
    &thread_image,
    NULL
};

int threads_priority[] = {
    PRIORITY_SEND_MONITOR,
    PRIORITY_CONNECT_ROBOT,
    PRIORITY_RECV_MONITOR,
    PRIORITY_MOVE_ROBOT,
    PRIORITY_BATTERY_STATE,
    PRIORITY_IMAGE,
    -1
};

void threads_init()
{
    int err;
    int i;

    for(i=0; threads_tasks_tab[i] != NULL; i++)
    {
        if((err = rt_task_create(threads_tasks_tab[i], NULL, 0, threads_priority[i], 0)))
        {
            rt_printf("Error task create: %s\n", strerror(-err));
            exit(EXIT_FAILURE);
        } else {
            rt_printf("Task %d created ! \n", i);
        }
    }
 
}

void threads_start()
{
    int err;
    int i;

    for(i=0; threads_tasks_tab[i] != NULL; i++)
    {
        if((err = rt_task_start(threads_tasks_tab[i], threads_functions_tab[i], NULL)))
        {
            rt_printf("Error task start: %s\n", strerror(-err));
            exit(EXIT_FAILURE);
        } else {
            rt_printf("Task %d started !\n", i);
        }
    }
}

void threads_stop()
{
    int i;
    for(i=0; threads_tasks_tab[i] != NULL; i++)
    {
        rt_task_delete(threads_tasks_tab[i]);
        rt_printf("Task %d stopped\n", i);
    }
}

int write_in_queue(RT_QUEUE *msgQueue, void * data, int size);

void thread_send_monitor(void * arg) {
    DMessage *msg;
    int err;

    while (1) {
        rt_printf("tenvoyer : Attente d'un message\n");
        if ((err = rt_queue_read(&queueMsgGUI, &msg, sizeof (DMessage), TM_INFINITE)) >= 0) {
            rt_printf("tenvoyer : envoi d'un message au moniteur\n");
            serveur->send(serveur, msg);
            msg->free(msg);
            rt_queue_free(&queueMsgGUI, msg);
        } else {
            rt_printf("Error msg queue write: %s\n", strerror(-err));
        }
    }
}

void thread_connect_robot(void * arg) {
    int status;
    int version_min,version_max;
    DMessage *message;

    rt_printf("tconnect : Debut de l'exécution de tconnect\n");

    while (1) {
        rt_printf("tconnect : Attente du sémarphore semConnecterRobot\n");
        rt_sem_p(&semConnecterRobot, TM_INFINITE);
        rt_printf("tconnect : Ouverture de la communication avec le robot\n");
        status = robot->open_device(robot);

        rt_mutex_acquire(&mutexEtat, TM_INFINITE);
        etatCommRobot = status;
        rt_mutex_release(&mutexEtat);

        if (status == STATUS_OK) {
            status = robot->start_insecurely(robot);
            if (status == STATUS_OK){
                rt_printf("********>tconnect : Robot démarrer<**********\n");

                rt_mutex_acquire(&mutexMove, TM_INFINITE);
                robot->get_version(robot, &version_max, &version_min);
                rt_mutex_release(&mutexMove);

                message = d_new_message();
                message->put_version(message,version_max,version_min);
                
                if (write_in_queue(&queueMsgGUI, message, sizeof (DMessage)) < 0) {
                    message->free(message);
                }
            }
        }

        message = d_new_message();
        message->put_state(message, status);

        rt_printf("tconnecter : Envoi message\n");
        message->print(message, 100);

        if (write_in_queue(&queueMsgGUI, message, sizeof (DMessage)) < 0) {
            message->free(message);
        }
    }
}

void thread_recv_monitor(void *arg) {
    DMessage *msg = d_new_message();
    int var1 = 1;
    int num_msg = 0;

    rt_printf("tserver : Début de l'exécution de serveur\n");
    serveur->open(serveur, "8000");
    rt_printf("tserver : Connexion\n");

    rt_mutex_acquire(&mutexEtat, TM_INFINITE);
    etatCommMoniteur = 0;
    rt_mutex_release(&mutexEtat);

    while (1) {
        rt_printf("tserver : Attente d'un message\n");

        rt_mutex_acquire(&mutexEtat, TM_INFINITE);
        etatCommMoniteur = 1;
        rt_mutex_release(&mutexEtat);

        var1 = serveur->receive(serveur, msg);
        num_msg++;
        if (var1 > 0) {
            switch (msg->get_type(msg)) {
                case MESSAGE_TYPE_ACTION:
                    rt_printf("tserver : Le message %d reçu est une action\n",
                            num_msg);
                    DAction *action = d_new_action();
                    action->from_message(action, msg);
                    switch (action->get_order(action)) {
                        case ACTION_CONNECT_ROBOT:
                            rt_printf("tserver : Action connecter robot\n");
                            rt_sem_v(&semConnecterRobot);
                            break;
                    }
                    break;
                case MESSAGE_TYPE_MOVEMENT:
                    rt_printf("tserver : Le message reçu %d est un mouvement\n",
                            num_msg);
                    rt_mutex_acquire(&mutexMove, TM_INFINITE);
                    move->from_message(move, msg);
                    move->print(move);
                    rt_mutex_release(&mutexMove);
                    break;
            }
        }
        else{
            rt_printf("Client disconnected, stopping robot and restarting server\n");
            rt_mutex_acquire(&mutexEtat, TM_INFINITE);
            etatCommMoniteur = 0;
            rt_mutex_release(&mutexEtat);
            rt_mutex_acquire(&mutexMove, TM_INFINITE);
            move->set(move, DIRECTION_STOP, 0);
            move->print(move);
            rt_mutex_release(&mutexMove);
            serveur->close(serveur);
            serveur->open(serveur, "8000");
        }
    }
}

void thread_move_robot(void *arg) {
    int status = 1;
    int gauche;
    int droite;
    unsigned int nbrErreur = 0;
    DMessage *message;

    rt_printf("tmove : Debut de l'éxecution de periodique à 200ms\n");
    rt_task_set_periodic(NULL, TM_NOW, 200000000);

    while (1) {
        /* Attente de l'activation périodique */
        rt_task_wait_period(NULL);
        rt_printf("tmove : Activation périodique\n");

        rt_mutex_acquire(&mutexEtat, TM_INFINITE);
        status = etatCommRobot;
        rt_mutex_release(&mutexEtat);

        if (status == STATUS_OK) {
            rt_mutex_acquire(&mutexMove, TM_INFINITE);
            switch (move->get_direction(move)) {
                case DIRECTION_FORWARD:
                    gauche = MOTEUR_ARRIERE_LENT;
                    droite = MOTEUR_ARRIERE_LENT;
                    break;
                case DIRECTION_LEFT:
                    gauche = MOTEUR_ARRIERE_LENT;
                    droite = MOTEUR_AVANT_LENT;
                    break;
                case DIRECTION_RIGHT:
                    gauche = MOTEUR_AVANT_LENT;
                    droite = MOTEUR_ARRIERE_LENT;
                    break;
                case DIRECTION_STOP:
                    gauche = MOTEUR_STOP;
                    droite = MOTEUR_STOP;
                    break;
                case DIRECTION_STRAIGHT:
                    gauche = MOTEUR_AVANT_LENT;
                    droite = MOTEUR_AVANT_LENT;
                    break;
            }
            rt_mutex_release(&mutexMove);

            status = robot->set_motors(robot, gauche, droite);

            if(status != STATUS_OK)
            {
                nbrErreur++;
            } else {
                nbrErreur = 0;
            }

            if (status != STATUS_OK && nbrErreur >= 10) {
                rt_mutex_acquire(&mutexEtat, TM_INFINITE);
                etatCommRobot = status;
                rt_mutex_release(&mutexEtat);

                message = d_new_message();
                message->put_state(message, status);

                rt_printf("tmove : Envoi message\n");
                if (write_in_queue(&queueMsgGUI, message, sizeof (DMessage)) < 0) {
                    message->free(message);
                }
                nbrErreur = 0;
            }
        }
    }
}
/*
* Periodic thread 250ms
* Check batterie state 
*/
void thread_battery_state(void * args){

    int status = 1;
    int tmp_battery = -1;
    int nbrErreur = 0;
    DMessage *message;

    rt_task_set_periodic(NULL, TM_NOW, 250000000);
    
    while (1) {
        rt_task_wait_period(NULL);
        rt_printf("tBatterieThread : Activation périodique\n");

        rt_mutex_acquire(&mutexEtat, TM_INFINITE);
        status = etatCommRobot;
        rt_mutex_release(&mutexEtat);

        print_status(status);
        if (status == STATUS_OK) {
            message = d_new_message();    
            
            rt_mutex_acquire(&mutexMove, TM_INFINITE);
            status = robot->get_vbat(robot, &tmp_battery);
            rt_mutex_release(&mutexMove);

            if(status == STATUS_OK){
                nbrErreur = 0;

                rt_mutex_acquire(&mutexBattery, TM_INFINITE);
                if(tmp_battery!=BATTERY_LEVEL_UNKNOWN)
                    battery->set_level(battery,tmp_battery);
                message->put_battery_level(message,battery);
                rt_mutex_release(&mutexBattery);

                rt_printf("tbattery_state : Envoi message\n");
                if (write_in_queue(&queueMsgGUI, message, sizeof (DMessage)) < 0) {
                    message->free(message);
                }
            }
            else if(status != STATUS_OK && nbrErreur<3){
                nbrErreur++;
                status = STATUS_OK;
            }
            else{
                rt_mutex_acquire(&mutexEtat, TM_INFINITE);
                etatCommRobot = status;
                rt_mutex_release(&mutexEtat);
            }
        }
    }
}

/*
* Periodic thread 600ms
* Send images to client if connected
*/
void thread_image(void * args){

    int status;
    DImage *image = d_new_image();
    DJpegimage *jpeg = d_new_jpegimage();
    DMessage *message = d_new_message();

    rt_task_set_periodic(NULL, TM_NOW, 600000000);
    
    while (1) {
        rt_task_wait_period(NULL);
        rt_printf("tImagesThread : Activation périodique\n");

        rt_mutex_acquire(&mutexEtat, TM_INFINITE);
        status = etatCommMoniteur;
        rt_mutex_release(&mutexEtat);

        if(status == 1)
        {
            if(camera->get_frame(camera, image) == 0)
            {
                jpeg->compress(jpeg, image);
                message->put_jpeg_image(message, jpeg);
                if (write_in_queue(&queueMsgGUI, message, sizeof (DMessage)) < 0) {
                    message->free(message);
                }
            } else {
                rt_printf("tImagesThread : impossible de toper l'image....\n");
            }
        }
    }

}

int write_in_queue(RT_QUEUE *msgQueue, void * data, int size) {
    void *msg;
    int err;

    msg = rt_queue_alloc(msgQueue, size);
    memcpy(msg, &data, size);

    if ((err = rt_queue_send(msgQueue, msg, sizeof (DMessage), Q_NORMAL)) < 0) {
        rt_printf("Error msg queue send: %s\n", strerror(-err));
    }
    

    return err;
}

void print_status(int status){
    switch(status){
        default:
        case STATUS_OK: 
            rt_printf(" ------------> STATUS_OK\n");
            break;
        case STATUS_ERR_NO_FILE:
            rt_printf(" ------------> STATUS_ERR_NO_FILE\n");
            break;
        case STATUS_ERR_TIMEOUT: 
            rt_printf(" ------------> STATUS_ERR_TIMEOUT\n");
            break;
        case STATUS_ERR_UNKNOWN_CMD: 
            rt_printf(" ------------> STATUS_ERR_UNKNOWN_CMD\n");
            break;
        case STATUS_ERR_INVALID_PARAMS: 
            rt_printf(" ------------> STATUS_ERR_INVALID_PARAMS\n");
            break;
        case STATUS_ERR_WDT_EXPIRED: 
            rt_printf(" ------------> STATUS_ERR_WDT_EXPIRED\n");
            break;
        case STATUS_ERR_SELECT: 
            rt_printf(" ------------> STATUS_ERR_SELECT\n");
            break;
        case STATUS_ERR_UNKNOWN: 
            rt_printf(" ------------> STATUS_ERR_UNKNOWN\n");
            break;
        case STATUS_ERR_CHECKSUM: 
            rt_printf(" ------------> STATUS_ERR_CHECKSUM\n");
            break;  
    }
}
