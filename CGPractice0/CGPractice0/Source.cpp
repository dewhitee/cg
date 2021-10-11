#if defined(linux) || defined(_WIN32)
#include <GL/glut.h> /*Для Linux и Windows*/
#else
#include <GLUT/GLUT.h> /*Для Mac OS*/
#endif
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
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	//
	///gluOrtho2D(0, 5, 0, 5);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1, 1, 1, 1);

	GLdouble plane[4] = {0, -2, 0, 500};
	glClipPlane(GL_CLIP_PLANE0, plane);
	glEnable(GL_CLIP_PLANE0);

	glBegin(GL_QUADS);
	//glColor3f(1.0, 1.0, 1.0);
	//glVertex2i(250, 450);
	//glColor3f(0.0, 0.0, 1.0);
	//glVertex2i(250, 150);
	//glColor3f(0.0, 1.0, 0.0);
	//glVertex2i(550, 150);
	//glColor3f(1.0, 0.0, 0.0);
	//glVertex2i(550, 450);

	//glVertex2i(3, 3);
	//glVertex2i(7, 3);
	//glVertex2i(7, 8);
	//glVertex2i(3, 8);

	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(150, 450);
	glVertex2i(150, 150);
	glVertex2i(250, 150);
	glVertex2i(250, 450);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 1.0, 0.0);
	glVertex2i(350, 450);
	glVertex2i(350, 350);
	glVertex2i(450, 350);
	glVertex2i(450, 450);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(550, 400);
	glVertex2i(600, 450);
	glVertex2i(650, 400);
	glVertex2i(600, 350);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(350, 100);
	glVertex2i(400, 150);
	glVertex2i(450, 60);
	glVertex2i(300, 150);
	glEnd();

	glDisable(GL_CLIP_PLANE0);
	glutSwapBuffers();
}
