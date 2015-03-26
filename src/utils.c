#include "utils.h"
#include "status.h"
#include "image_status.h"

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

// DImage *get_image()
// {
// 	DImage *image = d_new_image();

// 	if(image != NULL)
// 	{
// 		camera->get_frame(camera, image);
// 	}

// 	return image;
// }

// DJpegimage *get_jpeg_from_image(DImage *i)
// {
// 	DJpegimage * jpeg = d_new_jpegimage();

// 	if(jpeg != NULL)
// 	{
// 		jpeg->compress(jpeg, i);
// 	}

// 	return jpeg;
// }

// DJpegimage *get_jpeg()
// {
// 	DImage *img = get_image();
// 	DJpegimage *jpeg = NULL;

// 	if(img != NULL)
// 	{
// 		if((jpeg = get_jpeg_from_image(img)) != NULL)
// 		{
// 			return jpeg;
// 		} else {
// 			img->free(img);
// 			return NULL;
// 		}
// 	} else {
// 		return NULL;
// 	}
// }
