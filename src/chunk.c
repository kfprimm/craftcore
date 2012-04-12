
#include <craftcore.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <IL/il.h> 
#include <math.h>

#include <noise.h>

unsigned int   ccChunkPositionBuffer, ccChunkTextureBuffer;
float          ccChunkPositionData[CHUNKVERTEXCOUNT*3];
float          ccChunkTextureData[CHUNKVERTEXCOUNT*2];
unsigned short ccChunkIndices[CHUNKCUBECOUNT*6*2*3];

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
				if (y < result)
					chunk->block[x][y][z] = 1;
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
					ccChunkIndices[index*3 + 0] = CHUNKVERTEXINDEX(x + 1, y, z);
					ccChunkIndices[index*3 + 1] = CHUNKVERTEXINDEX(x + 1, y + 1, z);
					ccChunkIndices[index*3 + 2] = CHUNKVERTEXINDEX(x + 1, y + 1, z + 1);

					ccChunkIndices[index*3 + 3] = CHUNKVERTEXINDEX(x + 1, y, z);
					ccChunkIndices[index*3 + 4] = CHUNKVERTEXINDEX(x + 1, y + 1, z + 1);
					ccChunkIndices[index*3 + 5] = CHUNKVERTEXINDEX(x + 1, y, z + 1);
					
					index += 2;
				}
		}	
	chunk->tri_count = index;

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,chunk->tri_buffer);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,sizeof(ccChunkIndices),ccChunkIndices,GL_STATIC_DRAW_ARB);
}

unsigned int image, texture;

void ccChunkRender(ccChunk *chunk)
{
	glClientActiveTextureARB(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB,ccChunkTextureBuffer);
	glTexCoordPointer(2,GL_FLOAT,0,NULL);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB,ccChunkPositionBuffer);
	glVertexPointer(3,GL_FLOAT,0,NULL);

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,chunk->tri_buffer);
	glDrawElements(GL_TRIANGLES,chunk->tri_count*3,GL_UNSIGNED_SHORT,0);	
}

void ccChunksStartup()
{
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
		printf("BAD IL VERSION!\n");
	
	ilInit();
	ilGenImages(1, &image);
	ilBindImage(image);
	
	char success = ilLoadImage((const ILstring)"mods/standard/textures/dirt.png");
	if (success)
	{
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		if (!success)
			printf("Failed to convert image!\n");
		
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
	}
	else
		printf("Failed to load!\n");
	ilDeleteImages(1, &image); 
		
	for (int z = 0;z < CHUNKSIZE + 1;z++)
		for (int y = 0;y < CHUNKSIZE + 1;y++)
			for (int x = 0;x < CHUNKSIZE + 1;x++)
			{
				int index = CHUNKVERTEXINDEX(x, y, z);
				ccChunkPositionData[index*3 + 0] = x*2;
				ccChunkPositionData[index*3 + 1] = y*2;
				ccChunkPositionData[index*3 + 2] = z*2;
				
				ccChunkTextureData[index*2 + 0] = x;
				ccChunkTextureData[index*2 + 1] = y;
			}

	glGenBuffersARB(1, &ccChunkPositionBuffer);
	glGenBuffersARB(1, &ccChunkTextureBuffer);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,ccChunkPositionBuffer);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,sizeof(ccChunkPositionData),ccChunkPositionData,GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB,ccChunkTextureBuffer);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,sizeof(ccChunkTextureData),ccChunkTextureData,GL_STATIC_DRAW_ARB);	
}

