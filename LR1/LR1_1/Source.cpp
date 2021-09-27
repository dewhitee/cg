#if defined(linux) || defined(_WIN32)
#include <GL/glut.h> /*For Linux and Windows*/
#else
#include <GLUT/GLUT.h> /*For Mac OS*/
#endif

#include <math.h>

void reshape(int w, int h);
void display();
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
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
	gluOrtho2D(-100, 100, -100, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void display()
{
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	// axis
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2i(-100, 0);
	glVertex2i(100, 0);

	glVertex2i(0, -100);
	glVertex2i(0, 100);
	glEnd();

	// function
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 1);

	for (float x = -100; x <= 100; x += .5f)
	{
		glVertex2f(x, fabs(.25f * x + 3 * cos(100 * x) * sin(x)));
	}

	glEnd();
	glutSwapBuffers();
}
