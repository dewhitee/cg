#include <GL/glut.h>

void reshape(int w, int h);
void display();
void drawAxis();
void processNormalKeys(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);

constexpr float translationValue = 10.f;

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(800, 600);
	glutCreateWindow("Lab 3");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutMainLoop();

	return 0;
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display()
{
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2i(250, 450);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2i(250, 150);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2i(550, 150);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(550, 450);
	glEnd();
	drawAxis(); // draw axis should be after glClear
	glutSwapBuffers();
	glDisable(GL_CLIP_PLANE0);

}

void drawAxis()
{
	// X
	glColor4d(1, 0, 0, 0.5);
	glBegin(GL_LINES);
	glVertex2i(-1600, 300);
	glVertex2i(1600, 300);
	glEnd();

	// Y
	glColor4d(0, 1, 0, 0.5);
	glBegin(GL_LINES);
	glVertex2i(400, -1000);
	glVertex2i(400, 1000);
	glEnd();
}

void processNormalKeys(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
	if (key == 65)
	{
		glMatrixMode(GL_MODELVIEW);
		glTranslated(translationValue, translationValue, 0);
		display();
	}
}

void processSpecialKeys(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_UP:
			glMatrixMode(GL_MODELVIEW);
			glTranslated(0, translationValue, 0);
			display();
			break;
		case GLUT_KEY_DOWN:
			glMatrixMode(GL_MODELVIEW);
			glTranslated(0, -translationValue, 0);
			display();
			break;
		case GLUT_KEY_LEFT:
			glMatrixMode(GL_MODELVIEW);
			glTranslated(-translationValue, 0, 0);
			display();
			break;
		case GLUT_KEY_RIGHT:
			glMatrixMode(GL_MODELVIEW);
			glTranslated(translationValue, 0, 0);
			display();
			break;

	}
}
