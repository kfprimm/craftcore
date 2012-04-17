
#include <craftcore.h>

void cc_camera_set_color(cc_camera_t *camera, float r, float g, float b)
{
	camera->r = r;
	camera->g = g;
	camera->b = b;
}

void cc_camera_set_zoom(cc_camera_t *camera, float zoom)
{
	camera->zoom = zoom;
}

void cc_camera_set_range(cc_camera_t *camera, float near, float far)
{
	camera->near = near;
	camera->far  = far;
}
