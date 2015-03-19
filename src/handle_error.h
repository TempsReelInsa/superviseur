#ifndef HANDLE_ERROR_H
#define	HANDLE_ERROR_H

#include "global.h"
#include "includes.h"
#include "debug.h"
#include "mutex.h"
#include "msg_queue.h"

#define MAX_ERROR 2

void handle_error_process(int status);
void handle_error_process_hard(int status);
int handle_error_check();

#endif /* HANDLE_ERROR_H */
