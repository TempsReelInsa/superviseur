#include "utils.h"
#include "status.h"
#include "monitor_status.h"

void restart_robot()
{
	robot->stop(robot);
	status_process_hard(STATUS_ERR_NO_FILE); // stoooooop
}

void restart_all()
{
	restart_robot();
	monitor_status_set(MONITOR_STATUS_NOT_OK);
	serveur->close(serveur); // restart server
	serveur->open(serveur, "8000");
}
