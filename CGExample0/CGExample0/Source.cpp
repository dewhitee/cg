#if defined(linux) || defined(_WIN32)
#include <GL/glut.h>    /*Для Linux и Windows*/
#else
#include <GLUT/GLUT.h>  /*Для Mac OS*/
#endif

#include <math.h>

void reshape(int w, int h);
void display();

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	/*Включаем двойную буферизацию и четырехкомпонентный цвет*/

	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL lesson 1");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	glutMainLoop();

	return 0;
}

void reshape(int w, int h)
{
	// Defines part of a window where we drawing
	// The same size of the window (0, 0 coords of bottom LEFT corner)
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(0, w, 0, h);

	// Changing projection of world to fit low vertex values from display function
	//gluOrtho2D(0, 1, 0, 1);
	gluOrtho2D(-5, 5, -5, 5);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display()
{
	//glClearColor(/*1*/0, 0, 0, 0);
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	// axis
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2i(-5, 0);
	glVertex2i(5, 0);

	glVertex2i(0, -5);
	glVertex2i(0, 5);
	glEnd();

	// sinusoid
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 1);

	for (float x = -4; x <= 4; x += 0.05)
	{
		glVertex2f(x, sin(x));
	}
	glEnd();

	//glBegin(GL_QUADS);
	//glColor3f(1.0, 1.0, 1.0);
	////glVertex2i(250, 450);
	//glVertex2f(0.250, 0.450);
	//glColor3f(0.0, 0.0, 1.0);
	//glVertex2f(0.250, 0.150);
	//glColor3f(0.0, 1.0, 0.0);
	//glVertex2f(0.550, 0.150);
	//glColor3f(1.0, 0.0, 0.0);
	//glVertex2f(0.550, 0.450);
	glEnd();

	glutSwapBuffers();
}

