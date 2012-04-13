
#include <craftcore.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <IL/il.h> 
#include <math.h>

#include <noise.h>

ccChunk *TestChunk = NULL;

ccEntity camera;

void ccRender3D()
{
	//glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_SCISSOR_TEST);
	
	//glEnable(GL_NORMALIZE);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glAlphaFunc(GL_GEQUAL,0.5f);
}

void ccRenderStartup()
{
	glewInit();
	ccRender3D();
	ccChunksStartup();
	
	TestChunk = ccNewChunk();
	ccChunkBuild(TestChunk);
	
	ccEntityInit(&camera);
}

void ccRender(int width, int height)
{
	float matrix[16];
	
	ccRender3D();
	
	glViewport(0,0,width,height);
	glScissor(0,0,width,height);
	glClearColor(0.0f, 0.5f, 0.75f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float ratio=(float)width/(float)height;
	gluPerspective(R2D(atan((1.0f/(1.0f*ratio)))*2.0f), ratio, 1, 10000);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glMatrixMode(GL_MODELVIEW);

	ccMatrixCopy(matrix, camera.matrix);
	ccMatrixInverse(matrix);
	glLoadMatrixf(matrix);
	
	glPushMatrix();
	ccMatrixIdentity(matrix);
	ccMatrixTranslate(matrix, -CHUNKSIZE,0.0f,CHUNKSIZE);
	glMultMatrixf(matrix);
	
	ccChunkRender(TestChunk);
	glPopMatrix();
}
