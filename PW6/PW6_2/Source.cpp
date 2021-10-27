#include<Windows.h>
#include <GL/glut.h> 
#include<iostream>
#include<fstream>
#include<vector>
using namespace std;

struct Vector3
{
	float x, y, z;
};

struct ObjectStats
{
	int pointCount;
	int faceCount;
} statsD, statsB;

vector<Vector3> coordsD;
vector<Vector3> coordsB;
vector<vector<int>> facesD;
vector<vector<int>> facesB;
vector<Vector3> colorD;
vector<Vector3> colorB;

constexpr bool colorize = true;
constexpr float translationValue = 1.f;
constexpr float rotationValue = 5.f;
constexpr Vector3 pivotD = {27, 32, 0.f};
//constexpr Vector3 pivotB = {0.f, 0.f, 0.f};
//constexpr Vector3 depthVec = {0.f, 0.f, 10.f};

//GLfloat bezierPointsD1[8][3] = {
//	{35, 54, 0},
//	{37, 53, 0},
//	{38, 52, 0},
//	{39, 50, 0},
//	{39.7, 43, 0},
//	{39, 40, 0},
//	{37, 37, 0},
//	{35, 36, 0}
//};
//GLfloat bezierPointsD2[8][3] = {
//	{37, 58, 0},
//	{39, 57, 0},
//	{41, 55, 0},
//	{42, 53, 0},
//	{43, 42, 0},
//	{42, 37, 0},
//	{39, 33, 0},
//	{37, 32, 0}
//};
//GLfloat bezierPointsB1[5][3] = {
//	{66, 54, 0},
//	{68, 52, 0},
//	{68, 48, 0},
//	{66, 46, 0},
//};
//GLfloat bezierPointsB2[5][3] = {
//	{66, 44, 0},
//	{68, 42, 0},
//	{68, 39, 0},
//	{68, 37, 0},
//	{67, 35, 0}
//};
//GLfloat bezierPointsB3[5][3] = {
//	{68, 58, 0},
//	{71, 55, 0},
//	{71, 47, 0},
//	{69, 45, 0},
//};
//GLfloat bezierPointsB4[5][3] = {
//	{69, 45, 0},
//	{71, 43, 0},
//	{71, 34, 0},
//	{69, 32, 0},
//};

void reshape(int w, int h);
void display();
ObjectStats readFromFile(const char* fileName, const char* coloredFileName, vector<Vector3>& outCoords, 
	vector<vector<int>>& outFaces, vector<Vector3>& outColors, bool withColor);

void processNormalKeys(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);

void rotateRelative(Vector3 pivot, float angle, Vector3 rotation);
void scaleRelative(Vector3 pivot, float by);

void copyShiftedPoints(GLfloat points[][3], GLfloat copyFrom[][3], size_t size, Vector3 shiftBy);
void drawGrid();

//void drawShapeFromFile(const char* fileName);
void drawD();
void drawB();
void drawObject(void(*drawShapeFunc)(), Vector3 pivot, float angle);
void drawAxis();
void drawFaces(const vector<Vector3>& coords, const vector<vector<int>>& faces, const vector<Vector3>& colors, const ObjectStats stats);
void drawBezier(const GLfloat* points, GLint pointCount = 5, float step = 1000.f);

int main(int argc, char* argv[])
{
	//statsD = readFromFile("coordsD.txt", coordsD, facesD);
	statsD = readFromFile("coordsD3.txt", "coordsD3colored.txt", coordsD, facesD, colorD, colorize);
	statsB = readFromFile("coordsB3.txt", "coordsB3colored.txt", coordsB, facesB, colorB, colorize);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	// Depth test initialization  NOT WORKING HERE!!!!!!!!!!!
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_NEVER);
	//glDepthRange(0.0f, 1.0f);

	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL lesson 7");

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

	// Depth test initialization !!!!!!!!!!!!!!!!!!!!!!
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	//glOrtho(0, 10, 0, 10, -10, 10);
	//glOrtho(0, w, 0, h, w, h);
	///glOrtho(-100, 100, -100, 100, -100, 100);
	gluPerspective(
		90,		// FOV (y)
		1,		// aspect
		50,		// zNear => greater value - earlier cut when too near
		1000);	// zFar

	// Set view point (camera)
	gluLookAt(
		100,	// eyeX			- scaling adds distance from the zero coords
		100,	// eyeY 
		100,	// eyeZ
		0,//pivotD.x,		// centerX
		0,//pivotD.y,		// centerY 
		0,//pivotD.z,		// centerZ 
		0,		// upX
		1,		// upY			- with -1 Y will point at the bottom, X to the left and Z to the right
		0);		// upZ

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display()
{
	glClearColor(0.1, 0.1, 0.1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.9, 0.9, 0.9);

	// Depth test
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	drawGrid();
	drawAxis();
	glColor3f(0.9, 0.7, 0.9);
	drawObject(drawD, pivotD, 0.f);
	drawObject(drawB, pivotD, 0.f);

	glutSwapBuffers();

	glDisable(GL_CULL_FACE);
}

ObjectStats readFromFile(const char* fileName, const char* coloredFileName, vector<Vector3>& outCoords, 
	vector<vector<int>>& outFaces, vector<Vector3>& outColors, bool withColor)
{
	outCoords.clear();
	outFaces.clear();

	ObjectStats outStats = {};
	float x, y, z;
	Vector3 p;
	ifstream f(withColor ? coloredFileName : fileName, ios::in);
	f >> outStats.pointCount >> outStats.faceCount;
	for (int i = 0; i < outStats.pointCount; i++)
	{
		f >> p.x >> p.y >> p.z;
		outCoords.push_back(p);
	}
	outFaces.resize(outStats.faceCount);
	if (withColor)
	{
		outColors.resize(outStats.faceCount);
		for (int i = 0; i < outStats.faceCount; i++)
		{
			int n;
			f >> n;
			Vector3 color; // rgb
			f >> outColors[i].x >> outColors[i].y >> outColors[i].z;
			for (int j = 0; j < n; j++)
			{
				int p;
				f >> p;
				outFaces[i].push_back(p);
			}
		}
	}
	else
	{
		for (int i = 0; i < outStats.faceCount; i++)
		{
			int n;
			f >> n;
			for (int j = 0; j < n; j++)
			{
				int p;
				f >> p;
				outFaces[i].push_back(p);
			}
		}
	}
	f.close();
	return outStats;
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
		case 127: // Delete. Rotating around Z-axis counter-clockwise
		{
			glMatrixMode(GL_MODELVIEW);
			rotateRelative(pivotD, rotationValue, {0.f, 0.f, 1.f});
			display();
			break;
		}
		// ADDITIONALS
		case 'w':
		case 'W':
		{
			glMatrixMode(GL_MODELVIEW);
			glTranslated(0, 0, translationValue);
			display();
			break;
		}
		case 'A':
		case 'a':
		{
			glMatrixMode(GL_MODELVIEW);
			glTranslated(-translationValue, 0, 0);
			display();
			break;
		}
		case 's':
		case 'S':
		{
			glMatrixMode(GL_MODELVIEW);
			glTranslated(0, 0, -translationValue);
			display();
			break;
		}
		case 'd':
		case 'D':
		{
			glMatrixMode(GL_MODELVIEW);
			glTranslated(translationValue, 0, 0);
			display();
			break;
		}
		case 'x':
		case 'X': // Rotating around X-axis clockwise
		{
			glMatrixMode(GL_MODELVIEW);
			rotateRelative(pivotD, -rotationValue, {1.f, 0.f, 0.f});
			display();
			break;
		}
		case 'y':
		case 'Y': // Rotating around Y-axis clockwise
		{
			glMatrixMode(GL_MODELVIEW);
			rotateRelative(pivotD, -rotationValue, {0.f, 1.f, 0.f});
			display();
			break;
		}
		case 'z':
		case 'Z': // Rotating around Z-axis clockwise
		{
			glMatrixMode(GL_MODELVIEW);
			rotateRelative(pivotD, -rotationValue, {0.f, 0.f, 1.f});
			display();
			break;
		}
	}
}

void processSpecialKeys(int key, int x, int y)
{
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
			//glRotated(rotationValue, 1, 1, 1);
			//rotateRelative(pivotD, rotationValue, {1, 1, 1});
			glTranslated(0, -translationValue, 0);
			display();
			break;
		}
		case GLUT_KEY_RIGHT:
		{
			glMatrixMode(GL_MODELVIEW);
			glTranslated(translationValue, 0.f, 0.f);
			display();
			break;
		}
		case GLUT_KEY_LEFT:
		{
			glMatrixMode(GL_MODELVIEW);
			glTranslated(-translationValue, 0.f, 0.f);
			display();
			break;
		}
		case GLUT_KEY_HOME:
		{
			glMatrixMode(GL_MODELVIEW);
			rotateRelative(pivotD, rotationValue, {1.f, 0.f, 0.f});
			display();
			break;
		}
		case GLUT_KEY_END:
		{
			glMatrixMode(GL_MODELVIEW);
			rotateRelative(pivotD, rotationValue, {0.f, 1.f, 0.f});
			display();
			break;
		}
		case GLUT_KEY_PAGE_UP: // Rotating around all-axis clockwise
		{
			rotateRelative(pivotD, -rotationValue, {1.f, 1.f, 1.f});
			display();
			break;
		}
		case GLUT_KEY_PAGE_DOWN: // Rotating around all-axis counter-clockwise
		{
			rotateRelative(pivotD, rotationValue, {1.f, 1.f, 1.f});
			display();
			break;
		}
	}
}

void rotateRelative(Vector3 pivot, float angle, Vector3 rotation)
{
	glTranslatef(pivot.x, pivot.y, pivot.z);
	glRotatef(angle, rotation.x, rotation.y, rotation.z);
	glTranslatef(-pivot.x, -pivot.y, -pivot.z);
}

void scaleRelative(Vector3 pivot, float by)
{
	glTranslated(pivot.x, pivot.y, pivot.z);
	glScaled(by, by, by);
	glTranslated(-pivot.x, -pivot.y, -pivot.z);
}

void copyShiftedPoints(GLfloat points[][3], GLfloat copyFrom[][3], size_t size, Vector3 shiftBy)
{
	for (int i = 0; i < size; i++)
	{
		points[i][0] = copyFrom[i][0] + shiftBy.x;
		points[i][1] = copyFrom[i][1] + shiftBy.y;
		points[i][2] = copyFrom[i][2] + shiftBy.z;
	}
}

void drawGrid()
{
	constexpr float cellSize = 100;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_LINES);
	for (int i = 1; i <= 100; i++)
	{
		if (i == 0)
		{
			glColor3f(.6, .3, .3);
		}
		else if (i % 10 == 0)
		{
			glColor3f(.15, .15, .15);
		}
		else
		{
			glColor3f(.05, .05, .05);
		};
		glVertex3f(i, 0, 0);
		glVertex3f(i, 0, cellSize);
		if (i == 0)
		{
			glColor3f(.6, .3, .3);
		}
		else if (i % 10 == 0)
		{
			glColor3f(.15, .15, .15);
		}
		else
		{
			glColor3f(.05, .05, .05);
		};
		glVertex3f(0, 0, i);
		glVertex3f(cellSize, 0, i);
	};
	glEnd();
	glPopMatrix();
}

void drawD()
{
	//drawShapeFromFile("points_D_lines.txt");
	drawFaces(coordsD, facesD, colorD, statsD);

	///drawBezier(*bezierPointsD1, 8);
	///GLfloat d1[8][3];
	///copyShiftedPoints(d1, bezierPointsD1, 8, depthVec);
	///drawBezier(*d1, 8);

	///drawBezier(*bezierPointsD2, 8);
	///GLfloat d2[8][3];
	///copyShiftedPoints(d2, bezierPointsD2, 8, depthVec);
	///drawBezier(*d2, 8);
}

void drawB()
{
	//drawShapeFromFile("points_B_lines.txt");
	drawFaces(coordsB, facesB, colorB, statsB);
	///drawBezier(*bezierPointsB1, 4);
	///drawBezier(*bezierPointsB2);
	///drawBezier(*bezierPointsB3, 4);
	///drawBezier(*bezierPointsB4, 4);
}

void drawObject(void(*drawShapeFunc)(), Vector3 pivot, float angle)
{
	glPushMatrix();
	drawShapeFunc();
	glPopMatrix();
}

void drawAxis()
{
	constexpr int axisLength = 1000;

	glMatrixMode(GL_MODELVIEW);
	glLineWidth(1);
	glPushMatrix();
	// X
	glLoadIdentity();
	glColor4d(1, 0, 0, 0.5);
	glBegin(GL_LINES);
	glVertex2i(0, 0);
	glVertex2i(axisLength, 0);
	glEnd();

	// Y
	glColor4d(0, 1, 0, 0.5);
	glBegin(GL_LINES);
	glVertex2i(0, 0);
	glVertex2i(0, axisLength);
	glEnd();

	// Z
	glColor4d(0, 0, 1, 0.5);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, axisLength);
	glEnd();

	glPopMatrix();
}

void drawFaces(const vector<Vector3>& coords, const vector<vector<int>>& faces, const vector<Vector3>& colors, 
	const ObjectStats stats)
{
	const bool colorizedPolygon = colors.size() > 0;
	for (int i = 0; i < stats.faceCount; i++)
	{
		if (colorizedPolygon)
		{
			glBegin(GL_POLYGON);
			glColor3f(colors[i].x, colors[i].y, colors[i].z);
		}
		else
		{
			glBegin(GL_LINE_LOOP);
		}

		for (int j = 0; j < faces[i].size(); j++)
		{
			const float coordX = coords[faces[i][j]].x;
			const float coordY = coords[faces[i][j]].y;
			const float coordZ = coords[faces[i][j]].z;
			glVertex3f(coordX, coordY, coordZ);
		}
		glEnd();
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
