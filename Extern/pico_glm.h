// Pico Graphics Linear Math Library
// by. Foxuper

#ifndef PICO_GLM_H
#define PICO_GLM_H

#include <math.h>

// Type | vec2
typedef union vec2
{
	float array[2];
	struct { float x, y; };
}
vec2;

// Type | vec3
typedef union vec3
{
	float array[3];
	struct { float x, y, z; };
}
vec3;

// Type | vec4
typedef union vec4
{
	float array[4];
	struct { float x, y, z, w; };
}
vec4;

// Type | mat2
typedef union mat2
{
	vec2 rows[2];
	float array[4];
	float matrix[2][2];
}
mat2;

// Type | mat3
typedef union mat3
{
	vec3 rows[3];
	float array[9];
	float matrix[3][3];
}
mat3;

// Type | mat4
typedef union mat4
{
	vec4 rows[4];
	float array[16];
	float matrix[4][4];
}
mat4;

// Vector constants
extern const vec2 VEC2_ZERO;
extern const vec3 VEC3_ZERO;
extern const vec4 VEC4_ZERO;

// Matrix constants
extern const mat2 MAT2_ZERO;
extern const mat2 MAT2_IDENTITY;
extern const mat3 MAT3_ZERO;
extern const mat3 MAT3_IDENTITY;
extern const mat4 MAT4_ZERO;
extern const mat4 MAT4_IDENTITY;

// PI constant
#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

// Generic macros
#define deg_to_rad(deg) ((float) deg * M_PI / 180)
#define rad_to_deg(rad) ((float) rad * 180 / M_PI)

// Macros | vec2
#define vec2_init(x, y)  (vec2) {.array = {x, y}}
#define vec2_fill(value) (vec2) {.array = {value, value}}

// Macros | vec3
#define vec3_init(x, y, z) (vec3) {.array = {x, y, z}}
#define vec3_fill(value)   (vec3) {.array = {value, value, value}}
#define vec3_cross(vec_a, vec_b) mat3_mul_vec(mat3_cross_mat(vec_a), vec_b);

// Macros | vec4
#define vec4_init(x, y, z, w) (vec4) {.array = {x, y, z, w}}
#define vec4_fill(value)      (vec4) {.array = {value, value, value, value}}

// Macros | mat4
#define mat4_xrot_mat(angle)      mat4_from_mat3(mat3_xrot_mat(angle))
#define mat4_yrot_mat(angle)      mat4_from_mat3(mat3_yrot_mat(angle))
#define mat4_zrot_mat(angle)      mat4_from_mat3(mat3_zrot_mat(angle))
#define mat4_rot_mat(axis, angle) mat4_from_mat3(mat3_rot_mat(axis, angle))

// Generic functions | vec2
extern vec2  vec2_add        (const vec2 vec_a, const vec2 vec_b);
extern vec2  vec2_sub        (const vec2 vec_a, const vec2 vec_b);
extern float vec2_dot        (const vec2 vec_a, const vec2 vec_b);
extern vec2  vec2_scale      (const vec2 vec, float scale);
extern mat2  vec2_outer_prod (const vec2 vec_a, const vec2 vec_b);
extern float vec2_norm       (const vec2 vec);
extern float vec2_norm2      (const vec2 vec);
extern vec2  vec2_normalize  (const vec2 vec);
extern vec2  vec2_reflect    (const vec2 vec, const vec2 normal);

// Generic functions | vec3
extern vec3  vec3_add        (const vec3 vec_a, const vec3 vec_b);
extern vec3  vec3_sub        (const vec3 vec_a, const vec3 vec_b);
extern float vec3_dot        (const vec3 vec_a, const vec3 vec_b);
extern vec3  vec3_scale      (const vec3 vec, float scale);
extern mat3  vec3_outer_prod (const vec3 vec_a, const vec3 vec_b);
extern float vec3_norm       (const vec3 vec);
extern float vec3_norm2      (const vec3 vec);
extern vec3  vec3_normalize  (const vec3 vec);
extern vec3  vec3_reflect    (const vec3 vec, const vec3 normal);

// Generic functions | vec4
extern vec4  vec4_add        (const vec4 vec_a, const vec4 vec_b);
extern vec4  vec4_sub        (const vec4 vec_a, const vec4 vec_b);
extern float vec4_dot        (const vec4 vec_a, const vec4 vec_b);
extern vec4  vec4_scale      (const vec4 vec, float scale);
extern mat4  vec4_outer_prod (const vec4 vec_a, const vec4 vec_b);
extern float vec4_norm       (const vec4 vec);
extern float vec4_norm2      (const vec4 vec);
extern vec4  vec4_normalize  (const vec4 vec);
extern vec4  vec4_reflect    (const vec4 vec, const vec4 normal);

// Vector casting functions
extern vec3 vec3_from_vec2 (const vec2 vec);
extern vec4 vec4_from_vec3 (const vec3 vec);

// Generic functions | mat2
extern mat2 mat2_fill      (float value);
extern mat2 mat2_identity  (float value);
extern mat2 mat2_transpose (const mat2 mat);
extern mat2 mat2_add       (const mat2 mat_a, const mat2 mat_b);
extern mat2 mat2_sub       (const mat2 mat_a, const mat2 mat_b);
extern mat2 mat2_mul       (const mat2 mat_a, const mat2 mat_b);
extern vec2 mat2_mul_vec   (const mat2 mat_a, const vec2 vec_b);
extern mat2 mat2_scale     (const mat2 mat, float scale);

// Generic functions | mat3
extern mat3 mat3_fill      (float value);
extern mat3 mat3_identity  (float value);
extern mat3 mat3_transpose (const mat3 mat);
extern mat3 mat3_add       (const mat3 mat_a, const mat3 mat_b);
extern mat3 mat3_sub       (const mat3 mat_a, const mat3 mat_b);
extern mat3 mat3_mul       (const mat3 mat_a, const mat3 mat_b);
extern vec3 mat3_mul_vec   (const mat3 mat_a, const vec3 vec_b);
extern mat3 mat3_scale     (const mat3 mat, float scale);

// Generic functions | mat4
extern mat4 mat4_fill      (float value);
extern mat4 mat4_identity  (float value);
extern mat4 mat4_transpose (const mat4 mat);
extern mat4 mat4_add       (const mat4 mat_a, const mat4 mat_b);
extern mat4 mat4_sub       (const mat4 mat_a, const mat4 mat_b);
extern mat4 mat4_mul       (const mat4 mat_a, const mat4 mat_b);
extern vec4 mat4_mul_vec   (const mat4 mat_a, const vec4 vec_b);
extern mat4 mat4_scale     (const mat4 mat, float scale);

// Generic upper functions | mat3
extern mat3 mat3_scale_aniso (const mat3 mat, vec2 vec);
extern mat3 mat3_translate   (const mat3 mat, vec2 vec);

// Generic upper functions | mat4
extern mat4 mat4_scale_aniso (const mat4 mat, vec3 vec);
extern mat4 mat4_translate   (const mat4 mat, vec3 vec);

// Matrix casting functions
extern mat3 mat3_from_mat2 (mat2 mat);
extern mat4 mat4_from_mat3 (mat3 mat);

// Functions | mat3
extern mat3 mat3_cross_mat (const vec3 vec);
extern mat3 mat3_xrot_mat  (float angle);
extern mat3 mat3_yrot_mat  (float angle);
extern mat3 mat3_zrot_mat  (float angle);
extern mat3 mat3_rot_mat   (vec3 rot_axis, float angle);
extern mat3 mat3_rotate_x  (mat3 mat, float angle);
extern mat3 mat3_rotate_y  (mat3 mat, float angle);
extern mat3 mat3_rotate_z  (mat3 mat, float angle);
extern mat3 mat3_rotate    (mat3 mat, vec3 axis, float angle);

// Functions | mat4
extern mat4 mat4_rotate_x    (mat4 mat, float angle);
extern mat4 mat4_rotate_y    (mat4 mat, float angle);
extern mat4 mat4_rotate_z    (mat4 mat, float angle);
extern mat4 mat4_rotate      (mat4 mat, vec3 axis, float angle);
extern mat4 mat4_perspective (float fov, float aspect, float near, float far);
extern mat4 mat4_ortho       (float left, float right, float bottom, float top, float near, float far);

#endif // PICO_GLM_H



#ifdef PICO_GLM_IMPLEMENTATION

#include <string.h>
#include <assert.h>

// Auxiliary macros
#define AUX_PREV_VEC3 vec2
#define AUX_PREV_VEC4 vec3

// Asserting vector sizes
static_assert(sizeof(vec2) == 2 * sizeof(float), "[pico_glm] vec2 size is incorrect");
static_assert(sizeof(vec3) == 3 * sizeof(float), "[pico_glm] vec3 size is incorrect");
static_assert(sizeof(vec4) == 4 * sizeof(float), "[pico_glm] vec4 size is incorrect");

// Asserting matrix sizes
static_assert(sizeof(mat2) == 4  * sizeof(float), "[pico_glm] mat2 size is incorrect");
static_assert(sizeof(mat3) == 9  * sizeof(float), "[pico_glm] mat3 size is incorrect");
static_assert(sizeof(mat4) == 16 * sizeof(float), "[pico_glm] mat4 size is incorrect");

// Vector constants
const vec2 VEC2_ZERO = {.array = {0, 0}};
const vec3 VEC3_ZERO = {.array = {0, 0, 0}};
const vec4 VEC4_ZERO = {.array = {0, 0, 0, 0}};

// Matrix constants
const mat2 MAT2_ZERO =     {.array = {0, 0, 0, 0}};
const mat2 MAT2_IDENTITY = {.array = {1, 0, 0, 1}};
const mat3 MAT3_ZERO =     {.array = {0, 0, 0, 0, 0, 0, 0, 0, 0}};
const mat3 MAT3_IDENTITY = {.array = {1, 0, 0, 0, 1, 0, 0, 0, 1}};
const mat4 MAT4_ZERO =     {.array = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
const mat4 MAT4_IDENTITY = {.array = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}};

// Generic vector functions
#define GENERIC_VECTOR_FUNCTIONS(n) \
\
vec##n vec##n##_add (const vec##n vec_a, const vec##n vec_b) \
{ \
	vec##n vec_out; \
	for (int i = 0; i < n; i++) \
		vec_out.array[i] = vec_a.array[i] + vec_b.array[i]; \
	return vec_out; \
} \
\
vec##n vec##n##_sub (const vec##n vec_a, const vec##n vec_b) \
{ \
	vec##n vec_out; \
	for (int i = 0; i < n; i++) \
		vec_out.array[i] = vec_a.array[i] - vec_b.array[i]; \
	return vec_out; \
} \
\
float vec##n##_dot (const vec##n vec_a, const vec##n vec_b) \
{ \
	float dot = 0; \
	for (int i = 0; i < n; i++) \
		dot += vec_a.array[i] * vec_b.array[i]; \
	return dot; \
} \
\
vec##n vec##n##_scale (const vec##n vec, float scalar) \
{ \
	vec##n vec_out; \
	for (int i = 0; i < n; i++) \
		vec_out.array[i] = vec.array[i] * scalar; \
	return vec_out; \
} \
\
mat##n vec##n##_outer_prod (const vec##n vec_a, const vec##n vec_b) \
{ \
	mat##n mat_out; \
	for (int i = 0; i < n; i++) \
	{ \
		for (int j = 0; j < n; j++) \
			mat_out.matrix[i][j] = vec_a.array[i] * vec_b.array[j]; \
	} \
	return mat_out; \
} \
\
float vec##n##_norm (vec##n vec) \
{ \
	return sqrtf(vec##n##_dot(vec, vec)); \
} \
\
float vec##n##_norm2 (vec##n vec) \
{ \
	return vec##n##_dot(vec, vec); \
} \
\
vec##n vec##n##_normalize (vec##n vec) \
{ \
	float norm = vec##n##_norm(vec); \
	return vec##n##_scale(vec, 1 / (norm ? norm : 1)); \
} \
\
vec##n vec##n##_reflect (vec##n vec, vec##n normal) \
{ \
	vec##n vec_out; \
	float projection = 2 * vec##n##_dot(vec, normal); \
	for (int i = 0; i < n; i++) \
		vec_out.array[i] = vec.array[i] - projection * normal.array[i]; \
	return vec_out; \
}

// Generic vector functions
GENERIC_VECTOR_FUNCTIONS(2)
GENERIC_VECTOR_FUNCTIONS(3)
GENERIC_VECTOR_FUNCTIONS(4)

vec3 vec3_from_vec2 (const vec2 vec)
{
	return (vec3) {.array = {vec.x, vec.y, 0}};
}

vec4 vec4_from_vec3 (const vec3 vec)
{
	return (vec4) {.array = {vec.x, vec.y, vec.z, 0}};
}

// Generic matrix functions
#define GENERIC_MATRIX_FUNCTIONS(n) \
\
mat##n mat##n##_fill (float value) \
{ \
	mat##n mat; \
	for (int i = 0; i < n * n; i++) \
		mat.array[i] = value; \
	return mat; \
} \
\
mat##n mat##n##_identity (float value) \
{ \
	mat##n mat; \
	for (int i = 0; i < n; i++) \
	{ \
		for (int j = 0; j < n; j++) \
			mat.matrix[i][j] = (i == j) ? value : 0; \
	} \
	return mat; \
} \
\
mat##n mat##n##_transpose (const mat##n mat) \
{ \
	mat##n mat_out; \
	for (int i = 0; i < n; i++) \
	{ \
		for (int j = 0; j < n; j++) \
			mat_out.matrix[i][j] = mat.matrix[j][i]; \
	} \
	return mat_out; \
} \
\
mat##n mat##n##_add (const mat##n mat_a, const mat##n mat_b) \
{ \
	mat##n mat_out; \
	for (int i = 0; i < n * n; i++) \
		mat_out.array[i] = mat_a.array[i] + mat_b.array[i]; \
	return mat_out; \
} \
\
mat##n mat##n##_sub (const mat##n mat_a, const mat##n mat_b) \
{ \
	mat##n mat_out; \
	for (int i = 0; i < n * n; i++) \
		mat_out.array[i] = mat_a.array[i] - mat_b.array[i]; \
	return mat_out; \
} \
\
mat##n mat##n##_mul (const mat##n mat_a, const mat##n mat_b) \
{ \
	mat##n mat_out; \
	for (int i = 0; i < n; i++) \
	{ \
		for (int j = 0; j < n; j++) \
		{ \
			float dot = 0; \
			for (int k = 0; k < n; k++) \
				dot += mat_a.matrix[i][k] * mat_b.matrix[k][j]; \
			mat_out.matrix[i][j] = dot; \
		} \
	} \
	return mat_out; \
} \
\
vec##n mat##n##_mul_vec (const mat##n mat_a, const vec##n vec_b) \
{ \
	vec##n vec_out; \
	for (int i = 0; i < n; i++) \
	{ \
		float dot = 0; \
		for (int j = 0; j < n; j++) \
			dot += mat_a.matrix[i][j] * vec_b.array[j]; \
		vec_out.array[i] = dot; \
	} \
	return vec_out; \
} \
\
mat##n mat##n##_scale (const mat##n mat, float scale) \
{ \
	mat##n mat_out; \
	for (int i = 0; i < n * n; i++) \
		mat_out.array[i] = mat.array[i] * scale; \
	return mat_out; \
}

// Generic matrix functions
GENERIC_MATRIX_FUNCTIONS(2)
GENERIC_MATRIX_FUNCTIONS(3)
GENERIC_MATRIX_FUNCTIONS(4)

// Generic upper matrix functions
#define GENERIC_UPPER_MATRIX_FUNCTIONS(n) \
\
mat##n mat##n##_scale_aniso (const mat##n mat, const AUX_PREV_VEC##n vec) \
{ \
	mat##n mat_out = mat; \
	for (int i = 0; i < n - 1; i++) \
	{ \
		for (int j = 0; j < n; j++) \
			mat_out.matrix[i][j] *= vec.array[i]; \
	} \
	return mat_out; \
} \
\
mat##n mat##n##_translate (const mat##n mat, const AUX_PREV_VEC##n vec) \
{ \
	mat##n mat_out = mat; \
	for (int i = 0; i < n - 1; i++) \
	{ \
		for (int j = 0; j < n; j++) \
			mat_out.matrix[i][j] += mat.matrix[n - 1][j] * vec.array[i]; \
	} \
	return mat_out; \
}

// Generic upper matrix functions
GENERIC_UPPER_MATRIX_FUNCTIONS(3)
GENERIC_UPPER_MATRIX_FUNCTIONS(4)

mat3 mat3_from_mat2 (mat2 mat)
{
	return (mat3) {.matrix =
	{
		{mat.matrix[0][0], mat.matrix[0][1], 0},
		{mat.matrix[1][0], mat.matrix[1][1], 0},
		{0, 0, 1}
	}};
}

mat4 mat4_from_mat3 (mat3 mat)
{
	return (mat4) {.matrix =
	{
		{mat.matrix[0][0], mat.matrix[0][1], mat.matrix[0][2], 0},
		{mat.matrix[1][0], mat.matrix[1][1], mat.matrix[1][2], 0},
		{mat.matrix[2][0], mat.matrix[2][1], mat.matrix[2][2], 0},
		{0, 0, 0, 1}
	}};
}

mat3 mat3_cross_mat (const vec3 vec)
{
	return (mat3) {.matrix =
	{
		{0, -vec.z, vec.y},
		{vec.z, 0, -vec.x},
		{-vec.y, vec.x, 0}
	}};
}

mat3 mat3_xrot_mat (float angle)
{
	float sin, cos;
	sincosf(angle, &sin, &cos);

	return (mat3) {.matrix =
	{
		{1,   0,    0},
		{0, cos, -sin},
		{0, sin,  cos}
	}};
}

mat3 mat3_yrot_mat (float angle)
{
	float sin, cos;
	sincosf(angle, &sin, &cos);

	return (mat3) {.matrix =
	{
		{cos,  0, sin},
		{0,    1,   0},
		{-sin, 0, cos}
	}};
}

mat3 mat3_zrot_mat (float angle)
{
	float sin, cos;
	sincosf(angle, &sin, &cos);

	return (mat3) {.matrix =
	{
		{cos, -sin, 0},
		{sin,  cos, 0},
		{0,    0,   1}
	}};
}

mat3 mat3_rot_mat (vec3 axis, float angle)
{
	mat3 rot_mat;

	float sin = sinf(angle);
	float cos = cosf(angle);

	vec3 norm_axis = vec3_normalize(axis);
	mat3 cross_mat = mat3_cross_mat(norm_axis);
	mat3 outer_mat = vec3_outer_prod(norm_axis, norm_axis);

	cross_mat = mat3_scale(cross_mat, sin);
	outer_mat = mat3_scale(outer_mat, 1 - cos);

	rot_mat = mat3_identity(cos);
	rot_mat = mat3_add(rot_mat, cross_mat);
	rot_mat = mat3_add(rot_mat, outer_mat);

	return rot_mat;
}

mat3 mat3_rotate_x (mat3 mat, float angle)
{
	mat3 rot_mat = mat3_xrot_mat(angle);
	return mat3_mul(rot_mat, mat);
}

mat3 mat3_rotate_y (mat3 mat, float angle)
{
	mat3 rot_mat = mat3_yrot_mat(angle);
	return mat3_mul(rot_mat, mat);
}

mat3 mat3_rotate_z (mat3 mat, float angle)
{
	mat3 rot_mat = mat3_zrot_mat(angle);
	return mat3_mul(rot_mat, mat);
}

mat3 mat3_rotate (mat3 mat, vec3 axis, float angle)
{
	mat3 rot_mat = mat3_rot_mat(axis, angle);
	return mat3_mul(rot_mat, mat);
}

mat4 mat4_rotate_x (mat4 mat, float angle)
{
	mat4 rot_mat = mat4_xrot_mat(angle);
	return mat4_mul(rot_mat, mat);
}

mat4 mat4_rotate_y (mat4 mat, float angle)
{
	mat4 rot_mat = mat4_yrot_mat(angle);
	return mat4_mul(rot_mat, mat);
}

mat4 mat4_rotate_z (mat4 mat, float angle)
{
	mat4 rot_mat = mat4_zrot_mat(angle);
	return mat4_mul(rot_mat, mat);
}

mat4 mat4_rotate (mat4 mat, vec3 axis, float angle)
{
	mat4 rot_mat = mat4_rot_mat(axis, angle);
	return mat4_mul(rot_mat, mat);
}

mat4 mat4_perspective (float fov, float aspect, float near, float far)
{
	mat4 perspective = MAT4_ZERO;

	float depth = near - far;
	float scale = 1 / tanf(fov / 2);

	perspective.matrix[0][0] = scale / aspect;
	perspective.matrix[1][1] = scale;
	perspective.matrix[2][2] = (near + far) / depth;
	perspective.matrix[2][3] = (2 * near * far) / depth;
	perspective.matrix[3][2] = -1;

	return perspective;
}

mat4 mat4_ortho (float left, float right, float bottom, float top, float near, float far)
{
	mat4 ortho = MAT4_ZERO;

	float width = right - left;
	float height = top - bottom;
	float depth = far - near;

	ortho.matrix[0][0] = 2 / width;
	ortho.matrix[1][1] = 2 / height;
	ortho.matrix[2][2] = -2 / depth;
	ortho.matrix[0][3] = -(right + left) / width;
	ortho.matrix[1][3] = -(top + bottom) / height;
	ortho.matrix[2][3] = -(far + near) / depth;
	ortho.matrix[3][3] = 1;

	return ortho;
}

#endif // PICO_GLM_IMPLEMENTATION