#ifndef STATUS_H
#define	STATUS_H

#include "global.h"
#include "includes.h"
#include "debug.h"
#include "mutex.h"
#include "msg_queue.h"

#define MAX_ERROR 2

void status_process(int status);
void status_process_hard(int status);
int status_check(int bloquant);

#endif /* STATUS_H */
