#include "handle_error.h"

int nb_error = 0;

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

void handle_error_process_hard(int status){
    DMessage *message;

    mutex_state_acquire();
    etatCommRobot = status;
    mutex_state_release();

    nb_error = 0;

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
}
void handle_error_process(int status){
    DMessage *message;

	if(nb_error > MAX_ERROR){
        handle_error_process_hard(status);
	}
	else {
		nb_error++;
	}
}

int handle_error_check(){
	int status;

	mutex_state_acquire();
    status = etatCommRobot;
    mutex_state_release();

    return status == STATUS_OK;
}


