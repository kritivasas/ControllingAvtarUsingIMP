/*
Part of GVC Project
by :- Addy , Mau , Chinky , Vidushi , Monica
Copyright Reserved
Nobody outside group should access.
*/

#include <stdio.h>
#include <iostream>
#include <string>
#include <GL/freeglut.h>
#include <cmath>
#include "human.h"
#include "receiverprog.h"

using namespace std;

human A;

void display(void)
{
	//nothing to be touched over here.
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	A.draw();
	glutSwapBuffers();
}

void reshape (int w, int h)
{
	//nothing to be touched over here.
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 10.0, 800.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt (0.0, 0.0, 22.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void TimerFunc(int value)
{
	// this is where animation takes place.
	// free to edit. Take care.
	//double lst,lsp,let,lep;

	recv_data(&A.lShoulder, &A.lWrist, &A.rShoulder, &A.rWrist);
	glutTimerFunc(100,TimerFunc,1);
	glutPostRedisplay();
}

void keyboard (unsigned char key, int x, int y)
{
	// nothing for time being
}

void input()
{
	// nothing for time being
	init_recv();
	recv_data(&A.lShoulder, &A.rShoulder, &A.lWrist, &A.rWrist);
}

int main(int argc, char** argv)
{
	// nothing to be touched over here
	input();
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize (500, 500);
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(33,TimerFunc,1);
	glutMainLoop();
	return 0;
}
