
#include <craftcore.h>

// TODO: Implement projection functions to remove GL dependency.
#include <GL/gl.h>
#include <GL/glu.h>

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

void cc_camera_unproject(cc_camera_t *camera, float x, float y, float z, cc_vec3_t *vec)
{
	double ox, oy, oz;
	gluUnProject(x, y, z, camera->wrld_matrix, camera->proj_matrix, camera->viewport, &ox, &oy, &oz);
	vec->x = (float)ox;
	vec->y = (float)oy;
	vec->z = (float)oz;
}
