#include "utils.h"
#include "status.h"
#include "image_status.h"
#include "mutex.h"

void restart_robot()
{
	status_process_hard(STATUS_ERR_NO_FILE); // stoooooop
	robot->stop(robot);
}

void restart_all()
{
	restart_robot();
	image_reset_all();
	serveur->close_client(serveur); // restart server
}

DImage *get_image()
{
	DImage *image = d_new_image();

	if(image != NULL)
	{
		camera->get_frame(camera, image);
	}

	return image;
}

DJpegimage *get_jpeg_from_image(DImage *i)
{
	DJpegimage * jpeg = d_new_jpegimage();

	if(jpeg != NULL)
	{
		jpeg->compress(jpeg, i);
	}

	return jpeg;
}

DJpegimage *get_jpeg()
{
	DImage *img = get_image();
	DJpegimage *jpeg = NULL;

	if(img != NULL)
	{
		if((jpeg = get_jpeg_from_image(img)) != NULL)
		{
			return jpeg;
		} else {
			img->free(img);
			return NULL;
		}
	} else {
		return NULL;
	}
}
