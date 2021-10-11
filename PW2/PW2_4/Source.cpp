#include<Windows.h>

#if defined(linux) || defined(_WIN32)
#include <GL/glut.h> /*For Linux and Windows*/
#else
#include <GLUT/GLUT.h> /*For Mac OS*/
#endif

#include<vector>
#include<fstream>

using namespace std;

struct Point
{
	float x, y;
};

GLfloat bezierPointsD1[8][3] = {
	{35, 54},
	{37, 53},
	{38, 52},
	{39, 50},
	{39.7, 43},
	{39, 40},
	{37, 37},
	{35, 36}
};

GLfloat bezierPointsD2[8][3] = {
	{37, 58},
	{39, 57},
	{41, 55},
	{42, 53},
	{43, 42},
	{42, 37},
	{39, 33},
	{37, 32}
};

GLfloat bezierPointsB1[5][3] = {
	{66, 54},
	{68, 52},
	{68, 48},
	{66, 46},
};

GLfloat bezierPointsB2[5][3] = {
	{66, 44},
	{68, 42},
	{68, 39},
	{68, 37},
	{67, 35}
};

GLfloat bezierPointsB3[5][3] = {
	{68, 58},
	{71, 55},
	{71, 47},
	{69, 45},
};

GLfloat bezierPointsB4[5][3] = {
	{69, 45},
	{71, 43},
	{71, 34},
	{69, 32},
};

void reshape(int w, int h);
void displayEco();
void readFromFile(const char* fileName);
void lineto(Point p);
void moveto(Point p);
void drawShapeFromFile(const char* fileName);
vector<Point> point;
vector<int> code;
Point currentPoint;

int main(int argc, char* argv[])
{
	currentPoint.x = 0; currentPoint.y = 0;
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
	drawShapeFromFile("points_D_lines.txt");
	drawShapeFromFile("points_B_lines.txt");

	// --- Draw curvature of initials using Bezier curve
	auto drawBezier = [](const GLfloat* points, GLint pointCount = 5, float step = 1000.f)
	{
		glMap1f(GL_MAP1_VERTEX_3,
			0.0,		// Min t-value
			1.0,		// Max t-value
			3,			// Dimension of point (count of elements in a row)
			pointCount,	// Total points count
			points		// Control points array
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

	drawBezier(*bezierPointsD1, 8);
	drawBezier(*bezierPointsD2, 8);
	drawBezier(*bezierPointsB1, 4);
	drawBezier(*bezierPointsB2);
	drawBezier(*bezierPointsB3, 4);
	drawBezier(*bezierPointsB4, 4);
	glFlush();
}

void readFromFile(const char* fileName)
{
	fstream f(fileName, ios::in);
	int pointNumber;
	float x, y; Point p;
	f >> pointNumber;
	for (int i = 0; i < pointNumber; i++)
	{
		f >> p.x >> p.y;
		point.push_back(p);
	}
	int movesNumber, m;
	f >> movesNumber;
	for (int i = 0; i < movesNumber; i++)
	{
		f >> m; code.push_back(m);
	}
	f.close();
}

void moveto(Point p)
{
	currentPoint.x = p.x; currentPoint.y = p.y;
}

void lineto(Point p)
{
	glBegin(GL_LINES);
	glVertex2f(currentPoint.x, currentPoint.y);
	glVertex2f(p.x, p.y);
	glEnd();
	currentPoint.x = p.x; currentPoint.y = p.y;
}

void drawShapeFromFile(const char* fileName)
{
	printf("\nDrawing shape from file %s\n", fileName);
	readFromFile(fileName);
	glColor3d(1, 0, 0);
	for (int i = 0; i < code.size(); i++)
	{
		const int pointIndex = abs(code[i]) - 1;
		printf("Code[%d]=%d, Point[%d]=( x=%.2f, y=%.2f )\n", i, code[i], pointIndex, point[pointIndex].x, point[pointIndex].y);
		if (code[i] < 0)
		{
			moveto(point[pointIndex]);
		}
		else
		{
			lineto(point[pointIndex]);
		}
	}
	code.clear();
	point.clear();
}
