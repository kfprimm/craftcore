
#include <craftcore.h>
#include <string.h>

cc_block_t *cc_new_block()
{
	cc_block_t *block = cc_new(block);
	memset(block, 0, sizeof(cc_block_t));
	for (int i = 0;i < 6;i++)
	{
		block->color[i][0] = 1.0;
		block->color[i][1] = 1.0;
		block->color[i][2] = 1.0;
	}
	return block;
}

void cc_block_set_name(cc_block_t *block, const char *name)
{
	block->name = strdup(name);
}

void cc_block_set_texture(cc_block_t *block, int side, cc_texture_t *texture)
{
	block->texture[side] = texture;
}

void cc_block_set_color(cc_block_t *block, int side, float r, float g, float b)
{
	block->color[side][0] = r;
	block->color[side][1] = g;
	block->color[side][2] = b;
}
