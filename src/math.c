
#include <craftcore.h>
#include <math.h>

#define _(x, y) (x * 4 + y)

float ccMatrixA[16], ccMatrixB[16], ccMatrixC[16];

float *ccMatrixIdentity(float *m)
{
	memset(m, 0, sizeof(float)*16);
	m[_(0,0)] = 1.0f;
	m[_(1,1)] = 1.0f;
	m[_(2,2)] = 1.0f;
	m[_(3,3)] = 1.0f;
	return m;
}

float *ccMatrixCopy(float *dst, float *src)
{
	memcpy(dst, src, 16*4);
	return dst;
}

float *ccMatrixPitch(float *m, float angle)
{
	ccMatrixIdentity(m);
	m[_(1,1)] = cos(D2R(angle));
	m[_(2,1)] = sin(D2R(angle));
	m[_(1,2)] = -sin(D2R(angle));
	m[_(2,2)] = cos(D2R(angle));
	return m;
}

float *ccMatrixYaw(float *m, float angle)
{
	ccMatrixIdentity(m);
	m[_(0,0)] = cos(D2R(angle));
	m[_(2,0)] = sin(D2R(angle));
	m[_(0,2)] = -sin(D2R(angle));
	m[_(2,2)] = cos(D2R(angle));
	return m;
}

float *ccMatrixRoll(float *m, float angle)
{
	ccMatrixIdentity(m);
	m[_(0,0)] = cos(D2R(angle));
	m[_(0,1)] = -sin(D2R(angle));
	m[_(1,0)] = sin(D2R(angle));
	m[_(1,1)] = cos(D2R(angle));
	return m;
}

float *ccMatrixScaling(float *m, float x, float y, float z)
{
	ccMatrixIdentity(m);
	m[_(0,0)] = x;
	m[_(1,1)] = y;
	m[_(2,2)] = z;
	return m;
}

float *ccMatrixTranslation(float *m, float x, float y, float z)
{
	ccMatrixIdentity(m);
	m[_(3,0)] = x;
	m[_(3,1)] = y;
	m[_(3,2)] = z;
	return m;
}

float *ccMatrixMultiply(float *out, float *a, float *b)
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

float ccMatrixDeterminant(float *m)
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

float *ccMatrixInvert(float *out, float *in)
{
	float d = ccMatrixDeterminant(in);
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

void ccMatrixPosition(float *m, float *x, float *y, float *z)
{
	*x = m[_(3,0)];
	*y = m[_(3,1)];
	*z = m[_(3,2)];
}

float *ccMatrixScale(float *matrix, float x, float y, float z)
{
	ccMatrixCopy(ccMatrixA, matrix);
	ccMatrixScaling(ccMatrixB, x, y, z);
	return ccMatrixMultiply(matrix, ccMatrixB, ccMatrixA);
}

float *ccMatrixRotate(float *matrix, float yaw, float pitch, float roll)
{
	ccMatrixYaw(ccMatrixA, yaw);
	ccMatrixPitch(ccMatrixB, pitch);
	ccMatrixMultiply(ccMatrixC, ccMatrixB, ccMatrixA);
	ccMatrixRoll(ccMatrixA, roll);
	ccMatrixMultiply(ccMatrixB, ccMatrixA, ccMatrixC);

	ccMatrixCopy(ccMatrixA, matrix);
	return ccMatrixMultiply(matrix, ccMatrixB, ccMatrixA);
}

float *ccMatrixTranslate(float *matrix, float x, float y, float z)
{
	ccMatrixCopy(ccMatrixA, matrix);
	ccMatrixTranslation(ccMatrixB, x, y, z);
	return ccMatrixMultiply(matrix, ccMatrixB, ccMatrixA);
}

float *ccMatrixInverse(float *matrix)
{
	ccMatrixCopy(ccMatrixA, matrix);
	return ccMatrixInvert(matrix, ccMatrixA);
}

