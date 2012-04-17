
#include <craftcore.h>
#include <GL/gl.h>
#include <IL/il.h>

cc_atlas_t *cc_load_atlas(const char *path)
{
	cc_atlas_t *atlas = NULL;
	
	unsigned int image;
	ilGenImages(1, &image);
	ilBindImage(image);
	
	if (ilLoadImage((const ILstring)path))
		if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
		{
			atlas = cc_new(atlas);
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

void cc_atlas_upload(cc_atlas_t *atlas)
{
	glGenTextures(1, &atlas->texture);
	glBindTexture(GL_TEXTURE_2D, atlas->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, atlas->bpp, atlas->width, atlas->height, 0, atlas->format, GL_UNSIGNED_BYTE, atlas->data);
}

cc_texture_t *cc_atlas_texture(cc_atlas_t *atlas, int left, int top, int right, int bottom)
{
	cc_texture_t *texture = cc_new(texture);
	texture->atlas  = atlas;
	texture->left   = (float)left/atlas->width;
	texture->top    = (float)top/atlas->height;
	texture->right  = (float)right/atlas->width;
	texture->bottom = (float)bottom/atlas->height;
	texture->width  = right - left;
	texture->height = bottom - top;
	return texture;
}

void cc_startup_ui()
{
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
		printf("BAD IL VERSION!\n");
	
	ilInit();
}

void cc_draw_rect(float x, float y, float width, float height)
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

void cc_texture_draw(cc_texture_t *texture, float x, float y)
{
	if (texture->atlas->texture == 0)
		cc_atlas_upload(texture->atlas);
		
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->atlas->texture);
	
	glBegin(GL_QUADS);
	glTexCoord2f(texture->left,texture->top);
	glVertex2f(x, y);
	glTexCoord2f(texture->left,texture->bottom);
	glVertex2f(x, y + texture->height);
	glTexCoord2f(texture->right,texture->bottom);
	glVertex2f(x + texture->width, y + texture->height);
	glTexCoord2f(texture->right, texture->top);
	glVertex2f(x + texture->width, y);
	glEnd();	
}

void cc_ui_render()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor3f(1,1,1);
	
	/*lua_getglobal(L, "cm");
	lua_getfield(L, -1, "renderHUD");
	lua_call(L, 0, 0);
	lua_pop(L, 1);*/
	
	glDisable(GL_BLEND);
}
