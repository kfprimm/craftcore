
#include <craftcore.h>

void ccEntityRefreshMatrix(ccEntity *ent)
{
	ccMatrixIdentity(ent->matrix);
	ccMatrixTranslate(ent->matrix, ent->px, ent->py, ent->pz);
	ccMatrixRotate(ent->matrix, ent->ry, ent->rx, ent->rz);
	ccMatrixScale(ent->matrix, ent->sx, ent->sy, ent->sz);
}

void ccEntityInit(ccEntity *ent)
{
	memset(ent, 0, sizeof(ccEntity));
	ccMatrixIdentity(ent->matrix);
	ent->sx = ent->sy = ent->sz = 1.0f;
}

void ccEntityMove(ccEntity *ent, float x, float y, float z)
{
	float matrix[16];
	ccMatrixIdentity(matrix);
	ccMatrixRotate(matrix, ent->ry, ent->rx, ent->rz);
	ccMatrixTranslate(matrix, x, y, z);
	
	ccMatrixPosition(matrix, &x, &y, &z);
	ent->px += x;
	ent->py += y;
	ent->pz += z;
	ccEntityRefreshMatrix(ent);
}

void ccEntityTurn(ccEntity *ent, float pitch, float yaw, float roll)
{
	ent->rx += pitch;
	ent->ry += yaw;
	ent->rz += roll;
	ccEntityRefreshMatrix(ent);
}

