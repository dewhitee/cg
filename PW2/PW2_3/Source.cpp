#include<Windows.h>

#if defined(linux) || defined(_WIN32)
#include <GL/glut.h> /*For Linux and Windows*/
#else
#include <GLUT/GLUT.h> /*For Mac OS*/
#endif

#include<vector>
#include<fstream>

using namespace std;

GLfloat nodes[10] = {
	0, 0, 0, 0, 0,
	1, 1, 1, 1, 1
};

GLfloat firstNurbsPoints[5][3] = {
	{50, 20, 0},
	{20, 75, 0},
	{35, 80, 0},
	{50, 65, 0},
	{50, 65, 0}
};

GLfloat secondNurbsPoints[5][3] = {
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
	GLUnurbs* nurbs = gluNewNurbsRenderer();
	glLineWidth(3);

	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 0, 0);

	// --- Draw heart shape using Nurbs
	auto drawNurbs = [&](GLfloat* points)
	{
		gluBeginCurve(nurbs);
		gluNurbsCurve(
			nurbs,				// Nurbs object
			10,					// Count of parametrized curve knots (x2 count of points)
			nodes,				// Array of knots values
			3,					// Shift - count of floats in each array element
			points,				// Array of control points
			5,					// Count of control points
			GL_MAP1_VERTEX_3	// Type of two-dimensional calculator
		); 
		gluEndCurve(nurbs);
	};

	drawNurbs(*firstNurbsPoints);
	drawNurbs(*secondNurbsPoints);

	glutSwapBuffers();
	gluDeleteNurbsRenderer(nurbs);
	glFlush();
}
