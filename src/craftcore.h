
#ifndef CRAFTCORE_H
#define CRAFTCORE_H

#include <stdlib.h>
#include <string.h>

#include <keycodes.h>

#define min(v0, v1) ((v0) > (v1) ? (v1) : (v0))
#define max(v0, v1) ((v0) > (v1) ? (v0) : (v1))

#define FALSE 0
#define TRUE  1
#define ccNew(class) ((class*)malloc(sizeof(class)))

// Math

#define PI 3.14159265
#define D2R(v) ((v)*(PI/180.0f))
#define R2D(v) ((v)*(180.0f/PI))

float *ccMatrixIdentity(float *matrix);
float *ccMatrixCopy(float *dst, float *src);
float *ccMatrixMultiply(float *out, float *a, float *b);
float *ccMatrixTranslation(float *matrix, float x, float y, float z);
float *ccMatrixTranslate(float *matrix, float x, float y, float z);
float *ccMatrixInvert(float *out, float *in);
float *ccMatrixInverse(float *matrix);
float *ccMatrixScaling(float *matrix, float x, float y, float z);
float *ccMatrixScale(float *matrix, float x, float y, float z);
float *ccMatrixRotate(float *matrix, float yaw, float pitch, float roll);
void   ccMatrixPosition(float *matrix, float *x, float *y, float *z);

// Hook

typedef void (*CCHOOKFUNC)(void *data);

typedef struct ccHook
{
	CCHOOKFUNC func;
	struct ccHook *next;	
} ccHook;

ccHook *ccNewHook();
void ccHookAdd(ccHook *hook, CCHOOKFUNC func);
void ccHookRun(ccHook *hook, void *data);

// Event

#define EVENT_CLOSEBUTTON 1
#define EVENT_KEYUP       2
#define EVENT_KEYDOWN     3
#define EVENT_MOUSEMOVE   4
#define EVENT_MOUSEUP     5
#define EVENT_MOUSEDOWN   6

typedef struct ccEvent
{
	int id, data, x, y;
} ccEvent;

extern ccHook *ccEventHook;
void ccEmitEvent(int id, int data, int x, int y);
void ccEventStartup();

// System

void ccPollSystem();
int ccOpenContext(char *title, int width, int height);
int ccContextWidth();
int ccContextHeight();
void ccCloseContext();
void ccFlip();
void ccMoveMouse(int x, int y);
void ccMouseVisible(int visible);

// Entity

typedef struct ccEntity
{
	int class;
	float px, py, pz;
	float rx, ry, rz;
	float sx, sy, sz;
	float matrix[16];
} ccEntity;

void ccEntityInit(ccEntity *ent);
void ccEntityMove(ccEntity *ent, float x, float y, float z);
void ccEntityPosition(ccEntity *ent, float x, float y, float z);
void ccEntityTurn(ccEntity *ent, float pitch, float yaw, float roll);
void ccEntityRotation(ccEntity *ent, float *pitch, float *yaw, float *roll);
void ccEntityRotate(ccEntity *ent, float pitch, float yaw, float roll);

typedef struct ccPlayer
{
	ccEntity ent;
	float world_matrix[16];
} ccPlayer;

// Blocks

typedef struct ccTextureUV
{
	float left, top, right, bottom;
} ccTextureUV;
																
typedef struct ccBlock
{
	char *name;
	ccTextureUV *uv[6];
} ccBlock;

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

typedef struct ccChunk
{
	char block[CHUNKSIZE][CHUNKSIZE][CHUNKSIZE];
	char flags[CHUNKSIZE][CHUNKSIZE][CHUNKSIZE];
	unsigned short tri_count[6];
	unsigned int pos_buffer[6], tex_buffer[6], clr_buffer[6], tri_buffer[6];
} ccChunk;

ccChunk *ccNewChunk();
char ccChunkCheck(ccChunk *chunk, int x, int y, int z);
void ccChunkBuild(ccChunk *chunk);
void ccChunkRender(ccChunk *chunk);
void ccChunksStartup();

// World

typedef struct ccWorld
{

} ccWorld;

// Rendering

void ccRenderStartup();
void ccRender(int width, int height);

#endif
