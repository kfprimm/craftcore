
#include <craftcore.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

void cc_world_init(cc_world_t *world)
{
	cc_camera_t *camera = cc_new(camera);
	cc_entity_init(&camera->ent);
	
	cc_camera_set_color(camera, 0.0f, 0.5f, 0.75f);
	cc_camera_set_zoom(camera, 1.0);
	cc_camera_set_range(camera, 1.0, 1000.0);
	
	world->camera = camera;
}

void cc_world_render(cc_world_t *world, int width, int height)
{
	float matrix[16];
	
	cc_render_3d();
	
	cc_camera_t *camera = world->camera;
	
	glViewport(0,0,width,height);
	glScissor(0,0,width,height);
	glClearColor(camera->r, camera->g, camera->b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
		
	float ratio=(float)width/(float)height;
	gluPerspective(R2D(atan((1.0f/(world->camera->zoom*ratio)))*2.0f), ratio, world->camera->near, world->camera->far);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glMatrixMode(GL_MODELVIEW);

	cc_matrix_copy(matrix, camera->ent.matrix);
	cc_matrix_inverse(matrix);
	glLoadMatrixf(matrix);
	
	glPushMatrix();
	cc_identity_matrix(matrix);
	cc_matrix_translate(matrix, -CHUNKSIZE,0.0f,CHUNKSIZE);
	glMultMatrixf(matrix);
	
	glClientActiveTextureARB(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, world->texture);
	
	glPopMatrix();

}
cc_chunk_t *cc_world_add_chunk(cc_world_t *world)
{
	world->chunks = malloc(sizeof(cc_world_t*)*1);
	world->chunks[0] = cc_new_chunk();
	return world->chunks[0];
}
