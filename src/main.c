#include "includes.h"
#include "global.h"
#include "threads.h"

/**
 * \fn void initStruct(void)
 * \brief Initialisation des structures de l'application (tâches, mutex, 
 * semaphore, etc.)
 */
void initStruct(void);

/**
 * \fn void startTasks(void)
 * \brief Démarrage des tâches
 */
void startTasks(void);

/**
 * \fn void deleteTasks(void)
 * \brief Arrêt des tâches
 */
void deleteTasks(void);

int main(int argc, char**argv) {
    printf("#################################\n");
    printf("#      DE STIJL PROJECT         #\n");
    printf("#################################\n");

    //signal(SIGTERM, catch_signal);
    //signal(SIGINT, catch_signal);

    /* Avoids memory swapping for this program */
    mlockall(MCL_CURRENT | MCL_FUTURE);
    /* For printing, please use rt_print_auto_init() and rt_printf () in rtdk.h
     * (The Real-Time printing library). rt_printf() is the same as printf()
     * except that it does not leave the primary mode, meaning that it is a
     * cheaper, faster version of printf() that avoids the use of system calls
     * and locks, instead using a local ring buffer per real-time thread along
     * with a process-based non-RT thread that periodically forwards the
     * contents to the output stream. main() must call rt_print_auto_init(1)
     * before any calls to rt_printf(). If you forget this part, you won't see
     * anything printed.
     */
    rt_print_auto_init(1);
    initStruct();
    startTasks();
    pause();
    deleteTasks();

    return 0;
}

void initStruct(void) {
    int err;
    /* Creation des mutex */
    if ((err = rt_mutex_create(&mutexEtat, NULL))) {
        rt_printf("Error mutex create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }
    if ((err = rt_mutex_create(&mutexMove, NULL))) {
        rt_printf("Error mutex create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }
    if ((err = rt_mutex_create(&mutexBattery, NULL))) {
        rt_printf("Error mutex create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

    /* Creation du semaphore */
    if ((err = rt_sem_create(&semConnecterRobot, NULL, 0, S_FIFO))) {
        rt_printf("Error semaphore create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

    /* Creation des taches */
    if ((err = rt_task_create(&task_thread_recv_monitor, NULL, 0, PRIORITY_RECV_MONITOR, 0))) {
        rt_printf("Error task create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }
    if ((err = rt_task_create(&task_thread_connect_robot, NULL, 0, PRIORITY_CONNECT_ROBOT, 0))) {
        rt_printf("Error task create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }
    if ((err = rt_task_create(&task_thread_move_robot, NULL, 0, PRIORITY_MOVE_ROBOT, 0))) {
        rt_printf("Error task create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }
    if ((err = rt_task_create(&task_thread_send_monitor, NULL, 0, PRIORITY_SEND_MONITOR, 0))) {
        rt_printf("Error task create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }
    if ((err = rt_task_create(&task_thread_battery_state, NULL, 0, PRIORITY_BATTERY_STATE, 0))) {
        rt_printf("Error task create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

    if ((err = rt_task_create(&task_thread_image, NULL, 0, PRIORITY_IMAGE, 0))) {
        rt_printf("Error task create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

    /* Creation des files de messages */
    if ((err = rt_queue_create(&queueMsgGUI, "toto", MSG_QUEUE_SIZE*sizeof(DMessage), MSG_QUEUE_SIZE, Q_FIFO))){
        rt_printf("Error msg queue create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

    /* Creation des structures globales du projet */
    robot = d_new_robot();
    move = d_new_movement();
    serveur = d_new_server();
    battery = d_new_battery();
    camera = d_new_camera();
}

void startTasks() {
    int err;

    if ((err = rt_task_start(&task_thread_recv_monitor, &thread_recv_monitor, NULL))) {
        rt_printf("Error task start: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

    if ((err = rt_task_start(&task_thread_connect_robot, &thread_connect_robot, NULL))) {
        rt_printf("Error task start: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

    if ((err = rt_task_start(&task_thread_move_robot, &thread_move_robot, NULL))) {
        rt_printf("Error task start: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

    if ((err = rt_task_start(&task_thread_send_monitor, &thread_send_monitor, NULL))) {
        rt_printf("Error task start: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }
    if ((err = rt_task_start(&task_thread_battery_state, &thread_battery_state, NULL))) {
        rt_printf("Error task start: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

    if ((err = rt_task_start(&task_thread_image, &thread_image, NULL))) {
        rt_printf("Error task start: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

}

void deleteTasks() {
    rt_task_delete(&task_thread_recv_monitor);
    rt_task_delete(&task_thread_connect_robot);
    rt_task_delete(&task_thread_move_robot);
    rt_task_delete(&task_thread_send_monitor);
    rt_task_delete(&task_thread_battery_state);
    rt_task_delete(&task_thread_image);
}
