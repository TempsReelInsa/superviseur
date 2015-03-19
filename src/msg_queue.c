#include "msg_queue.h"

RT_QUEUE msg_queue;

void msg_queue_init()
{
	int err;
	if ((err = rt_queue_create(&msg_queue, "msg_queue", MSG_QUEUE_SIZE*sizeof(DMessage), MSG_QUEUE_SIZE, Q_FIFO))){
		rt_printf("Error msg queue create: %s\n", strerror(-err));
		exit(EXIT_FAILURE);
	}
}

int msg_queue_write(DMessage *data)
{
    void *msg;
    int err;

    msg = rt_queue_alloc(&msg_queue, sizeof(DMessage));
    memcpy(msg, &data, sizeof(DMessage));

    if ((err = rt_queue_send(&msg_queue, msg, sizeof (DMessage), Q_NORMAL)) < 0) {
        rt_printf("Error msg queue send: %s\n", strerror(-err));
    }

    return err;
}

int msg_queue_get(DMessage **data)
{
	return rt_queue_read(&msg_queue, data, sizeof (DMessage), TM_INFINITE);
}

void msg_queue_free(DMessage *data)
{
	rt_queue_free(&msg_queue, data);
}
