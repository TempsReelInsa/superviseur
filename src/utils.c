#include "utils.h"
#include "status.h"

void restart_robot()
{
	robot->stop(robot);
	status_process_hard(STATUS_ERR_NO_FILE); // stoooooop
}

void restart_all()
{
	restart_robot();
	serveur->close(serveur); // restart server
	serveur->open(serveur, "8000");
}
