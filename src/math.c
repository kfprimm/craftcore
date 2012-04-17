
#include <craftcore.h>
#include <math.h>

#define _(x, y) (x * 4 + y)

float ccMatrixA[16], ccMatrixB[16], ccMatrixC[16];

float *cc_identity_matrix(float *m)
{
	memset(m, 0, sizeof(float)*16);
	m[_(0,0)] = 1.0f;
	m[_(1,1)] = 1.0f;
	m[_(2,2)] = 1.0f;
	m[_(3,3)] = 1.0f;
	return m;
}

float *cc_matrix_copy(float *dst, float *src)
{
	memcpy(dst, src, 16*4);
	return dst;
}

float *cc_pitch_matrix(float *m, float angle)
{
	cc_identity_matrix(m);
	m[_(1,1)] = cos(D2R(angle));
	m[_(2,1)] = sin(D2R(angle));
	m[_(1,2)] = -sin(D2R(angle));
	m[_(2,2)] = cos(D2R(angle));
	return m;
}

float *cc_yaw_matrix(float *m, float angle)
{
	cc_identity_matrix(m);
	m[_(0,0)] = cos(D2R(angle));
	m[_(2,0)] = sin(D2R(angle));
	m[_(0,2)] = -sin(D2R(angle));
	m[_(2,2)] = cos(D2R(angle));
	return m;
}

float *cc_roll_matrix(float *m, float angle)
{
	cc_identity_matrix(m);
	m[_(0,0)] = cos(D2R(angle));
	m[_(0,1)] = -sin(D2R(angle));
	m[_(1,0)] = sin(D2R(angle));
	m[_(1,1)] = cos(D2R(angle));
	return m;
}

float *cc_scale_matrix(float *m, float x, float y, float z)
{
	cc_identity_matrix(m);
	m[_(0,0)] = x;
	m[_(1,1)] = y;
	m[_(2,2)] = z;
	return m;
}

float *cc_translation_matrix(float *m, float x, float y, float z)
{
	cc_identity_matrix(m);
	m[_(3,0)] = x;
	m[_(3,1)] = y;
	m[_(3,2)] = z;
	return m;
}

float *cc_matrix_multiply(float *out, float *a, float *b)
{
	out[_(0,0)] = a[_(0,0)] * b[_(0,0)] + a[_(0,1)] * b[_(1,0)] + a[_(0,2)] * b[_(2,0)] + a[_(0,3)] * b[_(3,0)];
	out[_(0,1)] = a[_(0,0)] * b[_(0,1)] + a[_(0,1)] * b[_(1,1)] + a[_(0,2)] * b[_(2,1)] + a[_(0,3)] * b[_(3,1)];
	out[_(0,2)] = a[_(0,0)] * b[_(0,2)] + a[_(0,1)] * b[_(1,2)] + a[_(0,2)] * b[_(2,2)] + a[_(0,3)] * b[_(3,2)];
	out[_(0,3)] = a[_(0,0)] * b[_(0,3)] + a[_(0,1)] * b[_(1,3)] + a[_(0,2)] * b[_(2,3)] + a[_(0,3)] * b[_(3,3)];
	out[_(1,0)] = a[_(1,0)] * b[_(0,0)] + a[_(1,1)] * b[_(1,0)] + a[_(1,2)] * b[_(2,0)] + a[_(1,3)] * b[_(3,0)];
	out[_(1,1)] = a[_(1,0)] * b[_(0,1)] + a[_(1,1)] * b[_(1,1)] + a[_(1,2)] * b[_(2,1)] + a[_(1,3)] * b[_(3,1)];
	out[_(1,2)] = a[_(1,0)] * b[_(0,2)] + a[_(1,1)] * b[_(1,2)] + a[_(1,2)] * b[_(2,2)] + a[_(1,3)] * b[_(3,2)];
	out[_(1,3)] = a[_(1,0)] * b[_(0,3)] + a[_(1,1)] * b[_(1,3)] + a[_(1,2)] * b[_(2,3)] + a[_(1,3)] * b[_(3,3)];
	out[_(2,0)] = a[_(2,0)] * b[_(0,0)] + a[_(2,1)] * b[_(1,0)] + a[_(2,2)] * b[_(2,0)] + a[_(2,3)] * b[_(3,0)];
	out[_(2,1)] = a[_(2,0)] * b[_(0,1)] + a[_(2,1)] * b[_(1,1)] + a[_(2,2)] * b[_(2,1)] + a[_(2,3)] * b[_(3,1)];
	out[_(2,2)] = a[_(2,0)] * b[_(0,2)] + a[_(2,1)] * b[_(1,2)] + a[_(2,2)] * b[_(2,2)] + a[_(2,3)] * b[_(3,2)];
	out[_(2,3)] = a[_(2,0)] * b[_(0,3)] + a[_(2,1)] * b[_(1,3)] + a[_(2,2)] * b[_(2,3)] + a[_(2,3)] * b[_(3,3)];
	out[_(3,0)] = a[_(3,0)] * b[_(0,0)] + a[_(3,1)] * b[_(1,0)] + a[_(3,2)] * b[_(2,0)] + a[_(3,3)] * b[_(3,0)];
	out[_(3,1)] = a[_(3,0)] * b[_(0,1)] + a[_(3,1)] * b[_(1,1)] + a[_(3,2)] * b[_(2,1)] + a[_(3,3)] * b[_(3,1)];
	out[_(3,2)] = a[_(3,0)] * b[_(0,2)] + a[_(3,1)] * b[_(1,2)] + a[_(3,2)] * b[_(2,2)] + a[_(3,3)] * b[_(3,2)];
	out[_(3,3)] = a[_(3,0)] * b[_(0,3)] + a[_(3,1)] * b[_(1,3)] + a[_(3,2)] * b[_(2,3)] + a[_(3,3)] * b[_(3,3)];
	return out;
}

float cc_matrix_determinant(float *m)
{
	return	m[_(0,0)]*m[_(1,1)]*m[_(2,2)]*m[_(3,3)] + m[_(0,0)]*m[_(1,2)]*m[_(2,3)]*m[_(3,1)] + m[_(0,0)]*m[_(1,3)]*m[_(2,1)]*m[_(3,2)] +
					m[_(0,1)]*m[_(1,0)]*m[_(2,3)]*m[_(3,2)] + m[_(0,1)]*m[_(1,2)]*m[_(2,0)]*m[_(3,3)] + m[_(0,1)]*m[_(1,3)]*m[_(2,2)]*m[_(3,0)] +
					m[_(0,2)]*m[_(1,0)]*m[_(2,1)]*m[_(3,3)] + m[_(0,2)]*m[_(1,1)]*m[_(2,3)]*m[_(3,0)] + m[_(0,2)]*m[_(1,3)]*m[_(2,0)]*m[_(3,1)] +
					m[_(0,3)]*m[_(1,0)]*m[_(2,2)]*m[_(3,1)] + m[_(0,3)]*m[_(1,1)]*m[_(2,0)]*m[_(3,2)] + m[_(0,3)]*m[_(1,2)]*m[_(2,1)]*m[_(3,0)] -
					m[_(0,0)]*m[_(1,1)]*m[_(2,3)]*m[_(3,2)] - m[_(0,0)]*m[_(1,2)]*m[_(2,1)]*m[_(3,3)] - m[_(0,0)]*m[_(1,3)]*m[_(2,2)]*m[_(3,1)] -
					m[_(0,1)]*m[_(1,0)]*m[_(2,2)]*m[_(3,3)] - m[_(0,1)]*m[_(1,2)]*m[_(2,3)]*m[_(3,0)] - m[_(0,1)]*m[_(1,3)]*m[_(2,0)]*m[_(3,2)] -
					m[_(0,2)]*m[_(1,0)]*m[_(2,3)]*m[_(3,1)] - m[_(0,2)]*m[_(1,1)]*m[_(2,0)]*m[_(3,3)] - m[_(0,2)]*m[_(1,3)]*m[_(2,1)]*m[_(3,0)] -
					m[_(0,3)]*m[_(1,0)]*m[_(2,1)]*m[_(3,2)] - m[_(0,3)]*m[_(1,1)]*m[_(2,2)]*m[_(3,0)] - m[_(0,3)]*m[_(1,2)]*m[_(2,0)]*m[_(3,1)];
}

float *cc_matrix_invert(float *out, float *in)
{
	float d = cc_matrix_determinant(in);
	if (d == 0) return NULL;
	d = 1.0f/d;
	out[_(0,0)] = (in[_(1,1)]*in[_(2,2)]*in[_(3,3)] + in[_(1,2)]*in[_(2,3)]*in[_(3,1)] + in[_(1,3)]*in[_(2,1)]*in[_(3,2)] - in[_(1,1)]*in[_(2,3)]*in[_(3,2)] - in[_(1,2)]*in[_(2,1)]*in[_(3,3)] - in[_(1,3)]*in[_(2,2)]*in[_(3,1)])*d;
	out[_(0,1)] = (in[_(0,1)]*in[_(2,3)]*in[_(3,2)] + in[_(0,2)]*in[_(2,1)]*in[_(3,3)] + in[_(0,3)]*in[_(2,2)]*in[_(3,1)] - in[_(0,1)]*in[_(2,2)]*in[_(3,3)] - in[_(0,2)]*in[_(2,3)]*in[_(3,1)] - in[_(0,3)]*in[_(2,1)]*in[_(3,2)])*d;
	out[_(0,2)] = (in[_(0,1)]*in[_(1,2)]*in[_(3,3)] + in[_(0,2)]*in[_(1,3)]*in[_(3,1)] + in[_(0,3)]*in[_(1,1)]*in[_(3,2)] - in[_(0,1)]*in[_(1,3)]*in[_(3,2)] - in[_(0,2)]*in[_(1,1)]*in[_(3,3)] - in[_(0,3)]*in[_(1,2)]*in[_(3,1)])*d;
	out[_(0,3)] = (in[_(0,1)]*in[_(1,3)]*in[_(2,2)] + in[_(0,2)]*in[_(1,1)]*in[_(2,3)] + in[_(0,3)]*in[_(1,2)]*in[_(2,1)] - in[_(0,1)]*in[_(1,2)]*in[_(2,3)] - in[_(0,2)]*in[_(1,3)]*in[_(2,1)] - in[_(0,3)]*in[_(1,1)]*in[_(2,2)])*d;
	out[_(1,0)] = (in[_(1,0)]*in[_(2,3)]*in[_(3,2)] + in[_(1,2)]*in[_(2,0)]*in[_(3,3)] + in[_(1,3)]*in[_(2,2)]*in[_(3,0)] - in[_(1,0)]*in[_(2,2)]*in[_(3,3)] - in[_(1,2)]*in[_(2,3)]*in[_(3,0)] - in[_(1,3)]*in[_(2,0)]*in[_(3,2)])*d;
	out[_(1,1)] = (in[_(0,0)]*in[_(2,2)]*in[_(3,3)] + in[_(0,2)]*in[_(2,3)]*in[_(3,0)] + in[_(0,3)]*in[_(2,0)]*in[_(3,2)] - in[_(0,0)]*in[_(2,3)]*in[_(3,2)] - in[_(0,2)]*in[_(2,0)]*in[_(3,3)] - in[_(0,3)]*in[_(2,2)]*in[_(3,0)])*d;
	out[_(1,2)] = (in[_(0,0)]*in[_(1,3)]*in[_(3,2)] + in[_(0,2)]*in[_(1,0)]*in[_(3,3)] + in[_(0,3)]*in[_(1,2)]*in[_(3,0)] - in[_(0,0)]*in[_(1,2)]*in[_(3,3)] - in[_(0,2)]*in[_(1,3)]*in[_(3,0)] - in[_(0,3)]*in[_(1,0)]*in[_(3,2)])*d;
	out[_(1,3)] = (in[_(0,0)]*in[_(1,2)]*in[_(2,3)] + in[_(0,2)]*in[_(1,3)]*in[_(2,0)] + in[_(0,3)]*in[_(1,0)]*in[_(2,2)] - in[_(0,0)]*in[_(1,3)]*in[_(2,2)] - in[_(0,2)]*in[_(1,0)]*in[_(2,3)] - in[_(0,3)]*in[_(1,2)]*in[_(2,0)])*d;
	out[_(2,0)] = (in[_(1,0)]*in[_(2,1)]*in[_(3,3)] + in[_(1,1)]*in[_(2,3)]*in[_(3,0)] + in[_(1,3)]*in[_(2,0)]*in[_(3,1)] - in[_(1,0)]*in[_(2,3)]*in[_(3,1)] - in[_(1,1)]*in[_(2,0)]*in[_(3,3)] - in[_(1,3)]*in[_(2,1)]*in[_(3,0)])*d;
	out[_(2,1)] = (in[_(0,0)]*in[_(2,3)]*in[_(3,1)] + in[_(0,1)]*in[_(2,0)]*in[_(3,3)] + in[_(0,3)]*in[_(2,1)]*in[_(3,0)] - in[_(0,0)]*in[_(2,1)]*in[_(3,3)] - in[_(0,1)]*in[_(2,3)]*in[_(3,0)] - in[_(0,3)]*in[_(2,0)]*in[_(3,1)])*d;
	out[_(2,2)] = (in[_(0,0)]*in[_(1,1)]*in[_(3,3)] + in[_(0,1)]*in[_(1,3)]*in[_(3,0)] + in[_(0,3)]*in[_(1,0)]*in[_(3,1)] - in[_(0,0)]*in[_(1,3)]*in[_(3,1)] - in[_(0,1)]*in[_(1,0)]*in[_(3,3)] - in[_(0,3)]*in[_(1,1)]*in[_(3,0)])*d;
	out[_(2,3)] = (in[_(0,0)]*in[_(1,3)]*in[_(2,1)] + in[_(0,1)]*in[_(1,0)]*in[_(2,3)] + in[_(0,3)]*in[_(1,1)]*in[_(2,0)] - in[_(0,0)]*in[_(1,1)]*in[_(2,3)] - in[_(0,1)]*in[_(1,3)]*in[_(2,0)] - in[_(0,3)]*in[_(1,0)]*in[_(2,1)])*d;
	out[_(3,0)] = (in[_(1,0)]*in[_(2,2)]*in[_(3,1)] + in[_(1,1)]*in[_(2,0)]*in[_(3,2)] + in[_(1,2)]*in[_(2,1)]*in[_(3,0)] - in[_(1,0)]*in[_(2,1)]*in[_(3,2)] - in[_(1,1)]*in[_(2,2)]*in[_(3,0)] - in[_(1,2)]*in[_(2,0)]*in[_(3,1)])*d;
	out[_(3,1)] = (in[_(0,0)]*in[_(2,1)]*in[_(3,2)] + in[_(0,1)]*in[_(2,2)]*in[_(3,0)] + in[_(0,2)]*in[_(2,0)]*in[_(3,1)] - in[_(0,0)]*in[_(2,2)]*in[_(3,1)] - in[_(0,1)]*in[_(2,0)]*in[_(3,2)] - in[_(0,2)]*in[_(2,1)]*in[_(3,0)])*d;
	out[_(3,2)] = (in[_(0,0)]*in[_(1,2)]*in[_(3,1)] + in[_(0,1)]*in[_(1,0)]*in[_(3,2)] + in[_(0,2)]*in[_(1,1)]*in[_(3,0)] - in[_(0,0)]*in[_(1,1)]*in[_(3,2)] - in[_(0,1)]*in[_(1,2)]*in[_(3,0)] - in[_(0,2)]*in[_(1,0)]*in[_(3,1)])*d;
	out[_(3,3)] = (in[_(0,0)]*in[_(1,1)]*in[_(2,2)] + in[_(0,1)]*in[_(1,2)]*in[_(2,0)] + in[_(0,2)]*in[_(1,0)]*in[_(2,1)] - in[_(0,0)]*in[_(1,2)]*in[_(2,1)] - in[_(0,1)]*in[_(1,0)]*in[_(2,2)] - in[_(0,2)]*in[_(1,1)]*in[_(2,0)])*d;	
	return out;
}

void cc_matrix_xyz(float *m, float *x, float *y, float *z)
{
	*x = m[_(3,0)];
	*y = m[_(3,1)];
	*z = m[_(3,2)];
}

float *cc_matrix_scale(float *matrix, float x, float y, float z)
{
	cc_matrix_copy(ccMatrixA, matrix);
	cc_scale_matrix(ccMatrixB, x, y, z);
	return cc_matrix_multiply(matrix, ccMatrixB, ccMatrixA);
}

float *cc_matrix_rotate(float *matrix, float yaw, float pitch, float roll)
{
	cc_yaw_matrix(ccMatrixA, yaw);
	cc_pitch_matrix(ccMatrixB, pitch);
	cc_matrix_multiply(ccMatrixC, ccMatrixB, ccMatrixA);
	cc_roll_matrix(ccMatrixA, roll);
	cc_matrix_multiply(ccMatrixB, ccMatrixA, ccMatrixC);

	cc_matrix_copy(ccMatrixA, matrix);
	return cc_matrix_multiply(matrix, ccMatrixB, ccMatrixA);
}

float *cc_matrix_translate(float *matrix, float x, float y, float z)
{
	cc_matrix_copy(ccMatrixA, matrix);
	cc_translation_matrix(ccMatrixB, x, y, z);
	return cc_matrix_multiply(matrix, ccMatrixB, ccMatrixA);
}

float *cc_matrix_inverse(float *matrix)
{
	cc_matrix_copy(ccMatrixA, matrix);
	return cc_matrix_invert(matrix, ccMatrixA);
}

