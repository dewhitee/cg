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

int lightType = 1;
bool lightingEnabled = true;
bool testPlanesEnabled = false;
bool testSphereEnabled = false;
constexpr bool colorize = true;
constexpr bool useCCW = true;

constexpr float translationValue = 1.f;
constexpr float rotationValue = 5.f;
constexpr Vector3 pivotD = {27, 32, 0.f};

void reshape(int w, int h);
void display();
ObjectStats readFromFile(const char* fileName, const char* coloredFileName, const char* coloredFileNameCCW, 
	vector<Vector3>& outCoords, vector<vector<int>>& outFaces, vector<Vector3>& outColors, bool withColor);

// Keys
void processNormalKeys(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);

// Transformations
void rotateRelative(Vector3 pivot, float angle, Vector3 rotation);
void scaleRelative(Vector3 pivot, float by);

// Light
void initLight();
void setLight();
void disableLight();
void setADC(GLenum light, GLfloat* ambient, GLfloat* diffuse, GLfloat* specular);

// Drawing
void drawGrid();
void drawD();
void drawB();
void drawObject(void(*drawShapeFunc)());
void drawAxis();
void drawFaces(const vector<Vector3>& coords, const vector<vector<int>>& faces, const vector<Vector3>& colors, const ObjectStats stats);
void drawTestPlanes();
void drawTestSphere();

// Normals
Vector3 calculateNormal(Vector3 pointA, Vector3 pointB, Vector3 pointC);

int main(int argc, char* argv[])
{
	//statsD = readFromFile("coordsD3.txt", "coordsD3colored.txt", coordsD, facesD, colorD, colorize);
	statsD = readFromFile("coordsD3.txt", "coordsD3colored.txt", "coordsD3colored_ccw.txt", coordsD, facesD, colorD, colorize);
	//statsB = readFromFile("coordsB3.txt", "coordsB3colored.txt", coordsB, facesB, colorB, colorize);
	statsB = readFromFile("coordsB3.txt", "coordsB3colored.txt", "coordsB3colored_ccw.txt", coordsB, facesB, colorB, colorize);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(800, 600);
	glutCreateWindow("Lab 6");

	initLight();

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
	///glEnable(GL_DITHER);
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

	setLight();

	// Depth test
	if (!useCCW)
	{
		glFrontFace(GL_CW);
	}
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	drawTestPlanes();
	drawTestSphere();
	drawGrid();
	drawAxis();

	glColor3f(0.9, 0.7, 0.9);
	drawObject(drawD);
	drawObject(drawB);

	disableLight();

	glDisable(GL_CULL_FACE);
	glutSwapBuffers();
}

ObjectStats readFromFile(const char* fileName, const char* coloredFileName, const char* coloredFileNameCCW,
	vector<Vector3>& outCoords, vector<vector<int>>& outFaces, vector<Vector3>& outColors, bool withColor)
{
	outCoords.clear();
	outFaces.clear();

	ObjectStats outStats = {};
	float x, y, z;
	Vector3 p;
	ifstream f(withColor ? (useCCW ? coloredFileNameCCW : coloredFileName) : fileName, ios::in);
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
		case '0': { lightType = 0; break; }
		case '1': { lightType = 1; break; }
		case '2': { lightType = 2; break; }
		case '3': { lightType = 3; break; }
		case '4': { lightType = 4; break; }
		case '5': { lightType = 5; break; }
		case 'r':
		case 'R':
		{
			glLoadIdentity();
			break;
		}
		case 't':
		case 'T':
		{
			testPlanesEnabled = !testPlanesEnabled;
			break;
		}
		case 'g':
		case 'G':
		{
			testSphereEnabled = !testSphereEnabled;
			break;
		}
		case 27: // ESC
		{
			exit(0);
			break;
		}
		case 43: // +
		{
			glMatrixMode(GL_MODELVIEW);
			scaleRelative(pivotD, 1.1f);
			break;
		}
		case 45: // -
		{
			glMatrixMode(GL_MODELVIEW);
			scaleRelative(pivotD, 0.9f);
			break;
		}
		case 127: // Delete. Rotating around Z-axis counter-clockwise
		{
			glMatrixMode(GL_MODELVIEW);
			rotateRelative(pivotD, rotationValue, {0.f, 0.f, 1.f});
			break;
		}
		// ADDITIONALS
		case 'w':
		case 'W':
		{
			glMatrixMode(GL_MODELVIEW);
			glTranslated(0, 0, translationValue);
			break;
		}
		case 'A':
		case 'a':
		{
			glMatrixMode(GL_MODELVIEW);
			glTranslated(-translationValue, 0, 0);
			break;
		}
		case 's':
		case 'S':
		{
			glMatrixMode(GL_MODELVIEW);
			glTranslated(0, 0, -translationValue);
			break;
		}
		case 'd':
		case 'D':
		{
			glMatrixMode(GL_MODELVIEW);
			glTranslated(translationValue, 0, 0);
			break;
		}
		case 'x':
		case 'X': // Rotating around X-axis clockwise
		{
			glMatrixMode(GL_MODELVIEW);
			rotateRelative(pivotD, -rotationValue, {1.f, 0.f, 0.f});
			break;
		}
		case 'y':
		case 'Y': // Rotating around Y-axis clockwise
		{
			glMatrixMode(GL_MODELVIEW);
			rotateRelative(pivotD, -rotationValue, {0.f, 1.f, 0.f});
			break;
		}
		case 'z':
		case 'Z': // Rotating around Z-axis clockwise
		{
			glMatrixMode(GL_MODELVIEW);
			rotateRelative(pivotD, -rotationValue, {0.f, 0.f, 1.f});
			break;
		}
	}
	glutPostRedisplay();
}

void processSpecialKeys(int key, int x, int y)
{
	glMatrixMode(GL_MODELVIEW);
	switch (key)
	{
		case GLUT_KEY_UP:
		{
			glTranslated(0, translationValue, 0);
			break;
		}
		case GLUT_KEY_DOWN:
		{
			glTranslated(0, -translationValue, 0);
			break;
		}
		case GLUT_KEY_RIGHT:
		{
			glTranslated(translationValue, 0.f, 0.f);
			break;
		}
		case GLUT_KEY_LEFT:
		{
			glTranslated(-translationValue, 0.f, 0.f);
			break;
		}
		case GLUT_KEY_HOME:
		{
			rotateRelative(pivotD, rotationValue, {1.f, 0.f, 0.f});
			break;
		}
		case GLUT_KEY_END:
		{
			rotateRelative(pivotD, rotationValue, {0.f, 1.f, 0.f});
			break;
		}
		case GLUT_KEY_PAGE_UP: // Rotating around all-axis clockwise
		{
			rotateRelative(pivotD, -rotationValue, {1.f, 1.f, 1.f});
			break;
		}
		case GLUT_KEY_PAGE_DOWN: // Rotating around all-axis counter-clockwise
		{
			rotateRelative(pivotD, rotationValue, {1.f, 1.f, 1.f});
			break;
		}
	}
	glutPostRedisplay();
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

void drawGrid()
{
	glDisable(GL_LIGHTING);

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

	if (lightingEnabled)
	{
		glEnable(GL_LIGHTING);
	}
}

void initLight()
{
	glClearColor(0.1, 0.1, 0.1, 0);
	glEnable(GL_LIGHTING);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_NORMALIZE);
}

void setLight()
{
	glPushMatrix();
	glLoadIdentity();
	GLfloat materialDiffuse[] = {1.0, 1.0, 1.0, 1.0};
	//glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	GLfloat lightDiffuse[] = {0.8, 0.8, 0.8, 1.0};
	GLfloat lightAmbient[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat lightSpecular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat lightShininess[] = {25.f};

	glMaterialfv(GL_FRONT, GL_SHININESS, lightShininess);
	switch (lightType)
	{
		case 0: // No light
		{
			disableLight();
			glDisable(GL_LIGHTING);
			lightingEnabled = false;
			break;
		}
		case 1: // Directional light
		{
			
			glEnable(GL_LIGHTING);
			lightingEnabled = true;
			//GLfloat lightDiffuse[] = {0.4, 0.7, 0.2};
			GLfloat lightPosition[] = {0.0, 0.0, /*-1.0*/1.0, 0.0}; // direction
			glEnable(GL_LIGHT0);
			setADC(GL_LIGHT0, lightAmbient, lightDiffuse, lightSpecular);
			glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
			break;
		}
		case 2: // Point light
		{
			constexpr float attenuationCoefficient = 0.02f;
			glEnable(GL_LIGHTING);
			lightingEnabled = true;
			GLfloat pointLightDiffuse[] = {1.0, 0.843, 0.0}; // gold color
			GLfloat lightPosition[] = {10.0, 25.0, 10.0, 1.0};
			glEnable(GL_LIGHT1);
			setADC(GL_LIGHT1, lightAmbient, pointLightDiffuse, lightSpecular);
			glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
			glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.0);
			glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, attenuationCoefficient * 0.2);
			glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, attenuationCoefficient * 0.4);
			break;
		}
		case 3: // Projector
		{
			glEnable(GL_LIGHTING);
			lightingEnabled = true;
			GLfloat lightPosition[] = {0.0, 0.0, /*1.0*/0.0, 1.0};
			GLfloat lightSpotDirection[] = {-1.0, 1.0, 0.0};
			glEnable(GL_LIGHT2);
			setADC(GL_LIGHT2, lightAmbient, lightDiffuse, lightSpecular);
			glLightfv(GL_LIGHT2, GL_POSITION, lightPosition);
			glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 90); // angle between axis and conuss side
			glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lightSpotDirection);
			glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 2.0);
			break;
		}
		case 4: // Projector 2
		{
			glEnable(GL_LIGHTING);
			lightingEnabled = true;
			GLfloat lightPosition[] = {100.0, 100.0, 100.0, 1.0};
			GLfloat lightSpotDirection[] = {-1.0, -1.0, -1.0};
			glEnable(GL_LIGHT3);
			setADC(GL_LIGHT3, lightAmbient, lightDiffuse, lightSpecular);
			glLightfv(GL_LIGHT3, GL_POSITION, lightPosition);
			glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 5); // angle between axis and conuss side
			glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, lightSpotDirection);
			glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 5.0);
			break;
		}
		case 5: // Projector 3
		{
			glEnable(GL_LIGHTING);
			lightingEnabled = true;
			GLfloat projectorLightDiffuse[] = {1.0, 0.843, 0.0}; // gold color
			GLfloat lightPosition[] = {100.0, 100.0, 100.0, 1.0};
			GLfloat lightSpotDirection[] = {-1.0, -1.0, -1.0};
			glEnable(GL_LIGHT3);
			setADC(GL_LIGHT3, lightAmbient, projectorLightDiffuse, lightSpecular);
			glLightfv(GL_LIGHT3, GL_POSITION, lightPosition);
			glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 45); // angle between axis and conuss side
			glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, lightSpotDirection);
			glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 25.0);
			break;
		}
	}
	glPopMatrix();
}

void disableLight()
{
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
	glDisable(GL_LIGHT3);
	glDisable(GL_LIGHT4);
}

void setADC(GLenum light, GLfloat* ambient, GLfloat* diffuse, GLfloat* specular)
{
	glLightfv(light, GL_AMBIENT, ambient);
	glLightfv(light, GL_DIFFUSE, diffuse);
	glLightfv(light, GL_SPECULAR, specular);
}

void drawD()
{
	drawFaces(coordsD, facesD, colorD, statsD);
}

void drawB()
{
	drawFaces(coordsB, facesB, colorB, statsB);
}

void drawObject(void(*drawShapeFunc)())
{
	glPushMatrix();
	drawShapeFunc();
	glPopMatrix();
}

void drawAxis()
{
	glDisable(GL_LIGHTING);

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

	// X-negative
	glColor4d(0.3, 0, 0, 0.5);
	glBegin(GL_LINES);
	glVertex2i(-axisLength, 0);
	glVertex2i(0, 0);
	glEnd();

	// Y
	glColor4d(0, 1, 0, 0.5);
	glBegin(GL_LINES);
	glVertex2i(0, 0);
	glVertex2i(0, axisLength);
	glEnd();

	// Y-negative
	glColor4d(0, 0.3, 0, 0.5);
	glBegin(GL_LINES);
	glVertex2i(0, -axisLength);
	glVertex2i(0, 0);
	glEnd();

	// Z
	glColor4d(0, 0, 1, 0.5);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, axisLength);
	glEnd();

	// Z-negative
	glColor4d(0, 0, 0.3, 0.5);
	glBegin(GL_LINES);
	glVertex3i(0, 0, -axisLength);
	glVertex3i(0, 0, 0);
	glEnd();

	glPopMatrix();

	if (lightingEnabled)
	{
		glEnable(GL_LIGHTING);
	}
}

void drawFaces(const vector<Vector3>& coords, const vector<vector<int>>& faces, const vector<Vector3>& colors,
	const ObjectStats stats)
{
	const bool colorizePolygon = colors.size() > 0;
	for (int i = 0; i < stats.faceCount; i++)
	{
		if (colorizePolygon)
		{
			glBegin(GL_POLYGON);
			glColor3f(colors[i].x, colors[i].y, colors[i].z);
			GLfloat polygonDiffuse[] = {colors[i].x, colors[i].y, colors[i].z};
			glMaterialfv(GL_FRONT, GL_DIFFUSE, polygonDiffuse);
		}
		else
		{
			glBegin(GL_LINE_LOOP);
		}

		//glNormal3f(0.0, 0.0, -1.0);

		vector<Vector3> triangleCoords;
		for (int j = 0; j < faces[i].size(); j++)
		{
			triangleCoords.push_back(coords[faces[i][j]]);
		}

		Vector3 normal = calculateNormal(triangleCoords[0], triangleCoords[1], triangleCoords[2]);
		GLfloat normalVec[] = {normal.x, normal.y, normal.z};
		glNormal3fv(normalVec);
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

void drawTestPlanes()
{
	constexpr float wallLength = 100.f;

	if (!testPlanesEnabled)
	{
		return;
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	GLfloat wallDiffuse[] = {1.0, 1.0, 1.0, 1.0};

	// X-aligned
	glMaterialfv(GL_FRONT, GL_DIFFUSE, wallDiffuse);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(wallLength, 0.0, 0.0);
		glVertex3f(wallLength, wallLength, 0.0);
		glVertex3f(0.0, wallLength, 0.0);
	glEnd();

	// Z-aligned
	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, wallLength, 0.0);
		glVertex3f(0.0, wallLength, wallLength);
		glVertex3f(0.0, 0.0, wallLength);
	glEnd();

	glPopMatrix();
}

void drawTestSphere()
{
	if (!testSphereEnabled)
	{
		return;
	}

	if (!useCCW)
	{
		glFrontFace(GL_CCW);
	}
	glPushMatrix();
	glLoadIdentity();
	glutSolidSphere(
		20,		// radius
		128,	// slices - divisions around z-axis
		128);	// stacks - divisions along z-axis
	
	if (!useCCW)
	{
		glFrontFace(GL_CW);
	}
	glPopMatrix();
}

Vector3 calculateNormal(Vector3 pointA, Vector3 pointB, Vector3 pointC)
{
	return
	{
		(pointB.y - pointA.y) * (pointC.z - pointA.z) - (pointB.z - pointA.z) * (pointC.y - pointA.y),
		(pointB.z - pointA.z) * (pointC.x - pointA.x) - (pointB.x - pointA.x) * (pointB.z - pointA.z),
		(pointB.x - pointA.x) * (pointC.y - pointA.y) - (pointB.y - pointA.y) * (pointC.x - pointA.x)
	};
}
