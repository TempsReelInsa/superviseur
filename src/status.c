#include "status.h"

int nb_error = 0;
int etatCommRobot = 1;

void set_status_robot(int s)
{
    mutex_state_acquire();
    etatCommRobot = s;
    mutex_state_release();
}

int get_status_robot()
{
    int ret;
    mutex_state_acquire();
    ret = etatCommRobot;
    mutex_state_release();
    return ret;
}


char * print_status(int status){
    switch(status){
        default:
        case STATUS_OK: 
            return("STATUS_OK");
            break;
        case STATUS_ERR_NO_FILE:
            return("STATUS_ERR_NO_FILE");
            break;
        case STATUS_ERR_TIMEOUT: 
            return("STATUS_ERR_TIMEOUT");
            break;
        case STATUS_ERR_UNKNOWN_CMD: 
            return("STATUS_ERR_UNKNOWN_CMD");
            break;
        case STATUS_ERR_INVALID_PARAMS: 
            return("STATUS_ERR_INVALID_PARAMS");
            break;
        case STATUS_ERR_WDT_EXPIRED: 
            return("STATUS_ERR_WDT_EXPIRED");
            break;
        case STATUS_ERR_SELECT: 
            return("STATUS_ERR_SELECT");
            break;
        case STATUS_ERR_UNKNOWN: 
            return("STATUS_ERR_UNKNOWN");
            break;
        case STATUS_ERR_CHECKSUM: 
            return("STATUS_ERR_CHECKSUM");
            break;  
    }
}

void status_process_hard(int status){
    DMessage *message;

    mutex_state_acquire();
    etatCommRobot = status;

	if(status != STATUS_OK){
        
        LOG_STATUS("%s\n",print_status(status));

        message = d_new_message();
        message->put_state(message, status);

        LOG_MOVE_ROBOT("Envoi message\n");
        if(msg_queue_write(message) < 0)
        {
            message->free(message);
        }
	}
    else{
        nb_error = 0;

        if(rt_sem_broadcast(&semStatusOk)!=0){
            perror("rt_sem_broadcast");
            exit(EXIT_FAILURE);
        }
    }
    mutex_state_release();
}
void status_process(int status){

	if(nb_error > MAX_ERROR || status == STATUS_OK){
        status_process_hard(status);
	}
	else {
        mutex_state_acquire();
		nb_error++;
        LOG_STATUS("++Nb error(%d)\n",nb_error);
        mutex_state_release();	
    }
}

int status_check(int bloquant){
	int status;

	mutex_state_acquire();
    status = etatCommRobot;
    mutex_state_release();

    if(bloquant && status != STATUS_OK){
        LOG_STATUS("Wait semaphore STATUS_OK\n");
        if(rt_sem_p(&semStatusOk,TM_INFINITE)!=0){
            perror("rt_sem_p");
            exit(EXIT_FAILURE);
        }
    }

    return status == STATUS_OK;
}
