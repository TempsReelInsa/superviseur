#include "status.h"

int nb_error = 0;
int etatCommRobot = 1;

void print_status(int status){
    switch(status){
        default:
        case STATUS_OK: 
            LOG_HANDLE_ERROR(" ------------> STATUS_OK\n");
            break;
        case STATUS_ERR_NO_FILE:
            LOG_HANDLE_ERROR(" ------------> STATUS_ERR_NO_FILE\n");
            break;
        case STATUS_ERR_TIMEOUT: 
            LOG_HANDLE_ERROR(" ------------> STATUS_ERR_TIMEOUT\n");
            break;
        case STATUS_ERR_UNKNOWN_CMD: 
            LOG_HANDLE_ERROR(" ------------> STATUS_ERR_UNKNOWN_CMD\n");
            break;
        case STATUS_ERR_INVALID_PARAMS: 
            LOG_HANDLE_ERROR(" ------------> STATUS_ERR_INVALID_PARAMS\n");
            break;
        case STATUS_ERR_WDT_EXPIRED: 
            LOG_HANDLE_ERROR(" ------------> STATUS_ERR_WDT_EXPIRED\n");
            break;
        case STATUS_ERR_SELECT: 
            LOG_HANDLE_ERROR(" ------------> STATUS_ERR_SELECT\n");
            break;
        case STATUS_ERR_UNKNOWN: 
            LOG_HANDLE_ERROR(" ------------> STATUS_ERR_UNKNOWN\n");
            break;
        case STATUS_ERR_CHECKSUM: 
            LOG_HANDLE_ERROR(" ------------> STATUS_ERR_CHECKSUM\n");
            break;  
    }
}

void status_process_hard(int status){
    DMessage *message;

    mutex_state_acquire();
    etatCommRobot = status;
    mutex_state_release();

	if(status != STATUS_OK){
        
        LOG_HANDLE_ERROR("Status error\n");
        print_status(status);

        message = d_new_message();
        message->put_state(message, status);

        LOG_MOVE_ROBOT("Envoi message\n");
        if(msg_queue_write(message) < 0)
        {
            message->free(message);
        }
	}
    else{
        mutex_state_acquire();
        nb_error = 0;
        mutex_state_release();
    }
}
void status_process(int status){
    DMessage *message;

	if(nb_error > MAX_ERROR){
        status_process_hard(status);
	}
	else {
        mutex_state_acquire();
		nb_error++;
        mutex_state_release();
	}
}

int status_check(int bloquant){
	int status;

	mutex_state_acquire();
    status = etatCommRobot;
    mutex_state_release();

    if(bloquant && status != STATUS_OK){
        LOG_HANDLE_ERROR("Wait semaphore STATUS_OK\n");
        rt_sem_p(&semStatusOk,TM_INFINITE);
        LOG_HANDLE_ERROR("STATUS_OK\n");
    }

    return status == STATUS_OK;
}


