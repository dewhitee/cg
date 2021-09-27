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

void reshape(int w, int h);
void display();
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
	glutCreateWindow("OpenGL lesson 3");
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
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void displayEco()
{
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 0, 0);
	//readFromFile("points_lab3.txt");
	//readFromFile("points_D.txt");
	//readFromFile("points_B.txt");
	//glColor3d(1, 0, 0);
	//for (int i = 0; i < code.size(); i++)
	//{
	//	if (code[i] < 0)
	//	{
	//		moveto(point[i]);
	//	}
	//	else
	//	{
	//		lineto(point[i]);
	//	}
	//}

	drawShapeFromFile("points_D.txt");
	drawShapeFromFile("points_B.txt");
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