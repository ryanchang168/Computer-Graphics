#include "Object.h"
#include "glut.h"
#include <math.h>
#include <iostream>

using namespace std;

#define PI acos(-1)

int windowSize[2];
float degrees[2] = { 0.0f, 0.0f };
float up = 0.0, r = 0.0;
bool stop = false;

void light();
void display();
void reshape(GLsizei, GLsizei);
void LoadModel(Object*);
void DrawBasis();
void keyboard(unsigned char key, int x, int y);

Object* elephant = new Object("elephant.obj");
Object* cube = new Object("cube.obj");
Object* teapot = new Object("teapot.obj");


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("HW1");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

void light()
{
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_position[] = { 50.0, 75.0, 80.0, 1.0 };
	glShadeModel(GL_SMOOTH);
	// z buffer enable
	glEnable(GL_DEPTH_TEST);
	// enable lighting
	glEnable(GL_LIGHTING);
	// set light property  
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
}

void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	///TO DO Enable DepthTest
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_NORMALIZE);
	//lighting
	light();
	
	//Viewport Matrix
	glViewport(0, 0, windowSize[0], windowSize[1]);
	// Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, windowSize[0] / (GLfloat)windowSize[1], 1.0, 1000);
	///ModelView Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(20.0f, 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	//cout << windowSize[1];

	///TO DO: add model, translate, rotate, scale
	//set keyboard control
	glTranslatef(up, 0.0, r);
	// Rotate totally
	glPushMatrix();
	//cout << up << r;
	glRotatef(degrees[1], 0, 1, 0);
	DrawBasis();

	//Elephant and cube
	glPushMatrix();
	glTranslatef(3.0, 0.0, -3.0);
	glRotatef(degrees[0], 0, 1, 0);
	LoadModel(elephant);
	glTranslatef(-2.0, 0.5, -2.0);
	glScalef(0.5f, 0.5f, 0.5f);
	LoadModel(cube);
	glPopMatrix();
	
	//Teapot
	glPushMatrix();
	glTranslatef(-3.0, 0.0, 3.0);
	LoadModel(teapot);
	glPopMatrix();

	//Pop all
	glPopMatrix();

	glutPostRedisplay();
	glutSwapBuffers();

	//Add degrees with each frame
	if (stop == false) {     // check whether to stop
		degrees[0] += 3;
		degrees[1] += 1;
	}
}

void reshape(GLsizei w, GLsizei h)
{
	windowSize[0] = w;
	windowSize[1] = h;
}

void LoadModel(Object* Model)
{
	for (size_t i = 0; i < Model->fNum; ++i)
	{
		glBegin(GL_TRIANGLES);
		for (size_t j = 0; j < 3; ++j)
		{
			glNormal3fv(Model->nList[Model->faceList[i][j].nIndex].ptr);
			glVertex3fv(Model->vList[Model->faceList[i][j].vIndex].ptr);
		}
		glEnd();
	}
}

void DrawBasis()
{
	///TO DO: draw basis
	glPolygonMode(GL_FRONT, GL_FILL);
	int R = 10, n = 20;
	float x, z, x2, z2;

	//頂面和底面
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < n; i++) {
		x = R * cos(2 * PI / n * i);
		z = R * sin(2 * PI / n * i);
		glVertex3f(x, 0, z);
	}
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.0f, -1.0, 0.0f);
	for (int i = 0; i < n; i++){
		x = R * cos(2 * PI / n * i);
		z = R * sin(2 * PI / n * i);
		glVertex3f(x, -5, z);
	}
	glEnd();

	//長方形
	//glNormal3f(0.0f, 0.0f, -1.0f);
	for (int i = 0; i < n; i++) {
		glBegin(GL_QUADS);
		glNormal3f((sin(PI / 180 * (18*i+18)) - sin(PI / 180 * (18*i))), 0.0f, (cos(PI / 180 * (18*i)) - cos(PI / 180 * (18*i+18))));
		//glNormal3f(0.0f, 0.0f, 1.0f);
		{
			x = R * cos(2 * PI / n * i);
			z = R * sin(2 * PI / n * i);
			x2 = R * cos(2 * PI / n * (i + 1));
			z2 = R * sin(2 * PI / n * (i + 1));
			glVertex3f(x, 0, z);
			glVertex3f(x, -5, z);
			glVertex3f(x2, -5, z2);
			glVertex3f(x2, 0, z2);
		}
		glEnd();
	}
}

void keyboard(unsigned char key, int x, int y)
{
	/// TO DO: Set the keyboard control
	switch (key) {
		case 119:
			up += 1.0;
			break;
		case 120:
			up -= 1.0;
			break;
		case 100:
			r += 1.0;
			break;
		case 97:
			r -= 1.0;
			break;
		case 32:
			stop = stop ^ true;    // change the stop state
			break;
	}
	


}