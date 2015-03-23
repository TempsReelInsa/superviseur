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

int msg_queue_write(DMessage *message)
{
    DMessage *qmsg;
    int err;

    qmsg = rt_queue_alloc(&msg_queue, sizeof(DMessage));
    if(qmsg == NULL)
    {
        return -1;
    }

    d_message_init(qmsg);
    qmsg->dataType = message->get_type(message);
    qmsg->len = message->get_lenght(message);
    qmsg->data = malloc(message->get_lenght(message));

    if(qmsg->data == NULL)
    {
        return -1;
    }

    memcpy(qmsg->data, message->get_data(message), qmsg->get_lenght(qmsg));

    if ((err = rt_queue_send(&msg_queue, qmsg, sizeof(DMessage), Q_NORMAL)) < 0) {
        rt_printf("Error msg queue send: %s\n", strerror(-err));
    }

    return err;
}

int msg_queue_get(DMessage **data)
{
    int err;
    DMessage *qmsg = NULL;
	if((err = rt_queue_receive(&msg_queue, (void **) &qmsg, TM_INFINITE)) >= 0 && qmsg != NULL)
    {
        (*data) = d_new_message();
        (*data)->dataType = qmsg->get_type(qmsg);
        (*data)->len = qmsg->get_lenght(qmsg);
        (*data)->data = malloc(qmsg->get_lenght(qmsg));
        memcpy((*data)->data, qmsg->get_data(qmsg), qmsg->get_lenght(qmsg));

        msg_queue_free(qmsg);

    }
    return err;
}

void msg_queue_free(DMessage *data)
{
    free(data->data);
	rt_queue_free(&msg_queue, data);
}
