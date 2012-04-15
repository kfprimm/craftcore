
#include <craftcore.h>
#include <GL/gl.h>
#include <IL/il.h>

ccAtlas *ccLoadAtlas(const char *path)
{
	ccAtlas *atlas = NULL;
	
	unsigned int image;
	ilGenImages(1, &image);
	ilBindImage(image);
	
	if (ilLoadImage((const ILstring)path))
		if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
		{
			atlas = ccNew(ccAtlas);
			atlas->width  = ilGetInteger(IL_IMAGE_WIDTH);
			atlas->height = ilGetInteger(IL_IMAGE_HEIGHT);
			atlas->bpp    = ilGetInteger(IL_IMAGE_BPP);
			atlas->format = ilGetInteger(IL_IMAGE_FORMAT);
			atlas->data   = (unsigned char *)malloc(atlas->width*atlas->height*atlas->bpp);
			memcpy(atlas->data, ilGetData(), atlas->width*atlas->height*atlas->bpp);

			atlas->texture = 0;
		}
		
	ilDeleteImages(1, &image);
	return atlas;
}

void ccAtlasUpload(ccAtlas *atlas)
{
	glGenTextures(1, &atlas->texture);
	glBindTexture(GL_TEXTURE_2D, atlas->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, atlas->bpp, atlas->width, atlas->height, 0, atlas->format, GL_UNSIGNED_BYTE, atlas->data);
}

ccImage *ccAtlasImage(ccAtlas *atlas, int left, int top, int right, int bottom)
{
	ccImage *image = ccNew(ccImage);
	image->atlas  = atlas;
	image->left   = (float)left/atlas->width;
	image->top    = (float)top/atlas->height;
	image->right  = (float)right/atlas->width;
	image->bottom = (float)bottom/atlas->height;
	image->width  = right - left;
	image->height = bottom - top;
	return image;
}

void ccUIStartup()
{

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

void ccDrawImage(ccImage *image, float x, float y)
{
	if (image->atlas->texture == 0)
		ccAtlasUpload(image->atlas);
		
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, image->atlas->texture);
	
	glBegin(GL_QUADS);
	glTexCoord2f(image->left,image->top);
	glVertex2f(x, y);
	glTexCoord2f(image->left,image->bottom);
	glVertex2f(x, y + image->height);
	glTexCoord2f(image->right,image->bottom);
	glVertex2f(x + image->width, y + image->height);
	glTexCoord2f(image->right, image->top);
	glVertex2f(x + image->width, y);
	glEnd();	
}

void ccRenderUI()
{
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 640, 480, 0, 0, 1);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor3f(1,1,1);
	
	lua_getglobal(L, "cm");
	lua_getfield(L, -1, "renderHUD");
	lua_call(L, 0, 0);
	lua_pop(L, 1);
	
	glDisable(GL_BLEND);
}
