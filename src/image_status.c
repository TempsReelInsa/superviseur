#include "image_status.h"
#include "mutex.h"
#include "debug.h"

int image_status = IMAGE_STATUS_NO;
int image_detect_area = IMAGE_FIND_ARENA_NO;
int image_compute_position = 0;

int image_status_set(int status){
	mutex_image_status_acquire();
	image_status = status;
	mutex_image_status_release();

	if(rt_sem_broadcast(&semMonitorStatusUpdated) != 0) // updated !!
	{
		perror("rt_sem_broadcast");
		exit(EXIT_FAILURE);
	}
}

void image_status_wait_for(int status){
	int ret;
	mutex_image_status_acquire();
	ret = image_status;
	mutex_image_status_release();

	while(ret != status){
		if(rt_sem_p(&semMonitorStatusUpdated, TM_INFINITE) !=0 )
		{
			perror("rt_sem_p");
			exit(EXIT_FAILURE);
		}

		mutex_image_status_acquire();
		ret = image_status;
		mutex_image_status_release();
	}
}

int image_is_compute_position(){
	int ret;
	mutex_image_status_acquire();
	ret = image_compute_position;
	mutex_image_status_release();
	return ret;
}

void image_set_compute_position(int ret){
	mutex_image_status_acquire();
	image_compute_position = ret;
	mutex_image_status_release();
}

int image_get_detect_area(){
	int ret;
	mutex_image_status_acquire();
	ret = image_detect_area;
	mutex_image_status_release();
	return ret;
}

void image_no_wait_detect_area(){
	int ret;
	mutex_image_status_acquire();
	ret = image_detect_area;
	mutex_image_status_release();

	while(ret == IMAGE_FIND_ARENA_WAIT){
		DPRINTF("Wait detect area update ... \n");
		if(rt_sem_p(&semDetectArena, TM_INFINITE) !=0 )
		{
			perror("rt_sem_p");
			exit(EXIT_FAILURE);
		}
		mutex_image_status_acquire();
		ret = image_detect_area;
		mutex_image_status_release();
	}
}

void image_set_detect_area(int ret){
	mutex_image_status_acquire();
	image_detect_area = ret;
	mutex_image_status_release();

	if(rt_sem_broadcast(&semDetectArena) != 0) // updated !!
	{
		perror("rt_sem_broadcast");
		exit(EXIT_FAILURE);
	}
}