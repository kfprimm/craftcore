
#ifndef CRAFTCORE_H
#define CRAFTCORE_H

#include <stdlib.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <keycodes.h>

#define min(v0, v1) ((v0) > (v1) ? (v1) : (v0))
#define max(v0, v1) ((v0) > (v1) ? (v0) : (v1))

#define FALSE 0
#define TRUE  1
#define cc_new(class) ((cc_##class##_t*)malloc(sizeof(cc_##class##_t)))

// Math

typedef float cc_vec3_t[3];

typedef struct cc_box_t
{
	cc_vec3_t min, max;
} cc_box_t;

typedef struct cc_octree_t
{
	struct cc_octree_t *child0, *child1, *child2, *child3;
	struct cc_octree_t *child4, *child5, *child6, *child7;
	cc_box_t bounds;
	void *data;
} cc_octree_t;

void cc_octree_init(cc_octree_t *tree, cc_box_t *box);

#define PI 3.14159265
#define D2R(v) ((v)*(PI/180.0f))
#define R2D(v) ((v)*(180.0f/PI))

void cc_vec3_add(float *out, float *a, float *b);
void cc_vec3_sub(float *out, float *a, float *b);
void cc_vec3_scale(float *out, float *vec, float factor);

int cc_line_intersection( float dst1, float dst2, float *p1, float *p2, float *hit);
int cc_vec3_in_box( float *vec, float *min, float *max, const int axis);
int cc_box_line_intersection( float *min, float *max, float *l1, float *l2, float *hit);

float *cc_matrix_copy(float *dst, float *src);
float *cc_matrix_multiply(float *out, float *a, float *b);

float *cc_identity_matrix(float *matrix);
float *cc_translation_matrix(float *matrix, float x, float y, float z);
float *cc_scale_matrix(float *matrix, float x, float y, float z);

float *cc_matrix_invert(float *out, float *in);
float *cc_matrix_inverse(float *matrix);

float *cc_matrix_translate(float *matrix, float x, float y, float z);
float *cc_matrix_scale(float *matrix, float x, float y, float z);
float *cc_matrix_rotate(float *matrix, float yaw, float pitch, float roll);

void   cc_matrix_xyz(float *matrix, float *x, float *y, float *z);

// Hook

typedef void (*CCHOOKFUNC)(void *data);

typedef struct cc_hook_t
{
	CCHOOKFUNC func;
	struct cc_hook_t *next;	
} cc_hook_t;

cc_hook_t *cc_new_hook();
void cc_hook_add(cc_hook_t *hook, CCHOOKFUNC func);
void cc_hook_run(cc_hook_t *hook, void *data);

// Event

#define EVENT_CLOSEBUTTON 1
#define EVENT_KEYUP       2
#define EVENT_KEYDOWN     3
#define EVENT_KEYCHAR     4
#define EVENT_MOUSEMOVE   5
#define EVENT_MOUSEUP     6
#define EVENT_MOUSEDOWN   7

typedef struct cc_event_t
{
	int id, data, x, y;
} cc_event_t;

extern cc_hook_t *cc_event_hook;
void cc_emit_event(int id, int data, int x, int y);
void cc_startup_event();

// System

void cc_system_poll();

int cc_context_open(char *title, int width, int height);
void cc_context_close();
void cc_context_flip();
void cc_context_size(int *width, int *height);

void cc_mouse_move(int x, int y);
void cc_mouse_visible(int visible);

// 2D

typedef struct cc_atlas_t
{
	int width, height, bpp, format;
	unsigned char *data;
	unsigned int texture;
} cc_atlas_t;

typedef struct cc_texture_t
{
	cc_atlas_t *atlas;
	float left, top, right, bottom;
	int width, height;
} cc_texture_t;

cc_atlas_t *cc_load_atlas(const char *path);
void cc_atlas_set(cc_atlas_t *atlas);
void cc_atlas_upload(cc_atlas_t *atlas);
cc_texture_t *cc_atlas_texture(cc_atlas_t *atlas, int left, int top, int right, int bottom);

void cc_texture_draw(cc_texture_t *image, float x, float y);
void cc_draw_rect(float x, float y, float width, float height);

// Entity

typedef struct cc_entity_t
{
	int class;
	float px, py, pz;
	float rx, ry, rz;
	float sx, sy, sz;
	float matrix[16];
} cc_entity_t;

void cc_entity_init(cc_entity_t *ent);
void cc_entity_move(cc_entity_t *ent, float x, float y, float z);
void cc_entity_turn(cc_entity_t *ent, float pitch, float yaw, float roll);
void cc_entity_get_position(cc_entity_t *ent, float *x, float *y, float *z);
void cc_entity_set_position(cc_entity_t *ent, float x, float y, float z);
void cc_entity_get_rotation(cc_entity_t *ent, float *pitch, float *yaw, float *roll);
void cc_entity_set_rotation(cc_entity_t *ent, float pitch, float yaw, float roll);

typedef struct cc_camera_t
{
	cc_entity_t ent;
	float wrld_matrix[16];
	float proj_matrix[16];
	float r, g, b;
	float zoom, near, far;
} cc_camera_t;

void cc_camera_set_color(cc_camera_t *camera, float r, float g, float b);
void cc_camera_set_zoom(cc_camera_t *camera, float zoom);
void cc_camera_set_range(cc_camera_t *camera, float near, float far);

typedef struct cc_player_t
{

} cc_player_t;

// Blocks

typedef struct cc_block_t
{
	char *name;
	cc_texture_t *texture[6];
	float color[6][3];
} cc_block_t;

cc_block_t *cc_new_block();
void cc_block_set_name(cc_block_t *block, const char *name);
void cc_block_set_texture(cc_block_t *block, int side, cc_texture_t *texture);
void cc_block_set_color(cc_block_t *block, int side, float r, float g, float b);

// Chunk

#define CHUNKSIZE 16

#define CHUNKCUBECOUNT   (CHUNKSIZE*CHUNKSIZE*CHUNKSIZE)
#define CHUNKVERTEXCOUNT ((CHUNKSIZE + 1)*(CHUNKSIZE + 1)*(CHUNKSIZE + 1))
#define CHUNKDATASIZE    (sizeof(float)*CHUNKVERTEXCOUNT*3)
#define CHUNKVERTEXINDEX(x, y, z) ((z) * (CHUNKSIZE + 1) * (CHUNKSIZE + 1) + (y) * (CHUNKSIZE + 1) + (x))

#define CHUNK_BACK    0
#define CHUNK_FRONT   1
#define CHUNK_BOTTOM  2
#define CHUNK_TOP     3
#define CHUNK_LEFT    4
#define CHUNK_RIGHT   5

typedef struct cc_chunk_t
{
	char block[CHUNKSIZE][CHUNKSIZE][CHUNKSIZE];
	char flags[CHUNKSIZE][CHUNKSIZE][CHUNKSIZE];
	unsigned short tri_count[6];
	unsigned int pos_buffer[6], tex_buffer[6];
	unsigned int clr_buffer[6], tri_buffer[6];
	cc_octree_t *tree;
} cc_chunk_t;

cc_chunk_t *cc_new_chunk();
char cc_chunk_check(cc_chunk_t *chunk, int x, int y, int z);
void cc_chunk_render(cc_chunk_t *chunk);
void cc_chunk_set_block(cc_chunk_t *chunk, char block, int x, int y, int z);
void cc_chunk_build_tree(cc_chunk_t *chunk);
void cc_startup_chunks();

// World

typedef struct cc_world_t
{
	cc_block_t **blocks;
	cc_chunk_t **chunks;
	int block_count, chunk_count;
	
	cc_camera_t *camera;
	unsigned int texture;
} cc_world_t;

void cc_world_init(cc_world_t *world);
void cc_world_add_block(cc_world_t *world, cc_block_t *block);
void cc_world_build_chunk(cc_world_t *world, cc_chunk_t *chunk);
void cc_world_render(cc_world_t *world, int width, int height);
cc_chunk_t *cc_world_add_chunk(cc_world_t *world);

// UI

void cc_startup_ui();
void cc_ui_render();

// Networking

void cc_startup_network();

// Rendering

void cc_render_2d(int width, int height);
void cc_render_3d();
void cc_startup_render();

// Lua

extern lua_State *L;
void cc_startup_lua();

#endif
