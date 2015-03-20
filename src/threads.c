#include "threads.h"
#include "msg_queue.h"
#include "debug.h"

#define PRIORITY_RECV_MONITOR 30
#define PRIORITY_CONNECT_ROBOT 20
#define PRIORITY_MOVE_ROBOT 10
#define PRIORITY_SEND_MONITOR 25
#define PRIORITY_BATTERY_STATE 25
#define PRIORITY_IMAGE 25
#define PRIORITY_WATCHDOG 5

RT_TASK task_thread_batterie_state;
RT_TASK task_thread_connect_robot;
RT_TASK task_thread_recv_monitor;
RT_TASK task_thread_send_monitor;
RT_TASK task_thread_move_robot;
RT_TASK task_thread_battery_state;
RT_TASK task_thread_image;
RT_TASK task_thread_watchdog;

RT_TASK *threads_tasks_tab[] = {
    &task_thread_send_monitor,
    &task_thread_connect_robot,
    &task_thread_recv_monitor,
    &task_thread_move_robot,
    &task_thread_battery_state,
    &task_thread_image,
    &task_thread_watchdog,
    NULL
};

void (*threads_functions_tab[])(void *) = {
    &thread_send_monitor,
    &thread_connect_robot,
    &thread_recv_monitor,
    &thread_move_robot,
    &thread_battery_state,
    &thread_image,
    &thread_watchdog,
    NULL
};

int threads_priority[] = {
    PRIORITY_SEND_MONITOR,
    PRIORITY_CONNECT_ROBOT,
    PRIORITY_RECV_MONITOR,
    PRIORITY_MOVE_ROBOT,
    PRIORITY_BATTERY_STATE,
    PRIORITY_IMAGE,
    PRIORITY_WATCHDOG,
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
        }
    }
}

void threads_stop()
{
    int i;
    for(i=0; threads_tasks_tab[i] != NULL; i++)
    {
        rt_task_delete(threads_tasks_tab[i]);
    }
}

void thread_send_monitor(void * arg) {
    DMessage *msg;
    int err;

    BEGIN_THREAD();

    while (1) {
        if ((err = msg_queue_get(&msg)) >= 0) {
            LOG_SEND_MONITOR("Sending message to monitor ... \n");
            serveur->send(serveur, msg);
            msg->free(msg);
        } else {
            LOG_SEND_MONITOR("Error msg queue write: %s\n", strerror(-err));
        }
    }
}

void thread_connect_robot(void * arg) {
    int status;
    int version_min,version_max;
    DMessage *message;

    BEGIN_THREAD();

    while (1) {
        LOG_CONNECT_ROBOT("Attente du sémarphore semConnecterRobot\n");
        rt_sem_p(&semConnecterRobot, TM_INFINITE);
        LOG_CONNECT_ROBOT("Ouverture de la communication avec le robot\n");

        mutex_robot_acquire();  
        status = robot->open_device(robot);

        status_process_hard(status);

        if (status_check(0)) {
            status = robot->start_insecurely(robot);
            DPRINTF("-------> status = %d\n",status);
            status_process_hard(status);
            
            if (status_check(0)){

                LOG_CONNECT_ROBOT("********> Robot démarrer<**********\n");
                LOG_CONNECT_ROBOT("tconnect : Launch Watchdog\n");
                rt_sem_v(&semLaunchWatchdog);
                
                robot->get_version(robot, &version_max, &version_min);

                message = d_new_message();
                message->put_version(message,version_max,version_min);
                
                if(msg_queue_write(message) < 0)
                {
                    message->free(message);
                }
            }
        }
        
        mutex_robot_release();

        message = d_new_message();
        message->put_state(message, status);

        LOG_CONNECT_ROBOT("Envoi message\n");
        message->print(message, 100);

        if(msg_queue_write(message) < 0)
        {
            message->free(message);
        }
    }
}

void thread_recv_monitor(void *arg) {
    DMessage *msg = d_new_message();
    DAction *action;
    int size;
    int num_msg = 0;

    BEGIN_THREAD();

    serveur->open(serveur, "8000");
    LOG_RECV_MONITOR("Server binded ...  Waiting for connection & messages\n");

    while (1) {
        LOG_RECV_MONITOR("Wait message ...\n");

        size = serveur->receive(serveur, msg);
        num_msg++;

        if (size > 0)
        {
            LOG_RECV_MONITOR("Message #%d\n", num_msg);
            switch (msg->get_type(msg))
            {
                case MESSAGE_TYPE_ACTION:
                    LOG_RECV_MONITOR("MESSAGE_TYPE_ACTION\n");
                    action = d_new_action();
                    action->from_message(action, msg);
                    switch (action->get_order(action))
                    {
                        case ACTION_CONNECT_ROBOT:
                            LOG_RECV_MONITOR("ACTION_CONNECT_ROBOT\n");
                            rt_sem_v(&semConnecterRobot);
                            break;
                    }
                    break;
                case MESSAGE_TYPE_MOVEMENT:
                    LOG_RECV_MONITOR("MESSAGE_TYPE_MOVEMENT\n");
                    mutex_robot_acquire();
                    move->from_message(move, msg);
                    mutex_robot_release();
                    break;
            }
        }
        else
        {
            /*
            TODO: restart everyshit
            */
            LOG_RECV_MONITOR("Client disconnected, stopping robot and restarting server\n");
            
            mutex_robot_acquire();
            move->set(move, DIRECTION_STOP, 0);
            mutex_robot_release();
            
            serveur->close(serveur);
            serveur->open(serveur, "8000");
        }
    }
}

void thread_move_robot(void *arg) {
    int status = 1;
    int gauche;
    int droite;

    BEGIN_THREAD();
    rt_task_set_periodic(NULL, TM_NOW, 200000000);

    while (1) {
        rt_task_wait_period(NULL);

        if (status_check(1)) {
            mutex_robot_acquire();
            switch (move->get_direction(move))
            {
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
            mutex_robot_release();

            status = robot->set_motors(robot, gauche, droite);

            status_process(status); 
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
    DMessage *message;

    BEGIN_THREAD();
    rt_task_set_periodic(NULL, TM_NOW, 250000000);
    
    while (1) {
        rt_task_wait_period(NULL);
        LOG_BATTERY_STATE("Activation périodique\n");

        if (status_check(1)) {
            mutex_robot_acquire();
            status = robot->get_vbat(robot, &tmp_battery);
            mutex_robot_release();

            status_process(status);

            if (status_check(0)) {
                message = d_new_message();    
                
                mutex_battery_acquire();
                if(tmp_battery!=BATTERY_LEVEL_UNKNOWN)
                    battery->set_level(battery,tmp_battery);
                message->put_battery_level(message,battery);
                mutex_battery_release();

                LOG_BATTERY_STATE("Envoi message\n");
                if(msg_queue_write(message) < 0)
                {
                    message->free(message);
                }

            }
        }
    }
}

/*
* Periodic thread 600ms
* Send images to client if connected
*/
void thread_image(void * args){

    // int status;
    // DImage *image = d_new_image();
    // DJpegimage *jpeg = d_new_jpegimage();
    // DMessage *message = d_new_message();

    // rt_task_set_periodic(NULL, TM_NOW, 600000000);
    
    // while (1) {
    //     rt_task_wait_period(NULL);
    //     rt_printf("tImagesThread : Activation périodique\n");

    //     rt_mutex_acquire(&mutexEtat, TM_INFINITE);
    //     status = etatCommMoniteur;
    //     rt_mutex_release(&mutexEtat);

    //     if(status == 1)
    //     {
    //         if(camera->get_frame(camera, image) == 0)
    //         {
    //             jpeg->compress(jpeg, image);
    //             message->put_jpeg_image(message, jpeg);
    //             if(msg_queue_write(message) < 0)
    //             {
    //                 message->free(message);
    //             }
    //         } else {
    //             rt_printf("tImagesThread : impossible de toper l'image....\n");
    //         }
    //     }
    // }

}

void thread_watchdog(void * args){
    // int status;
    // unsigned int nbrErreur = 0;
    // DMessage *message;

    // BEGIN_THREAD();

    // while(1){
    //     LOG_WATCHDOG("thread_watchdog : Attente du sémarphore semLaunchWatchdog\n");
    //     rt_sem_p(&semLaunchWatchdog, TM_INFINITE);

    //     mutex_state_acquire();
    //     status = etatCommRobot;
    //     mutex_state_release();
    //     LOG_WATCHDOG("thread_watchdog : Started\n");
    //     rt_task_set_periodic(NULL, TM_NOW, 1000000000);

    //     while(status==STATUS_OK){
    //         LOG_WATCHDOG("thread_watchdog : I'm alive\n");

    //         mutex_robot_acquire();
    //         status = robot->reload_wdt(robot);
    //         print_status(status);
    //         mutex_robot_release();

    //         if(status != STATUS_OK)
    //         {
    //             nbrErreur++;
    //             status = STATUS_OK;
    //         } else {
    //             nbrErreur = 0;
    //         }

    //         if (status != STATUS_OK && nbrErreur >= 10) {
    //             mutex_state_acquire();
    //             etatCommRobot = status;
    //             mutex_state_release();

    //             message = d_new_message();
    //             message->put_state(message, status);

    //             LOG_WATCHDOG("tmove : Envoi message\n");
    //             if(msg_queue_write(message) < 0)
    //             {
    //                 message->free(message);
    //             }
    //             nbrErreur = 0;
    //         }
    //         rt_task_wait_period(NULL);
    //     }
    // }
}

