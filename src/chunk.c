
#include <craftcore.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <IL/il.h> 
#include <math.h>

float          ccChunkPosition[6][CHUNKCUBECOUNT*4*3];
float          ccChunkTextureData[6][CHUNKCUBECOUNT*4*2];
float          ccChunkColorData[6][CHUNKCUBECOUNT*4*4];
unsigned short ccChunkIndices[6][CHUNKCUBECOUNT*2*3];
unsigned short ccChunkFaceCount[6];

cc_chunk_t *cc_new_chunk()
{
	cc_chunk_t *chunk = cc_new(chunk);
	memset(chunk, 0, sizeof(cc_chunk_t));
	return chunk;
}

char cc_chunk_check(cc_chunk_t *chunk, int x, int y, int z)
{
	if (x < 0 || x > CHUNKSIZE - 1  || y < 0 || y > CHUNKSIZE - 1 || z < 0 || z > CHUNKSIZE - 1)
		return -1;
	return chunk->block[x][y][z];
}

void cc_chunk_data_add_face(cc_block_t *block, int dir, float x, float y, float z)
{
	unsigned short cnt = ccChunkFaceCount[dir];
	float *pos = ccChunkPosition[dir], *tex = ccChunkTextureData[dir], *clr = ccChunkColorData[dir];
	unsigned short *idx = ccChunkIndices[dir];
	
	#define set(i, x, y, z) pos[(cnt*12) + (i*3)+0] = (x)*2;pos[(cnt*12) + (i*3)+ 1] = (y)*2;pos[(cnt*12) + (i*3)+2] = (z)*2;
	
	float shade = 1.0;
	switch (dir)
	{
	case CHUNK_BACK:
		shade = 0.5;
		set(3, x + 1, y + 1, z);
		set(0, x + 1, y, z);
		set(1, x, y, z);
		set(2, x, y + 1, z);
		break;
	case CHUNK_FRONT:
		shade = 0.8;
		set(0, x, y, z + 1);
		set(1, x + 1, y, z + 1);
		set(2, x + 1, y + 1, z + 1);
		set(3, x, y + 1, z + 1);
		break;
	case CHUNK_BOTTOM:
		shade = 1.0;
		set(0, x, y, z);
		set(1, x + 1, y, z);
		set(2, x + 1, y, z + 1);
		set(3, x, y, z + 1);
		break;
	case CHUNK_TOP:
		shade = 1.0;
		set(0, x + 1, y + 1, z + 1);
		set(1, x + 1, y + 1, z);
		set(2, x, y + 1, z);
		set(3, x, y + 1, z + 1);
		break;
	case CHUNK_LEFT:
		shade = 0.7;
		set(2, x, y + 1, z + 1);
		set(3, x, y + 1, z);
		set(0, x, y, z);
		set(1, x, y, z + 1);
		break;
	case CHUNK_RIGHT:
		shade = 0.6;
		set(1, x + 1, y, z);
		set(2, x + 1, y + 1, z);
		set(3, x + 1, y + 1, z + 1);
		set(0, x + 1, y, z + 1);
		break;
	}
	
	clr[cnt*16 +  0] = block->color[dir][0] * shade;
	clr[cnt*16 +  1] = block->color[dir][1] * shade;
	clr[cnt*16 +  2] = block->color[dir][2] * shade;
	clr[cnt*16 +  3] = 1.0;
	
	clr[cnt*16 +  4] = block->color[dir][0] * shade;
	clr[cnt*16 +  5] = block->color[dir][1] * shade;
	clr[cnt*16 +  6] = block->color[dir][2] * shade;
	clr[cnt*16 +  7] = 1.0;

	clr[cnt*16 +  8] = block->color[dir][0] * shade;
	clr[cnt*16 +  9] = block->color[dir][1] * shade;
	clr[cnt*16 + 10] = block->color[dir][2] * shade;
	clr[cnt*16 + 11] = 1.0;
	
	clr[cnt*16 + 12] = block->color[dir][0] * shade;
	clr[cnt*16 + 13] = block->color[dir][1] * shade;
	clr[cnt*16 + 14] = block->color[dir][2] * shade;
	clr[cnt*16 + 15] = 1.0;
	
	tex[cnt*8 + 0] = block->texture[dir]->left;
	tex[cnt*8 + 1] = block->texture[dir]->bottom;
	
	tex[cnt*8 + 2] = block->texture[dir]->right;
	tex[cnt*8 + 3] = block->texture[dir]->bottom;
	
	tex[cnt*8 + 4] = block->texture[dir]->right;
	tex[cnt*8 + 5] = block->texture[dir]->top;
	
	tex[cnt*8 + 6] = block->texture[dir]->left;
	tex[cnt*8 + 7] = block->texture[dir]->top;
	
	idx[cnt*6 + 0] = cnt*4 + 0;
	idx[cnt*6 + 1] = cnt*4 + 1;
	idx[cnt*6 + 2] = cnt*4 + 2;

	idx[cnt*6 + 3] = cnt*4 + 2;
	idx[cnt*6 + 4] = cnt*4 + 3;
	idx[cnt*6 + 5] = cnt*4 + 0;
	
	ccChunkFaceCount[dir] += 1;
}

void cc_chunk_set_block(cc_chunk_t *chunk, char block, int x, int y, int z)
{
	chunk->block[x][y][z] = block;
}

void cc_chunk_build_tree(cc_chunk_t *chunk)
{
	
}

void cc_world_build_chunk(cc_world_t *world, cc_chunk_t *chunk)
{
	for (int i = 0;i < 6;i++)
		ccChunkFaceCount[i] = 0;
		
	for (int z = 0;z < CHUNKSIZE;z++)
		for (int y = 0;y < CHUNKSIZE;y++)
			for (int x = 0;x < CHUNKSIZE;x++)
			{
				if (chunk->block[x][y][z] == 0)
					continue;

				cc_block_t *block = world->blocks[chunk->block[x][y][z] - 1];

				if (cc_chunk_check(chunk, x, y, z - 1) < 1)
					cc_chunk_data_add_face(block, CHUNK_BACK, x, y, z);
							
				if (cc_chunk_check(chunk, x, y, z + 1) < 1)
					cc_chunk_data_add_face(block, CHUNK_FRONT, x, y, z);
								
				if (cc_chunk_check(chunk, x, y - 1, z) < 1)
					cc_chunk_data_add_face(block, CHUNK_BOTTOM, x, y, z);
					
				if (cc_chunk_check(chunk, x, y + 1, z) < 1)
					cc_chunk_data_add_face(block, CHUNK_TOP, x, y, z);
								
				if (cc_chunk_check(chunk, x - 1, y, z) < 1)
					cc_chunk_data_add_face(block, CHUNK_LEFT, x, y, z);
				
				if (cc_chunk_check(chunk, x + 1, y, z) < 1)
					cc_chunk_data_add_face(block, CHUNK_RIGHT, x, y, z);
			}	

	glGenBuffersARB(6, chunk->tri_buffer);
	glGenBuffersARB(6, chunk->pos_buffer);
	glGenBuffersARB(6, chunk->tex_buffer);
	glGenBuffersARB(6, chunk->clr_buffer);	

	for (int i = 0;i < 6;i++)
	{

		chunk->tri_count[i] = ccChunkFaceCount[i];
		
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,chunk->pos_buffer[i]);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,chunk->tri_count[i]*4*3*sizeof(float),ccChunkPosition[i],GL_STATIC_DRAW_ARB);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB,chunk->clr_buffer[i]);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,sizeof(ccChunkColorData[i]),ccChunkColorData[i],GL_STATIC_DRAW_ARB);	

		glBindBufferARB(GL_ARRAY_BUFFER_ARB,chunk->tex_buffer[i]);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,sizeof(ccChunkTextureData[i]),ccChunkTextureData[i],GL_STATIC_DRAW_ARB);	
		
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,chunk->tri_buffer[i]);
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,chunk->tri_count[i]*2*3*sizeof(unsigned short),ccChunkIndices[i],GL_STATIC_DRAW_ARB);
	}
}

void cc_chunk_render(cc_chunk_t *chunk)
{
	for (int i = 0;i < 6;i++)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,chunk->tex_buffer[i]);
		glTexCoordPointer(2,GL_FLOAT,0,NULL);
		
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,chunk->clr_buffer[i]);
		glColorPointer(4,GL_FLOAT,0,NULL);
	
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,chunk->pos_buffer[i]);
		glVertexPointer(3,GL_FLOAT,0,NULL);
	
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,chunk->tri_buffer[i]);
		glDrawElements(GL_TRIANGLES,chunk->tri_count[i]*2*3,GL_UNSIGNED_SHORT,0);	
	}
}

