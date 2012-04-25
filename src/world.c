
#include <craftcore.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

void cc_world_init(cc_world_t *world)
{
	memset(world, 0, sizeof(cc_world_t));
	cc_camera_t *camera = cc_new(camera);
	cc_entity_init(&camera->ent);
	
	cc_camera_set_color(camera, 0.0f, 0.5f, 0.75f);
	cc_camera_set_zoom(camera, 1.0);
	cc_camera_set_range(camera, 1.0, 1000.0);
	
	world->camera = camera;
}

void cc_world_add_block(cc_world_t *world, cc_block_t *block)
{
	if (world->blocks != NULL)
		world->blocks = realloc(world->blocks, sizeof(cc_block_t*)*world->block_count + 1);		
	else
		world->blocks = malloc(sizeof(cc_block_t*));
	
	world->blocks[world->block_count] = block;
	world->block_count += 1;	
}

void cc_octree_render(cc_octree_t *tree)
{
	int parent = FALSE;
	for (int z = 0;z < 2;z++)
		for (int y = 0;y < 2;y++)
			for (int x = 0;x < 2;x++)
				if (tree->child[x][y][z])
				{
					parent = TRUE;
					cc_octree_render(tree->child[x][y][z]);
				}
	
	if (parent)
		glLineWidth(1.0);
	else
		glLineWidth(1.5);

	glBegin(GL_QUADS);
	
	glVertex3f(tree->bounds.min.x, tree->bounds.min.y, tree->bounds.min.z);
	glVertex3f(tree->bounds.max.x, tree->bounds.min.y, tree->bounds.min.z);
	glVertex3f(tree->bounds.max.x, tree->bounds.max.y, tree->bounds.min.z);
	glVertex3f(tree->bounds.min.x, tree->bounds.max.y, tree->bounds.min.z);
	
	glVertex3f(tree->bounds.min.x, tree->bounds.min.y, tree->bounds.max.z);
	glVertex3f(tree->bounds.max.x, tree->bounds.min.y, tree->bounds.max.z);
	glVertex3f(tree->bounds.max.x, tree->bounds.max.y, tree->bounds.max.z);
	glVertex3f(tree->bounds.min.x, tree->bounds.max.y, tree->bounds.max.z);
	
	glVertex3f(tree->bounds.min.x, tree->bounds.max.y, tree->bounds.min.z);
	glVertex3f(tree->bounds.max.x, tree->bounds.max.y, tree->bounds.min.z);
	glVertex3f(tree->bounds.max.x, tree->bounds.max.y, tree->bounds.max.z);
	glVertex3f(tree->bounds.min.x, tree->bounds.max.y, tree->bounds.max.z);
	
	glVertex3f(tree->bounds.min.x, tree->bounds.min.y, tree->bounds.min.z);
	glVertex3f(tree->bounds.max.x, tree->bounds.min.y, tree->bounds.min.z);
	glVertex3f(tree->bounds.max.x, tree->bounds.min.y, tree->bounds.max.z);
	glVertex3f(tree->bounds.min.x, tree->bounds.min.y, tree->bounds.max.z);
	
	glVertex3f(tree->bounds.max.x, tree->bounds.min.y, tree->bounds.min.z);
	glVertex3f(tree->bounds.max.x, tree->bounds.max.y, tree->bounds.min.z);
	glVertex3f(tree->bounds.max.x, tree->bounds.max.y, tree->bounds.max.z);
	glVertex3f(tree->bounds.max.x, tree->bounds.min.y, tree->bounds.max.z);
	
	glVertex3f(tree->bounds.min.x, tree->bounds.min.y, tree->bounds.min.z);
	glVertex3f(tree->bounds.min.x, tree->bounds.max.y, tree->bounds.min.z);
	glVertex3f(tree->bounds.min.x, tree->bounds.max.y, tree->bounds.max.z);
	glVertex3f(tree->bounds.min.x, tree->bounds.min.y, tree->bounds.max.z);	
	
	glEnd();
}

void cc_world_render(cc_world_t *world, int width, int height)
{
	float matrix[16];
	
	cc_camera_t *camera = world->camera;
	
	glViewport(0,0,width,height);
	glScissor(0,0,width,height);
	glClearColor(camera->r, camera->g, camera->b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
		
	float ratio=(float)width/(float)height;
	gluPerspective(R2D(atan((1.0f/(world->camera->zoom*ratio)))*2.0f), ratio, world->camera->near, world->camera->far);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glMatrixMode(GL_MODELVIEW);

	cc_matrix_copy(matrix, camera->ent.matrix);
	cc_matrix_inverse(matrix);
	glLoadMatrixf(matrix);
	
	glGetIntegerv(GL_VIEWPORT, camera->viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, camera->wrld_matrix);
	glGetDoublev(GL_PROJECTION_MATRIX, camera->proj_matrix);
	
	/*glPushMatrix();
	cc_identity_matrix(matrix);
	cc_matrix_translate(matrix, -CHUNKSIZE,0.0f,CHUNKSIZE);
	glMultMatrixf(matrix);*/
	
	cc_atlas_set(world->blocks[0]->texture[0]->atlas);
	
	cc_chunk_render(world->chunks[0]);
	
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0, 1.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	cc_octree_render(world->chunks[0]->tree);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	
	glPopMatrix();

}
cc_chunk_t *cc_world_add_chunk(cc_world_t *world)
{
	world->chunks = malloc(sizeof(cc_world_t*)*1);
	world->chunks[0] = cc_new_chunk();
	return world->chunks[0];
}
