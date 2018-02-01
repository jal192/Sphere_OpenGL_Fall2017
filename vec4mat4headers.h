/*
	vec4mat4headers.h

	Created on: Sept 17, 2017
	Author: Jason Ly
*/

#ifdef __APPLE__  // include Mac OS X verions of headers

#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>

#else // non-Mac OS X operating systems

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#endif  // __APPLE__

#include <math.h>

// NOTE: All vectors are being treated as column vectors and
//		 all matrices are treated as being column major
// Vector multiplication was not coded because it's undefined

// Define Vector
typedef struct {
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;
} vec4;

typedef struct
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
} vec3;

// Define Matrix
typedef struct {
	vec4 x;
	vec4 y;
	vec4 z;
	vec4 w;
} mat4;

typedef struct {
	vec3 x;
	vec3 y;
	vec3 z;
} mat3;

// Vector function signatures
vec3 v3_add(vec3 v1, vec3 v2);
vec3 v3_sub(vec3 v1, vec3 v2);
vec3 v3_cross_prod(vec3 v1, vec3 v2);
vec3 v3_scalar_mult(vec3 v, GLfloat s);
GLfloat v3_dot_prod(vec3 v1, vec3 v2);
vec3 v3_unit_vec(vec3 v1);
void print_vec3(vec3 v);

vec4 v4_add(vec4 v1, vec4 v2);
vec4 v4_sub(vec4 v1, vec4 v2);
vec4 v4_scalar_mult(vec4 v, GLfloat s);
GLfloat v4_dot_prod(vec4 v1, vec4 v2);
vec4 v4_cross_prod(vec4 v1, vec4 v2);
vec4 v4_unit_vec(vec4 v1);
GLfloat v4_length(vec4 v1);
void print_vec4(vec4 v);

// Matrix function signatures
mat3 m3_add(mat3 m1, mat3 m2);
mat3 m3_sub(mat3 m1, mat3 m2);
mat3 m3_mult(mat3 m1, mat3 m2);
mat3 m3_scalar_mult(mat3 m, GLfloat s);
mat3 m3_inverse(mat3 m);
mat3 m3_transpose(mat3 m);
void print_mat3(mat3 m);
mat3 m3_minor(mat3 m);
mat3 m3_cofactor(mat3 m);
GLfloat m3_determinant(mat3 m);

mat4 m4_add(mat4 m1, mat4 m2);
mat4 m4_sub(mat4 m1, mat4 m2);
mat4 m4_mult(mat4 m1, mat4 m2);
mat4 m4_scalar_mult(mat4 m, GLfloat s);
mat4 m4_inverse(mat4 m);
mat4 m4_transpose(mat4 m);
void print_mat4(mat4 m);
mat4 m4_minor(mat4 m);
mat4 m4_cofactor(mat4 m);
GLfloat m4_determinant(mat4 m);
