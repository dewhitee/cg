#if defined(linux) || defined(_WIN32)
#include <GL/glut.h>    /*Для Linux и Windows*/
#else
#include <GLUT/GLUT.h>  /*Для Mac OS*/
#endif

void reshape(int w, int h);
void display();
void processNormalKeys(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);

void axes();

int min = -150, max = 150, angle = 5;

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL lesson 6");

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
	//glOrtho(-15, 15, -15, 15, -15, 15);
	glOrtho(min, max, min, max, min, max);
	//gluPerspective(60, 1, 0, 20);
	//gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
	gluLookAt(3.0, 3.0, 3.0 - 4.5, 0.0, 0.0, -4.5, 0, 1, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(1, 1, 1, 0);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	//glColor3f(1.0, 0.0, 0.0);

	axes();

	glColor3f(0.8, 0.8, 0.8);

	glutWireTeapot(5);
	//glutSolidSphere(6, 5, 8);
	//glutWireSphere(10, 30, 30);
	//glutWireCube(10);

	//glutWireCone(10, 10, 30, 30);

	//glutSolidTorus(4, 12, 40, 40);	// тор - пончик
	//glutWireTorus(4, 12, 40, 40);		// сначала - innerRadius, pfntv outerRadius

	//glutWireTetrahedron();
	//glutWireOctahedron();
	//glutWireDodecahedron();
	//glutWireIcosahedron();

	glutSwapBuffers();
}

//void axes()
//{
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();			// Текущую матрицу отправить в стэк
//	glLoadIdentity();		// Загрузить единичную матрицу
//	glBegin(GL_LINES);
//
//	glColor3d(1, 0, 0);			// ось X	- красная
//	glVertex3f(min, 0.0, 0.0);
//	glVertex3f(max, 0.0, 0.0);
//
//	glColor3d(0, 1, 0);			// ось Y	- зеленая
//	glVertex3f(0.0, min, 0.0);
//	glVertex3f(0.0, max, 0.0);
//
//	glColor3d(0, 0, 1);			// ось Z
//	glVertex3f(0.0, 0.0, min);
//	glVertex3f(0.0, 0.0, max);
//
//	glEnd();
//
//	glPopMatrix();		// Достать текущую матрицу
//}

void axes()
{

	/*
		1. Для вывода координатной сетки необходимо:
			1.1. Текущую матрицу отправить в стэк
			1.2. Загрузить единичную матрицу
			1.3. Достать текущую матрицу
	*/
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();			// Текущую матрицу отправить в стэк
	glLoadIdentity();		// Загрузить единичную матрицу
	glBegin(GL_LINES);

	glColor3d(1, 0, 0);			// ось X	- красная
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(max, 0.0, 0.0);

	glColor3d(0, 1, 0);			// ось Y	- зеленая
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, max, 0.0);

	glColor3d(0, 0, 1);			// ось Z
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, max);

	glEnd();

	glPopMatrix();		// Достать текущую матрицу
}

void processNormalKeys(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
	if (key == 43)	// 43 - plus
	{
		glScaled(1.1, 1.1, 1.1);
		display();
	}
	if (key == 45)	// 45 - minus
	{
		glScaled(0.9, 0.9, 0.9);
		display();
	}
	if (key == 127)	// 127 - DELETE,	вращение объектов против часовой стрелки вокруг оси OZ
	{
		//glTranslated(-5, -5, 0);
		glRotated(angle, 0, 0, 1);
		//glTranslated(5, 5, 0);
		display();
	}

}

void processSpecialKeys(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:
			glTranslated(-1, 0, 0);
			display();
			break;
		case GLUT_KEY_RIGHT:
			glTranslated(1, 0, 0);
			display();
			break;
		case GLUT_KEY_UP:
			glTranslated(0, 1, 0);
			display();
			break;
		case GLUT_KEY_DOWN:
			glTranslated(0, -1, 0);
			display();
			break;
		case GLUT_KEY_HOME:				// вращение объектов против часовой стрелки вокруг оси OX
			//glTranslated(50, 0, 1);
			glRotated(-angle, 1, 0, 0);
			//glTranslated(-50, 0, 1);
			display();
			break;
		case GLUT_KEY_END:				// вращение объектов против часовой стрелки вокруг оси OY
			glRotated(angle, 0, 1, 0);
			display();
			break;
		case GLUT_KEY_PAGE_UP:			// вращение объектов по часовой стрелки вокруг всех трех осей одновременно
			glRotated(-angle, 1, 1, 1);
			display();
			break;
		case GLUT_KEY_PAGE_DOWN:		// вращение объектов против часовой стрелки вокруг всех трех осей одновременно
			glRotated(angle, 1, 1, 1);
			display();
			break;
	}
}
