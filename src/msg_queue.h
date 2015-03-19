#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

#include "includes.h"

#define MSG_QUEUE_SIZE 10

void msg_queue_init();
int msg_queue_write(DMessage * data);
int msg_queue_get(DMessage **data);
void msg_queue_free(DMessage *data);

#endif