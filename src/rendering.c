
#include <craftcore.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#define CHUNKCUBECOUNT   (CHUNKSIZE*CHUNKSIZE*CHUNKSIZE)
#define CHUNKVERTEXCOUNT ((CHUNKSIZE + 1)*(CHUNKSIZE + 1)*(CHUNKSIZE + 1))
#define CHUNKDATASIZE    (sizeof(float)*CHUNKVERTEXCOUNT*3)
#define CHUNKVERTEXINDEX(x, y, z) ((z) * (CHUNKSIZE + 1) * (CHUNKSIZE + 1) + (y) * (CHUNKSIZE + 1) + (x))

unsigned int   ccChunkPositionBuffer = 0;
float          ccChunkPositionData[CHUNKVERTEXCOUNT*3];
unsigned short ccChunkIndices[CHUNKCUBECOUNT*6*2*3];

ccChunk *TestChunk = NULL;

ccChunk *ccNewChunk()
{
	ccChunk *chunk = ccNew(ccChunk);
	
	for (int z = 0;z < CHUNKSIZE;z++)
		for (int y = 0;y < CHUNKSIZE;y++)
			for (int x = 0;x < CHUNKSIZE;x++)
				chunk->block[x][y][z] = 1;
				
	return chunk;
}

char ccChunkCheck(ccChunk *chunk, int x, int y, int z)
{
	if (x < 0 || x > CHUNKSIZE || y < 0 || y > CHUNKSIZE || z < 0 || z > CHUNKSIZE)
		return -1;
	return chunk->block[x][y][z];
}

void ccChunkBuild(ccChunk *chunk)
{
	if (chunk->tri_buffer == 0)
		glGenBuffersARB(1, &chunk->tri_buffer);
	
	unsigned short index = 0;
	for (int z = 0;z < CHUNKSIZE;z++)
		for (int y = 0;y < CHUNKSIZE;y++)
			for (int x = 0;x < CHUNKSIZE;x++)
			{
				if (chunk->block[x][y][z] == 0)
					continue;
				
				// Back
				if (ccChunkCheck(chunk, x, y, z - 1) < 1)
				{
					ccChunkIndices[index*3 + 0] = CHUNKVERTEXINDEX(x + 1, y + 1, z);
					ccChunkIndices[index*3 + 1] = CHUNKVERTEXINDEX(x + 1, y, z);
					ccChunkIndices[index*3 + 2] = CHUNKVERTEXINDEX(x, y, z);

					ccChunkIndices[index*3 + 3] = CHUNKVERTEXINDEX(x, y, z);
					ccChunkIndices[index*3 + 4] = CHUNKVERTEXINDEX(x, y + 1, z);
					ccChunkIndices[index*3 + 5] = CHUNKVERTEXINDEX(x + 1, y + 1, z);
					index += 2;
				}
							
				// Front
				if (ccChunkCheck(chunk, x, y, z + 1) < 1)
				{
					ccChunkIndices[index*3 + 0] = CHUNKVERTEXINDEX(x, y, z + 1);
					ccChunkIndices[index*3 + 1] = CHUNKVERTEXINDEX(x + 1, y, z + 1);
					ccChunkIndices[index*3 + 2] = CHUNKVERTEXINDEX(x + 1, y + 1, z + 1);
								
					ccChunkIndices[index*3 + 3] = CHUNKVERTEXINDEX(x + 1, y + 1, z + 1);
					ccChunkIndices[index*3 + 4] = CHUNKVERTEXINDEX(x, y + 1, z + 1);
					ccChunkIndices[index*3 + 5] = CHUNKVERTEXINDEX(x, y, z + 1);
					index += 2;
				}
								
				// Bottom
				if (ccChunkCheck(chunk, x, y - 1, z) < 1)
				{
					ccChunkIndices[index*3 + 0] = CHUNKVERTEXINDEX(x, y, z);
					ccChunkIndices[index*3 + 1] = CHUNKVERTEXINDEX(x + 1, y, z);
					ccChunkIndices[index*3 + 2] = CHUNKVERTEXINDEX(x + 1, y, z + 1);
								
					ccChunkIndices[index*3 + 3] = CHUNKVERTEXINDEX(x + 1, y, z + 1);
					ccChunkIndices[index*3 + 4] = CHUNKVERTEXINDEX(x, y, z + 1);
					ccChunkIndices[index*3 + 5] = CHUNKVERTEXINDEX(x, y, z);
					index += 2;
				}
				
				// Top
				if (ccChunkCheck(chunk, x, y + 1, z) < 1)
				{
					ccChunkIndices[index*3 + 0] = CHUNKVERTEXINDEX(x + 1, y + 1, z + 1);
					ccChunkIndices[index*3 + 1] = CHUNKVERTEXINDEX(x + 1, y + 1, z);
					ccChunkIndices[index*3 + 2] = CHUNKVERTEXINDEX(x, y + 1, z);
								
					ccChunkIndices[index*3 + 3] = CHUNKVERTEXINDEX(x, y + 1, z);
					ccChunkIndices[index*3 + 4] = CHUNKVERTEXINDEX(x, y + 1, z + 1);
					ccChunkIndices[index*3 + 5] = CHUNKVERTEXINDEX(x + 1, y + 1, z + 1);
					index += 2;
				}
								
				// Left 
				if (ccChunkCheck(chunk, x - 1, y, z) < 1)
				{
					ccChunkIndices[index*3 + 0] = CHUNKVERTEXINDEX(x, y + 1, z + 1);
					ccChunkIndices[index*3 + 1] = CHUNKVERTEXINDEX(x, y + 1, z);
					ccChunkIndices[index*3 + 2] = CHUNKVERTEXINDEX(x, y, z);
				
					ccChunkIndices[index*3 + 3] = CHUNKVERTEXINDEX(x, y, z + 1);
					ccChunkIndices[index*3 + 4] = CHUNKVERTEXINDEX(x, y + 1, z + 1);
					ccChunkIndices[index*3 + 5] = CHUNKVERTEXINDEX(x, y, z);
					index += 2;
				}
				
				// Right
				if (ccChunkCheck(chunk, x + 1, y, z) < 1)
				{
					ccChunkIndices[index*3 + 0] = CHUNKVERTEXINDEX(x, y + 1, z + 1);
					ccChunkIndices[index*3 + 1] = CHUNKVERTEXINDEX(x, y + 1, z);
					ccChunkIndices[index*3 + 2] = CHUNKVERTEXINDEX(x, y, z);
				
					ccChunkIndices[index*3 + 3] = CHUNKVERTEXINDEX(x, y, z + 1);
					ccChunkIndices[index*3 + 4] = CHUNKVERTEXINDEX(x, y + 1, z + 1);
					ccChunkIndices[index*3 + 5] = CHUNKVERTEXINDEX(x, y, z);
					index += 2;
				}
		}	
	chunk->tri_count = index;
	printf("Count = %i\n", index);

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,chunk->tri_buffer);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,chunk->tri_count,ccChunkIndices,GL_STATIC_DRAW_ARB);
}

void ccChunkRender(ccChunk *chunk)
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,ccChunkPositionBuffer);
	glVertexPointer(3,GL_FLOAT,0,0);
	
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,chunk->tri_buffer);
	glDrawElements(GL_TRIANGLES,chunk->tri_count,GL_UNSIGNED_SHORT,0);
}

void ccRenderStartup()
{
	glewInit();
	
	//glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_SCISSOR_TEST);
	
	glEnable(GL_NORMALIZE);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);

	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glAlphaFunc(GL_GEQUAL,0.5f);
	
	for (int z = 0;z < CHUNKSIZE + 1;z++)
		for (int y = 0;y < CHUNKSIZE + 1;y++)
			for (int x = 0;x < CHUNKSIZE + 1;x++)
			{
				int index = CHUNKVERTEXINDEX(x, y, z);
				ccChunkPositionData[index*3 + 0] = x*2;
				ccChunkPositionData[index*3 + 1] = y*2;
				ccChunkPositionData[index*3 + 2] = z*2;
			}
	
	glGenBuffersARB(1, &ccChunkPositionBuffer);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,ccChunkPositionBuffer);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,CHUNKDATASIZE,ccChunkPositionData,GL_STATIC_DRAW_ARB);
	
	TestChunk = ccNewChunk();
	ccChunkBuild(TestChunk);
}

float rotation = 0.0f;

void ccRender(int width, int height)
{
	glViewport(0,0,width,height);
	glScissor(0,0,width,height);
	glClearColor(0.0f, 0.5f, 0.75f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float ratio=(float)width/(float)height;
	gluPerspective(R2D(atan((1.0f/(1.0f*ratio)))*2.0f), ratio, 1, 1000);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-(CHUNKSIZE*2/2.0),0.0f,-(CHUNKSIZE*2/2.0)-CHUNKSIZE-5);
	glRotatef(rotation*4,1.0f,1.0f,0.0f);
	ccChunkRender(TestChunk);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	rotation += 0.02f;
}
