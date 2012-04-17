
#include <craftcore.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <IL/il.h> 
#include <math.h>

#include <noise.h>

ccBlock **ccBlocks = NULL;

float          ccChunkPosition[6][CHUNKCUBECOUNT*4*3];
float          ccChunkTextureData[6][CHUNKCUBECOUNT*4*2];
float          ccChunkColorData[6][CHUNKCUBECOUNT*4*4];
unsigned short ccChunkIndices[6][CHUNKCUBECOUNT*2*3];
unsigned short ccChunkFaceCount[6];

ccChunk *ccNewChunk()
{
	ccChunk *chunk = ccNew(ccChunk);

	int octaves = 2;
	float freq  = 4;
	int   seed  = 80185;
	
	srand(seed);
	
	for (int z = 0;z < CHUNKSIZE;z++)
		for (int x = 0;x < CHUNKSIZE;x++)
		{
			float amp    = 1;
			float vec[2] = {((float)x/CHUNKSIZE) * freq, ((float)z/CHUNKSIZE) * freq};
			
			float result = 0.0;
			for (int i = 0; i < octaves; i++)
			{
				result += noise2(vec)*amp;
				vec[0] *= 2.0f;
				vec[1] *= 2.0f;
		    amp *= 0.5f;
			}

			result = (result * 2) + 3;
			for (int y = 0;y < CHUNKSIZE;y++)
			{
				if (y + 1 < result)
					chunk->block[x][y][z] = 1;
				else if (y < result)
					chunk->block[x][y][z] = 2;
				else
					chunk->block[x][y][z] = 0;
			}

		}
	
	return chunk;
}

char ccChunkCheck(ccChunk *chunk, int x, int y, int z)
{
	if (x < 0 || x > CHUNKSIZE - 1  || y < 0 || y > CHUNKSIZE - 1 || z < 0 || z > CHUNKSIZE - 1)
		return -1;
	return chunk->block[x][y][z];
}

void ccChunkDataAddFace(char block, int dir, float x, float y, float z)
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
	
	ccSide *side = ccBlocks[block - 1]->side[dir];
		
	clr[cnt*16 +  0] = side->r * shade;
	clr[cnt*16 +  1] = side->g * shade;
	clr[cnt*16 +  2] = side->b * shade;
	clr[cnt*16 +  3] = 1.0;
	
	clr[cnt*16 +  4] = side->r * shade;
	clr[cnt*16 +  5] = side->g * shade;
	clr[cnt*16 +  6] = side->b * shade;
	clr[cnt*16 +  7] = 1.0;

	clr[cnt*16 +  8] = side->r * shade;
	clr[cnt*16 +  9] = side->g * shade;
	clr[cnt*16 + 10] = side->b * shade;
	clr[cnt*16 + 11] = 1.0;
	
	clr[cnt*16 + 12] = side->r * shade;
	clr[cnt*16 + 13] = side->g * shade;
	clr[cnt*16 + 14] = side->b * shade;
	clr[cnt*16 + 15] = 1.0;
	
	tex[cnt*8 + 0] = side->texture->left;
	tex[cnt*8 + 1] = side->texture->bottom;
	
	tex[cnt*8 + 2] = side->texture->right;
	tex[cnt*8 + 3] = side->texture->bottom;
	
	tex[cnt*8 + 4] = side->texture->right;
	tex[cnt*8 + 5] = side->texture->top;
	
	tex[cnt*8 + 6] = side->texture->left;
	tex[cnt*8 + 7] = side->texture->top;
	
	idx[cnt*6 + 0] = cnt*4 + 0;
	idx[cnt*6 + 1] = cnt*4 + 1;
	idx[cnt*6 + 2] = cnt*4 + 2;

	idx[cnt*6 + 3] = cnt*4 + 2;
	idx[cnt*6 + 4] = cnt*4 + 3;
	idx[cnt*6 + 5] = cnt*4 + 0;
	
	ccChunkFaceCount[dir] += 1;
}

void ccChunkBuild(ccChunk *chunk)
{
	for (int i = 0;i < 6;i++)
		ccChunkFaceCount[i] = 0;
		
	for (int z = 0;z < CHUNKSIZE;z++)
		for (int y = 0;y < CHUNKSIZE;y++)
			for (int x = 0;x < CHUNKSIZE;x++)
			{
				char block = chunk->block[x][y][z];
				if (chunk->block[x][y][z] == 0)
					continue;

				if (ccChunkCheck(chunk, x, y, z - 1) < 1)
					ccChunkDataAddFace(block, CHUNK_BACK, x, y, z);
							
				if (ccChunkCheck(chunk, x, y, z + 1) < 1)
					ccChunkDataAddFace(block, CHUNK_FRONT, x, y, z);
								
				if (ccChunkCheck(chunk, x, y - 1, z) < 1)
					ccChunkDataAddFace(block, CHUNK_BOTTOM, x, y, z);
					
				if (ccChunkCheck(chunk, x, y + 1, z) < 1)
					ccChunkDataAddFace(block, CHUNK_TOP, x, y, z);
								
				if (ccChunkCheck(chunk, x - 1, y, z) < 1)
					ccChunkDataAddFace(block, CHUNK_LEFT, x, y, z);
				
				if (ccChunkCheck(chunk, x + 1, y, z) < 1)
					ccChunkDataAddFace(block, CHUNK_RIGHT, x, y, z);
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

unsigned int image, texture;

void ccChunkRender(ccChunk *chunk)
{
	glClientActiveTextureARB(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

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

void ccChunksStartup()
{

}

