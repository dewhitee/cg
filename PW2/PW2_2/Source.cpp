#include<Windows.h>

#if defined(linux) || defined(_WIN32)
#include <GL/glut.h> /*For Linux and Windows*/
#else
#include <GLUT/GLUT.h> /*For Mac OS*/
#endif

#include<vector>
#include<fstream>

using namespace std;

GLfloat firstBezierPoints[5][3] = {
	{50, 20, 0},
	{20, 75, 0},
	{35, 80, 0},
	{50, 65, 0},
	{50, 65, 0}
};

GLfloat secondBezierPoints[5][3] = {
	{50, 20, 0},
	{80, 75, 0},
	{65, 80, 0},
	{50, 65, 0},
	{50, 65, 0}
};

void reshape(int w, int h);
void displayEco();

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Lab 2");
	glutDisplayFunc(/*display*/displayEco);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 100, 0, 100);
	//gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void displayEco()
{
	glLineWidth(3);
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 0, 0);

	// --- Draw heart shape using Bezier curve
	auto drawBezier = [](const GLfloat* points, float step = 1000.f)
	{
		glMap1f(GL_MAP1_VERTEX_3,
			0.0,	// Min t-value
			1.0,	// Max t-value
			3,		// Dimension of point (count of elements in a row)
			5,		// Total points count
			points	// Control points array
		);
		glEnable(GL_MAP1_VERTEX_3);
		glColor3d(1, 0, 0);
		glBegin(GL_LINE_STRIP);
		for (float t = 0; t <= 1; t += 1 / step)
		{
			glEvalCoord1f(t);
		}
		glEnd();
	};

	drawBezier(*firstBezierPoints);
	drawBezier(*secondBezierPoints);
	glFlush();
}
