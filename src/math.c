
#include <craftcore.h>
#include <math.h>

void cc_octree_print(cc_octree_t *tree, int depth)
{
	int leaf = TRUE;
	for (int z = 0;z < 2;z++)
		for (int y = 0;y < 2;y++)
			for (int x = 0;x < 2;x++)
				if (tree->child[x][y][z] != NULL)
					leaf = FALSE;
	
	printf("%*s", depth, " ");
	if (leaf)
		printf("LEAF\n");
	else
		printf("BRANCH\n");
	
	for (int z = 0;z < 2;z++)
		for (int y = 0;y < 2;y++)
			for (int x = 0;x < 2;x++)
				if (tree->child[x][y][z] != NULL)
					cc_octree_print(tree->child[x][y][z], depth + 1);
}

void cc_octree_init(cc_octree_t *tree)
{
	memset(tree, 0, sizeof(cc_octree_t));
}

void cc_octree_free(cc_octree_t *tree)
{
	for (int z = 0;z < 2;z++)
		for (int y = 0;y < 2;y++)
			for (int x = 0;x < 2;x++)
				if (tree->child[x][y][z])
					cc_octree_free(tree->child[x][y][z]);
}

int cc_octree_line_intersection(cc_octree_t *tree, cc_line_t *line, float *t, void **data)
{
	*data = tree->data;
	if (!cc_box_line_intersection(&tree->bounds, line, t))
		return 0;

	cc_vec3_t newhit;
	float newt = 2.0, r;	
	int parent = FALSE;
	void *dat = NULL;
	
	for (int z = 0;z < 2;z++)
		for (int y = 0;y < 2;y++)
			for (int x = 0;x < 2;x++)
				if (tree->child[x][y][z] != NULL)
				{
					parent = TRUE;
					void *tmp = NULL;
					if (cc_octree_line_intersection(tree->child[x][y][z], line, &r, &tmp))
						if (r < newt)
						{
							newt = r;
							dat = tmp;
						}
				}
				
	if (parent)
	{
		if (newt == 2.0f)
			return 0;
		*t = newt;
		*data = dat;
	}
		
	return 1;
}

void cc_vec3_add(float *out, float *a, float *b)
{
	out[0] = a[0] + b[0];
	out[1] = a[1] + b[1];
	out[2] = a[2] + b[2];
}

void cc_vec3_sub(float *out, float *a, float *b)
{
	out[0] = a[0] - b[0];
	out[1] = a[1] - b[1];
	out[2] = a[2] - b[2];
}

void cc_vec3_scale(float *out, float *vec, float factor)
{
	out[0] = vec[0] * factor;
	out[1] = vec[1] * factor;
	out[2] = vec[2] * factor;
}

float cc_vec3_length(float *vec)
{
	return sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
}

float cc_vec3_distance(float *vec1, float *vec2)
{
	float vec[3];
	cc_vec3_sub(vec, vec2, vec1);
	return cc_vec3_length(vec);
}

void cc_line_point(cc_line_t *line, float t, cc_vec3_t *out)
{
	float diff[3];
	cc_vec3_sub(diff, &line->p1, &line->p0);
	cc_vec3_scale(diff, diff, t);
	cc_vec3_add(out, &line->p0, diff);
}

// Credit: http://www.garagegames.com/community/blogs/view/309
int cc_box_line_intersection( cc_box_t *box, cc_line_t *line, float *t)
{
	float st,et,fst = 0,fet = 1;  
	float const *bmin = &box->min.x;  
	float const *bmax = &box->max.x;  
	float const *si = &line->p0.x;  
	float const *ei = &line->p1.x;  

	for (int i = 0; i < 3; i++)
	{  
		if (*si < *ei)
		{  
			if (*si > *bmax || *ei < *bmin)  
				return 0;  
			float di = *ei - *si;  
			st = (*si < *bmin)? (*bmin - *si) / di: 0;  
			et = (*ei > *bmax)? (*bmax - *si) / di: 1;  
		}  
		else
		{  
			if (*ei > *bmax || *si < *bmin)  
				return 0;  
			float di = *ei - *si;  
			st = (*si > *bmax)? (*bmax - *si) / di: 0;  
			et = (*ei < *bmin)? (*bmin - *si) / di: 1;  
		}  

		if (st > fst) fst = st;  
		if (et < fet) fet = et;  
		if (fet < fst)  
			return 0;  
		bmin++; bmax++;  
		si++; ei++;  
	}  

	*t = fst;  
	return 1;  
}

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

