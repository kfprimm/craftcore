
#include <craftcore.h>

void cc_entity_refresh_matrix(cc_entity_t *ent)
{
	cc_identity_matrix(ent->matrix);
	cc_matrix_translate(ent->matrix, ent->px, ent->py, ent->pz);
	cc_matrix_rotate(ent->matrix, ent->ry, ent->rx, ent->rz);
	cc_matrix_scale(ent->matrix, ent->sx, ent->sy, ent->sz);
}

void cc_entity_init(cc_entity_t *ent)
{
	memset(ent, 0, sizeof(cc_entity_t));
	ent->sx = ent->sy = ent->sz = 1.0f;
	cc_entity_refresh_matrix(ent);
}

void cc_entity_move(cc_entity_t *ent, float x, float y, float z)
{
	float matrix[16];
	cc_identity_matrix(matrix);
	cc_matrix_rotate(matrix, ent->ry, ent->rx, ent->rz);
	cc_matrix_translate(matrix, x, y, z);
	
	cc_matrix_xyz(matrix, &x, &y, &z);
	ent->px += x;
	ent->py += y;
	ent->pz += z;
	cc_entity_refresh_matrix(ent);
}

void cc_entity_set_position(cc_entity_t *ent, float x, float y, float z)
{
	ent->px = x;
	ent->py = y;
	ent->pz = z;
	cc_entity_refresh_matrix(ent);	
}

void cc_entity_get_rotation(cc_entity_t *ent, float *pitch, float *yaw, float *roll)
{
	*pitch = ent->rx;
	*yaw   = ent->ry;
	*roll  = ent->rz;
}

void cc_entity_set_rotation(cc_entity_t *ent, float pitch, float yaw, float roll)
{
	ent->rx = pitch;
	ent->ry = yaw;
	ent->rz = roll;
	cc_entity_refresh_matrix(ent);
}

void cc_entity_turn(cc_entity_t *ent, float pitch, float yaw, float roll)
{
	ent->rx += pitch;
	ent->ry += yaw;
	ent->rz += roll;
	cc_entity_refresh_matrix(ent);
}

