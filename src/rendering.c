
#include <craftcore.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

void ccRenderInit()
{
	//glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_SCISSOR_TEST);
	
	glEnable(GL_NORMALIZE);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);

	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glAlphaFunc(GL_GEQUAL,0.5f);
}

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
}
