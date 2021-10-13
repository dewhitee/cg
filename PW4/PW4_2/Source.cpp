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
void display();
void readFromFile(const char* fileName);
void lineto(Point p);
void moveto(Point p);
void drawShapeFromFile(const char* fileName);
void processNormalKeys(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);
void drawBezier(const GLfloat* points, GLint pointCount = 5, float step = 1000.f);

void drawD();
void drawB();
void drawObject(void(*drawShapeFunc)(), Point pivot, float angle);

void rotateRelative(Point point, float angle);
void scaleRelative(Point point, float by);

vector<Point> point;
vector<int> code;
Point currentPoint;

Point pivotD = {27, 32};
Point pivotB = {57, 32};

float currentAngle = 0.f;

int main(int argc, char* argv[])
{
	currentPoint.x = 0; currentPoint.y = 0;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Lab 3");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
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
	//gluOrtho2D(0, 100, 0, 100);
	gluOrtho2D(0, w, 0, h);
	//glOrtho(0, w, 0, h, -w, w);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display()
{
	glLineWidth(3);
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 0, 0);
	drawObject(drawD, pivotD, currentAngle);
	drawObject(drawB, pivotB, -currentAngle);
	glutSwapBuffers();
}

void drawD()
{
	drawShapeFromFile("points_D_lines.txt");
	drawBezier(*bezierPointsD1, 8);
	drawBezier(*bezierPointsD2, 8);
}

void drawB()
{
	drawShapeFromFile("points_B_lines.txt");
	drawBezier(*bezierPointsB1, 4);
	drawBezier(*bezierPointsB2);
	drawBezier(*bezierPointsB3, 4);
	drawBezier(*bezierPointsB4, 4);
}

void drawObject(void(*drawShapeFunc)(), Point pivot, float angle)
{
	glPushMatrix();
		rotateRelative(pivot, angle);
		drawShapeFunc();
	glPopMatrix();
}

void rotateRelative(Point point, float angle)
{
	glTranslatef(point.x, point.y, 0.f);
	glRotatef(angle, 0.f, 0.f, 1.f);
	glTranslatef(-point.x, -point.y, 0.f);
}

void scaleRelative(Point point, float by)
{
	glTranslated(point.x, point.y, 0.0);
	glScaled(by, by, 0.0);
	glTranslated(-point.x, -point.y, 0.0);
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

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27: // ESC
		{
			exit(0);
			break;
		}
		case 43: // +
		{
			glMatrixMode(GL_MODELVIEW);
			scaleRelative(pivotD, 1.1f);
			display();
			break;
		}
		case 45: // -
		{
			glMatrixMode(GL_MODELVIEW);
			scaleRelative(pivotD, 0.9f);
			display();
			break;
		}
	}
}

void processSpecialKeys(int key, int x, int y)
{
	constexpr float translationValue = 20.f;
	constexpr float rotationValue = 15.f;

	switch (key)
	{
		case GLUT_KEY_UP:
		{
			glMatrixMode(GL_MODELVIEW);
			glTranslated(0, translationValue, 0);
			display();
			break;
		}
		case GLUT_KEY_DOWN:
		{
			glMatrixMode(GL_MODELVIEW);
			glTranslated(0, -translationValue, 0);
			display();
			break;
		}
		case GLUT_KEY_LEFT:
		{
			glMatrixMode(GL_MODELVIEW);
			glTranslated(-translationValue, 0, 0);
			display();
			break;
		}
		case GLUT_KEY_RIGHT:
		{
			glMatrixMode(GL_MODELVIEW);
			glTranslated(translationValue, 0, 0);
			display();
			break;
		}
		case GLUT_KEY_HOME:
		{
			glMatrixMode(GL_MODELVIEW);
			rotateRelative(pivotD, rotationValue);
			display();
			break;
		}
		case GLUT_KEY_END:
		{
			glMatrixMode(GL_MODELVIEW);
			rotateRelative(pivotD, -rotationValue);
			display();
			break;
		}
		case GLUT_KEY_PAGE_UP:
		{
			// Rotate D counter-clockwise around some point
			// Rotate B clockwise around some point
			currentAngle += rotationValue;
			display();
			break;
		}
		case GLUT_KEY_PAGE_DOWN:
		{
			// Rotate D clockwise around some point
			// Rotate B counter-clockwise around some point
			currentAngle -= rotationValue;
			display();
			break;
		}
	}
}

void drawBezier(const GLfloat* points, GLint pointCount, float step)
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
}
