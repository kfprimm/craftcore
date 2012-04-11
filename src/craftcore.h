
#ifndef CRAFTCORE_H
#define CRAFTCORE_H

#include <stdlib.h>
#include <string.h>

#include <keycodes.h>

#define PI 3.14159265
#define D2R(v) ((v)*(PI/180.0f))
#define R2D(v) ((v)*(180.0f/PI))

#define FALSE 0
#define TRUE  1
#define ccNew(class) ((class*)malloc(sizeof(class)))

// hook.c

typedef void (*CCHOOKFUNC)(void *data);

typedef struct ccHook
{
	CCHOOKFUNC func;
	struct ccHook *next;	
} ccHook;

ccHook *ccNewHook();
void ccHookAdd(ccHook *hook, CCHOOKFUNC func);
void ccHookRun(ccHook *hook, void *data);

// event.c

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

// system.c

void ccPollSystem();
int ccOpenContext(char *title, int width, int height);
int ccContextWidth();
int ccContextHeight();
void ccCloseContext();
void ccFlip();

// world.c

#define CHUNKSIZE 16

typedef struct ccChunk
{
	char block[CHUNKSIZE][CHUNKSIZE][CHUNKSIZE];
	char flags[CHUNKSIZE][CHUNKSIZE][CHUNKSIZE];
	unsigned short tri_count;
	unsigned int pos_buffer, tri_buffer;
} ccChunk;

typedef struct ccWorld
{

} ccWorld;

// rendering.c

void ccRenderStartup();
void ccRender(int width, int height);

#endif
