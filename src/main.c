#include "includes.h"
#include "global.h"
#include "threads.h"
#include "mutex.h"
#include "msg_queue.h"

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
    threads_start();
    pause();
    threads_stop();

    return 0;
}

void initStruct(void) {
    int err;

    /* Creation des mutex */
    mutex_init();

    /* Creation du semaphore */
    if ((err = rt_sem_create(&semConnecterRobot, NULL, 0, S_FIFO))) {
        rt_printf("Error semaphore create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

    /* Creation des taches */
    threads_init();

    /* Creation des files de messages */
    // if ((err = rt_queue_create(&queueMsgGUI, "toto", MSG_QUEUE_SIZE*sizeof(DMessage), MSG_QUEUE_SIZE, Q_FIFO))){
    //     rt_printf("Error msg queue create: %s\n", strerror(-err));
    //     exit(EXIT_FAILURE);
    // }
    msg_queue_init();

    /* Creation des structures globales du projet */
    robot = d_new_robot();
    move = d_new_movement();
    serveur = d_new_server();
    battery = d_new_battery();
    camera = d_new_camera();
}
