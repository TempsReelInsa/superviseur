#include "monitor_status.h"
#include "mutex.h"
#include "debug.h"

int monitor_status = MONITOR_STATUS_NOT_OK;

void monitor_status_set(int v)
{
	mutex_monitor_status_acquire();
	monitor_status = v;

	if(monitor_status == MONITOR_STATUS_OK)
	{
		if(rt_sem_broadcast(&semMonitorStatusOk) != 0)
		{
			perror("rt_sem_broadcast");
			exit(EXIT_FAILURE);
		}
	}

	mutex_monitor_status_release();
}

int monitor_status_get()
{
	int ret;
	mutex_monitor_status_acquire();
	ret = monitor_status;
	mutex_monitor_status_release();
	return ret;
}


int monitor_status_check()
{
	int status = monitor_status_get();

    if(status != MONITOR_STATUS_OK){
        DPRINTF("Wait semaphore MONITOR_STATUS_OK\n");
        if(rt_sem_p(&semMonitorStatusOk,TM_INFINITE)!=0){
            perror("rt_sem_p");
            exit(EXIT_FAILURE);
        }

        status = monitor_status_get();

    }

    return status == STATUS_OK;
}

