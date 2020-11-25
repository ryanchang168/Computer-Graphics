#define GLM_ENABLE_EXPERIMENTAL

#include "Object.h"
#include "FreeImage.h"
#include "glew.h"
#include "freeglut.h"
#include "shader.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/glm.hpp"
#include <iostream>
#include <string>
#include <math.h>
#include <stb_image.h>

using namespace std;

void shaderInit();
void bindbufferInit();
void textureInit();
void display();
void idle();
void reshape(GLsizei w, GLsizei h);
void keyboard(unsigned char key, int x, int y);
void DrawBasis();
void DrawUmbreon();
void LoadTexture(unsigned int&, const char*);

GLuint program;
GLuint VAO, VBO[3];
unsigned int basistexture, modeltexture;
int windowSize[2] = { 600, 600 };
float angle = 0.0f;

Object* model = new Object("UmbreonHighPoly.obj");

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(windowSize[0], windowSize[1]);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("hw2");

	glewInit();
	shaderInit();
	bindbufferInit();
	textureInit();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}

void shaderInit() {
	//// TODO: ////
	//
	// Hint:
	// 1. createShader
	// 2. createProgram

	GLuint vert = createShader("Shaders/vertexShader.vert", "vertex");
	GLuint frag = createShader("Shaders/fragmentShader.frag", "fragment");
	program = createProgram(vert, frag);

}

void bindbufferInit() {
	//// TODO: ////
	//
	//	 Hint:
	// 1. Setup VAO
	// 2. Setup VBO of vertex positions, normals, and texcoords
	/*struct VertexAttribute { 
		GLfloat position[3];
		GLfloat normal[3];
		GLfloat texcoord[2];
	};*/
	//VertexAttribute* vertices;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(3, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, model->positions.size() * sizeof(float), &model->positions[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, model->normals.size() * sizeof(float), &model->normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, model->texcoords.size() * sizeof(float), &model->texcoords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	//glBindVertexArray(0);



	// glEnable(GL_LIGHTING);
	// glShadeModel(GL_SMOOTH);

	//glEnable(GL_TEXTURE_2D);
}

void textureInit() {
	LoadTexture(basistexture, "basis.jpg");
	LoadTexture(modeltexture, "Umbreon.jpg");
}

glm::mat4 getV()
{
	// set camera position and configuration
	return glm::lookAt(glm::vec3(7.5, 5.0, 7.5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}
glm::mat4 getP()
{
	// set perspective view
	float fov = 60.0f;
	float aspect = windowSize[0] / windowSize[1];
	float nearDistance = 1.0f;
	float farDistance = 1000.0f;
	return glm::perspective(glm::radians(fov), aspect, nearDistance, farDistance);
}

void display() {
	//Clear the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawUmbreon();

	DrawBasis();

	angle += 0.1;
	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h) {
	windowSize[0] = w;
	windowSize[1] = h;
}

void LoadTexture(unsigned int& texture, const char* tFileName) {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(tFileName, &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

void keyboard(unsigned char key, int x, int y) {

}

void idle() {
	glutPostRedisplay();
}

void DrawBasis() {
	// viewport transformation
	glViewport(0, 0, windowSize[0], windowSize[1]);

	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)windowSize[0] / (GLfloat)windowSize[1], 1.0, 1000.0);

	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(7.5, 5.0, 7.5,// eye
		0.0, 0.0, 0.0,     // center
		0.0, 1.0, 0.0);    // up

	glPushMatrix();
	glRotatef(angle, 0, 1, 0);

	//// if you don't need this, you can just deleting
	float n = 20;
	float R = 4;
	float PI = 3.1415926;

	//// TODO: ////
	//
	// draw the basis and make the side of basis with texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, basistexture);

	GLfloat x, z, x2, z2;

	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 1.0f, 0.0f);
	//glTexCoord2f(0.0f, 0.0f);
	for (int i = 0; i < n; i++) {
		x = R * cos(2 * PI / n * i);
		z = R * sin(2 * PI / n * i);
		glVertex3f(x, 0, z);
	}
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.0f, -1.0, 0.0f);
	//glTexCoord2f(0.0f, 0.0f);
	for (int i = 0; i < n; i++) {
		x = R * cos(2 * PI / n * i);
		z = R * sin(2 * PI / n * i);
		glVertex3f(x, -5, z);
	}
	glEnd();

	for (int i = 0; i < n; i++) {
		glBegin(GL_QUADS);
		glNormal3f((sin(PI / 180 * (18 * i + 18)) - sin(PI / 180 * (18 * i))), 0.0f, (cos(PI / 180 * (18 * i)) - cos(PI / 180 * (18 * i + 18))));
		{
			x = R * cos(2 * PI / n * i);
			z = R * sin(2 * PI / n * i);
			x2 = R * cos(2 * PI / n * (i + 1));
			z2 = R * sin(2 * PI / n * (i + 1));
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(x, 0, z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(x, -5, z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(x2, -5, z2);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(x2, 0, z2);
		}
		glEnd();
	}

	glPopMatrix();
}

void DrawUmbreon()
{
	glUseProgram(program);
	glm::mat4 M(1.0f);
	M = glm::rotate(M, glm::radians(angle), glm::vec3(0, 1, 0));
	M = glm::translate(M, glm::vec3(0, 1.3, 0));

	GLuint ModelMatrixID = glGetUniformLocation(program, "M");
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &M[0][0]);

	//// TODO: ////
	// pass projection matrix, and view matrix and trigger by Uniform (use getP() amd getV())
	// also pass modeltexture to shader and trigger by Uniform

	//GLfloat pmtx[16] = { getP()[0][0] };
	//GLfloat mmtx[16] = { getV()[0][0] };

	//glGetFloatv(GL_PROJECTION_MATRIX, pmtx);
	//glGetFloatv(GL_MODELVIEW_MATRIX, mmtx);
	
	GLint pmatLoc = glGetUniformLocation(program, "P");
	GLint mmatLoc = glGetUniformLocation(program, "V");
	glUniformMatrix4fv(pmatLoc, 1, GL_FALSE, &getP()[0][0]);
	glUniformMatrix4fv(mmatLoc, 1, GL_FALSE, &getV()[0][0]);
	//glUseProgram(0);

	//glUseProgram(program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, modeltexture);
	GLint texLoc = glGetUniformLocation(program, "Texture");
	glUniform1i(texLoc, 0);
	
	
	glBindVertexArray(VAO);
	glDrawArrays(GL_QUADS, 0, 4 * model->fNum);

	//glDisableVertexAttribArray(2);
	//glDisableVertexAttribArray(1);
	//glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	//glActiveTexture(0);
	
}