/*
* sphere.c
*
*  Created on: Sept 28, 2017
*      Author: Jason Ly
*/

#ifdef __APPLE__  // include Mac OS X versions of headers

#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>

#else // non-Mac OS X operating systems

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#endif  // __APPLE__

#include "vec4mat4headers.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
#include "initShader.h"
#include <string.h>
#include <time.h>
#include <windows.h>

// Function signatures
float randomFloat();
void motion(int x, int y);
mat4 rotate_about_x(float d, float alpha_y, float alpha_z);
mat4 rotate_about_y(float d, float alpha_x);
mat4 rotate_about_z(float theta);
mat4 scale(float sx, float sy, float sz);

// Number of vertices
int num_vertices = 1086;

// Stores the colors and vertices
vec4 vertices[1086];
vec4 colors[1086];

float radian_to_degree = 180.0 / M_PI;

// Set the screen width and height
int screen_width = 512;
int screen_height = 512;

// Moves the window to the right side of the screen
int window_position_width = 900;
int window_position_height = 100;

// Stores the radius or what half the sides of the screen is
int mid_width_x;
int mid_height_y;

GLuint ctm_location;

// Translation matrix
mat4 tr_matrix =
{ { 1.0, 0.0, 0.0, 0.0 },
{ 0.0, 1.0, 0.0, 0.0 },
{ 0.0, 0.0, 1.0, 0.0 },
{ 0.0, 0.0, 0.0, 1.0 } };

// Keeping track of the previous and current mouse coordinates
int curr_x;
int curr_y;
float curr_z;
int prev_x;
int prev_y;
float prev_z;

// Calculate the amount of rotation about z axis
float theta_z;

// Keep track of the last rotation matrix
mat4 last_rotation_matrix;

// Check if the default points have been modified
int first_coordinate = 0;

int enableIdle = 0;
int leftDown = 1;
clock_t start_t, end_t;
float diff_t;

// Check if a last rotation matrix has been stored
int last_rot_mat_exists = 0;

void init(void) {
	// This code below is from the book and example code provided by Tan
	// The code has been modified to use triangles instead of quads
	// Get all of the sphere except the poles
	const float DegreesToRadians = M_PI / 180.0;
	float phir;
	float phir20;
	float thetar;
	int k = 0;

	// Generate half of the triangles for the quad
	for (float phi = -80.0; phi <= 80.0; phi += 20.0) {
		phir = phi*DegreesToRadians;
		phir20 = (float)(phi + 20.0)*DegreesToRadians;
		for (float theta = -180.0; theta < 180.0; theta += 20.0) {
			thetar = theta*DegreesToRadians;
			vertices[k].x = sin(thetar)*cos(phir);
			vertices[k].y = cos(thetar)*cos(phir);
			vertices[k].z = sin(phir);
			vertices[k].w = 1.0;
			k++;
			vertices[k].x = sin(thetar)*cos(phir20);
			vertices[k].y = cos(thetar)*cos(phir20);
			vertices[k].z = sin(phir20);
			vertices[k].w = 1.0;
			k++;
			thetar = (theta + 20)*DegreesToRadians;
			vertices[k].x = sin(thetar)*cos(phir);
			vertices[k].y = cos(thetar)*cos(phir);
			vertices[k].z = sin(phir);
			vertices[k].w = 1.0;
			k++;
		}
	}

	// Generate the other half of the triangles for the quads
	for (float phi = -80.0; phi <= 80.0; phi += 20.0) {
		phir = phi*DegreesToRadians;
		phir20 = (float)(phi + 20.0)*DegreesToRadians;
		for (float theta = -180.0; theta < 180.0; theta += 20.0) {
			thetar = theta*DegreesToRadians;
			vertices[k].x = sin(thetar)*cos(phir20);
			vertices[k].y = cos(thetar)*cos(phir20);
			vertices[k].z = sin(phir20);
			vertices[k].w = 1.0;
			k++;
			thetar = (theta + 20)*DegreesToRadians;
			vertices[k].x = sin(thetar)*cos(phir20);
			vertices[k].y = cos(thetar)*cos(phir20);
			vertices[k].z = sin(phir20);
			vertices[k].w = 1.0;
			k++;
			vertices[k].x = sin(thetar)*cos(phir);
			vertices[k].y = cos(thetar)*cos(phir);
			vertices[k].z = sin(phir);
			vertices[k].w = 1.0;
			k++;
		}
	}

	// Get the top and bottom
	float sin80 = sin(80.0*DegreesToRadians);
	float cos80 = cos(80.0*DegreesToRadians);

	for (float theta = -180.0; theta <= 180.0; theta += 20.0)
	{
		// Draw the triangle, taking the first point on the outside
		// of the circle, then the center then the next point
		float thetar = theta*DegreesToRadians;
		vertices[k].x = sin(thetar)*cos80;
		vertices[k].y = cos(thetar)*cos80;
		vertices[k].z = sin80;
		vertices[k].w = 1.0;
		k++;
		vertices[k].x = 0;
		vertices[k].y = 0;
		vertices[k].z = 1;
		vertices[k].w = 1.0;
		k++;
		float thetatemp = (theta + 20)*DegreesToRadians;
		vertices[k].x = sin(thetatemp)*cos80;
		vertices[k].y = cos(thetatemp)*cos80;
		vertices[k].z = sin80;
		vertices[k].w = 1.0;
		k++;
	}

	for (float theta = -180.0; theta <= 180.0; theta += 20.0)
	{
		// Draw the triangle, taking the first point on the outside
		// of the circle, then the center then the next point
		float temp = theta;
		
		float thetatemp = (temp + 20)*DegreesToRadians;
		vertices[k].x = sin(thetatemp)*cos80;
		vertices[k].y = cos(thetatemp)*cos80;
		vertices[k].z = -sin80;
		vertices[k].w = 1.0;
		k++;

		vertices[k].x = 0;
		vertices[k].y = 0;
		vertices[k].z = -1;
		vertices[k].w = 1.0;
		k++;
		
		float thetar = temp*DegreesToRadians;
		vertices[k].x = sin(thetar)*cos80;
		vertices[k].y = cos(thetar)*cos80;
		vertices[k].z = -sin80;
		vertices[k].w = 1.0;
		k++;
	}

	srand(time(NULL));

	// Generate random colors
	for (int i = 0; i < num_vertices; i++) {
		colors[i].x = randomFloat();
		colors[i].y = randomFloat();
		colors[i].z = randomFloat();
		colors[i].w = 1.0;
	}

	// Calculate the center of the screen
	mid_width_x = floor(screen_width / 2.0);
	mid_height_y = floor(screen_height / 2.0);

	GLuint program = initShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));

	ctm_location = glGetUniformLocation(program, "ctm");

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glDepthRange(1, 0);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&tr_matrix);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);

	glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
	// Terminate program
	if (key == 'q')
		exit(0);

	// Press space bar to stop spinning
	if (key == ' ') {
		enableIdle = 0;
		first_coordinate = 0;
		last_rot_mat_exists = 0;
	}

	glutPostRedisplay();
}

void idle(void)
{
	if (enableIdle)
	{
		// Continually apply rotation matrix until idle stops
		tr_matrix = m4_mult(last_rotation_matrix, tr_matrix);

		// Slow down idle because it was spinning too fast
		Sleep(10);

		glutPostRedisplay();
	}
}

void mouse(int button, int state, int x, int y) {
	GLfloat scaling_factor = 1.02;
	// Scrolling Up
	if (button == 3) {
		mat4 scaling_matrix = scale(scaling_factor, scaling_factor, scaling_factor);
		tr_matrix = m4_mult(scaling_matrix, tr_matrix);
		glutPostRedisplay();
	}
	// Scrolling Down
	else if (button == 4) {
		mat4 scaling_matrix = scale(1/scaling_factor, 1/scaling_factor, 1/scaling_factor);
		tr_matrix = m4_mult(scaling_matrix, tr_matrix);
		glutPostRedisplay();
	}

	// Left mouse button is pressed down
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// Start time
		start_t = clock();
		motion(x, y);
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		// End time
		end_t = clock();
		
		first_coordinate = 0;

		// Get the difference between the start and end time
		diff_t = (float)(end_t - start_t) / CLOCKS_PER_SEC;

		// If the clicks and drags and lets go of the mouse in under 0.5 seconds, spin object forever
		if (diff_t < 0.5 && last_rot_mat_exists == 1) {
			enableIdle = 1;
		}
	}
}

void motion(int x, int y) {
	// Recenter x,y to center of the screen
	curr_x = x - mid_width_x;
	curr_y = -(y - mid_height_y);

	// NOTE: I am assuming that the screen widths are the same for x, y, z
	// Makes calculating z a little easier

	float curr_z = sqrt((mid_width_x*mid_height_y) - (curr_x*curr_x) - (curr_y*curr_y));
	// Check for nan, space where user moves outside of the trackball
	// Don't calculate a rotation matrix if the user exceeds the trackball
	// Also don't calculate if the previous and current points are the same - prevents object from disappearing
	if (curr_z == curr_z && ((prev_x != curr_x) || (prev_y != curr_y))) {
		// Update values for the first time
		if (first_coordinate == 0) {
			prev_x = curr_x;
			prev_y = curr_y;
			prev_z = curr_z;

			first_coordinate = 1;
		}
		else {
			// Initialize the vectors representing the previous and current points
			vec4 curr_point = { curr_x, curr_y, curr_z, 0.0 };
			vec4 prev_point = { prev_x, prev_y, prev_z, 0.0 };

			// Use unit vectors of the current and previous point to make calculating the answer simpler
			vec4 unit_curr = v4_unit_vec(curr_point);
			vec4 unit_prev = v4_unit_vec(prev_point);

			// Obtain the vector that you rotate around
			vec4 cross_curr_prev = v4_cross_prod(prev_point, curr_point);

			// Calculate the unit vector of the rotation vector
			vec4 unit_vector = v4_unit_vec(cross_curr_prev);

			// Calculate the length of the normal vector
			float d = sqrt((unit_vector.y*unit_vector.y) + (unit_vector.z*unit_vector.z));

			// By default set the rotation matrices about the x axis to identity matrix
			// If we are not rotating about the x-axis then calculate rx_pos and rx_neg as usual
			mat4 rx_pos =
			{ { 1.0, 0.0, 0.0, 0.0 },
			{ 0.0, 1.0, 0.0, 0.0 },
			{ 0.0, 0.0, 1.0, 0.0 },
			{ 0.0, 0.0, 0.0, 1.0 } };

			mat4 rx_neg = 
			{ { 1.0, 0.0, 0.0, 0.0 },
			{ 0.0, 1.0, 0.0, 0.0 },
			{ 0.0, 0.0, 1.0, 0.0 },
			{ 0.0, 0.0, 0.0, 1.0 } };

			// If d does not equal 0 then we are not rotating about the x-axis
			if (d != 0) {
				rx_pos = rotate_about_x(d, unit_vector.y, unit_vector.z);	// Rx(+)
				rx_neg = rotate_about_x(d, -unit_vector.y, unit_vector.z);  // Rx(-)
			}

			// Calculate the dot product of the unit vectors of the current and previous points
			GLfloat dot_curr_prev = v4_dot_prod(unit_curr, unit_prev);
			// Calculate the angle of rotation about z
			theta_z = radian_to_degree * acos(dot_curr_prev);
			
			// These steps follow what is shown in the slides
			// Get the rotation matrices that rotate the rotation axis along the x, y, z axis
			mat4 ry_pos = rotate_about_y(d, unit_vector.x);		// Ry(+)
			mat4 ry_neg = rotate_about_y(d, -unit_vector.x);	// Ry(-)
			mat4 rz = rotate_about_z(theta_z);					// Rz(theta_z)

			// Calculate the rotation matrix
			mat4 rotation_matrix = m4_mult(ry_neg, rx_pos);		// Ry(-)Rx(+)
			rotation_matrix = m4_mult(rz, rotation_matrix);		// Rz(theta_z)Ry(-)Rx(+)
			rotation_matrix = m4_mult(ry_pos, rotation_matrix); // Ry(+)Rz(theta_z)Ry(-)Rx(+)
			rotation_matrix = m4_mult(rx_neg, rotation_matrix); // Rx(-)Ry(+)Rz(theta_z)Ry(-)Rx(+)

			// Apply rotation matrix to translation matrix
			tr_matrix = m4_mult(rotation_matrix, tr_matrix);

			glutPostRedisplay();

			// Set new previous position
			prev_x = curr_x;
			prev_y = curr_y;
			prev_z = curr_z;
			
			// Keep track of the last rotation matrix
			last_rotation_matrix = rotation_matrix;
			last_rot_mat_exists = 1;
		}
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutInitWindowPosition(window_position_width, window_position_height);
	glutCreateWindow("Sphere: Transformation by Jason Ly");
	glewInit();
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutIdleFunc(idle);
	glutMainLoop();

	return 0;
}

// Get random numbers for colors
float randomFloat() {
	float x = ((float)rand() / (float)(RAND_MAX));
	return x;
}

// Function used to rotate an object about the x axis
mat4 rotate_about_x(float d, float alpha_y, float alpha_z) {
	mat4 result;
	result.x.x = 1; result.y.x = 0;			  result.z.x = 0;			 result.w.x = 0;
	result.x.y = 0; result.y.y = alpha_z / d; result.z.y = -alpha_y / d; result.w.y = 0;
	result.x.z = 0; result.y.z = alpha_y / d; result.z.z = alpha_z / d;  result.w.z = 0;
	result.x.w = 0;	result.y.w = 0;			  result.z.w = 0;			 result.w.w = 1;
	return result;
}

// Function used to rotate an object about the y axis
mat4 rotate_about_y(float d, float alpha_x) {
	mat4 result;
	result.x.x = d;		   result.y.x = 0; result.z.x = alpha_x; result.w.x = 0;
	result.x.y = 0;		   result.y.y = 1; result.z.y = 0;       result.w.y = 0;
	result.x.z = -alpha_x; result.y.z = 0; result.z.z = d;		 result.w.z = 0;
	result.x.w = 0;		   result.y.w = 0; result.z.w = 0;       result.w.w = 1;
	return result;
}

// Function used to rotate an object about the z axis
mat4 rotate_about_z(float theta) {
	mat4 result;
	float s = (M_PI / 180.0)*theta;
	result.x.x = cos(s);  result.y.x = -sin(s); result.z.x = 0; result.w.x = 0;
	result.x.y = sin(s);  result.y.y = cos(s);  result.z.y = 0; result.w.y = 0;
	result.x.z = 0;		  result.y.z = 0;		result.z.z = 1; result.w.z = 0;
	result.x.w = 0;		  result.y.w = 0;		result.z.w = 0; result.w.w = 1;
	return result;
}

// Function used to zoom in and out
mat4 scale(float sx, float sy, float sz) {
	mat4 result;
	result.x.x = sx; result.y.x = 0;  result.z.x = 0;  result.w.x = 0;
	result.x.y = 0;  result.y.y = sy; result.z.y = 0;  result.w.y = 0;
	result.x.z = 0;	 result.y.z = 0;  result.z.z = sz; result.w.z = 0;
	result.x.w = 0;	 result.y.w = 0;  result.z.w = 0;  result.w.w = 1;
	return result;
}
