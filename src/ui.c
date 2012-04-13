
#include <craftcore.h>
#include <GL/gl.h>
#include <IL/il.h>

unsigned int gui, gui_image;

void ccUIStartup()
{
	ilGenImages(1, &gui_image);
	ilBindImage(gui_image);
	
	char success = ilLoadImage((const ILstring)"mods/standard/import/gui/gui.png");
	if (success)
	{
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		if (!success)
			printf("Failed to convert image!\n");
		
		glGenTextures(1, &gui);
		glBindTexture(GL_TEXTURE_2D, gui);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
	}
	else
		printf("Failed to load!\n");
		
	ilDeleteImages(1, &gui_image); 
}

void ccDrawRect(float x, float y, float width, float height)
{
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);
	glVertex2f(x, y);
	glTexCoord2f(0,22.0/256.0);
	glVertex2f(x, y + height);
		glTexCoord2f(182.0/256.0,22.0/256.0);
	glVertex2f(x + width, y + height);
	glTexCoord2f(182.0/256.0, 0.0);
	glVertex2f(x + width, y);
	glEnd();
}

void ccRenderUI()
{
	glDisable(GL_DEPTH_TEST);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho(0, 320, 240, 0, 0, 1);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_BLEND);
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gui);
	ccDrawRect((320-182)/2,218,182,22);
	
	glDisable(GL_BLEND);
}
