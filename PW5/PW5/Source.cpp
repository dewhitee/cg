#if defined(linux) || defined(_WIN32)
#include <GL/glut.h> /*For Linux and Windows*/
#else
#include <GLUT/GLUT.h> /*For Mac OS*/
#endif

struct Vector3
{
	float x, y, z;
};

void reshape(int w, int h);
void display();
void processNormalKeys(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);

void drawObject();
void drawAxis();

void rotateRelative(Vector3 pivot, float angle, Vector3 rotation);
void scaleRelative(Vector3 pivot, float by);

constexpr float translationValue = 1.f;
constexpr float rotationValue = 5.f;
constexpr Vector3 pivot = {0.f, 0.f, 0.f};

using namespace std;

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(800, 600);
	glutCreateWindow("Lab 4");

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
	//glOrtho(-15, 15, /*-15, 15*/-60, 60, -15, 15);
	//glOrtho(-15, 15, -15, 15, -15, 15);

	//glOrtho(-100, 100, -100, 100, -100, 100);
	gluPerspective(
		60,		// FOV (y)
		1,		// aspect
		-1,		// zNear => lesser value - earlier cut when too near
		10);	// zFar

	// Set view point (camera)
	gluLookAt(
		10,		// eyeX			- scaling adds distance from the zero coords
		10,		// eyeY 
		10,		// eyeZ
		0,		// centerX
		0,		// centerY 
		0,		// centerZ 
		0,		// upX
		1,		// upY			- with -1 Y will point at the bottom, X to the left and Z to the right
		0);		// upZ

	//glOrtho(0, w, 0, h, 0, w);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(1, 1, 1, 0);
}

void display()
{
	glClearColor(0.1, 0.1, 0.1, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.9, 0.9, 0.9);

	drawAxis();
	drawObject();

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27: // Escape
		{
			exit(0);
			break;
		}
		case 127: // Delete. Rotating around Z-axis counter-clockwise
		{
			glMatrixMode(GL_MODELVIEW);
			rotateRelative({}, rotationValue, {0.f, 0.f, 1.f});
			display();
			break;
		}
		case 43: // +
		{
			glMatrixMode(GL_MODELVIEW);
			scaleRelative({}, 1.1f);
			display();
			break;
		}
		case 45: // -
		{
			glMatrixMode(GL_MODELVIEW);
			scaleRelative({}, 0.9f);
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
		case 's':
		case 'S':
		{
			glMatrixMode(GL_MODELVIEW);
			glTranslated(0, 0, -translationValue);
			display();
			break;
		}
		case 'x':
		case 'X': // Rotating around X-axis clockwise
		{
			glMatrixMode(GL_MODELVIEW);
			rotateRelative(pivot, -rotationValue, {1.f, 0.f, 0.f});
			display();
			break;
		}
		case 'y':
		case 'Y': // Rotating around Y-axis clockwise
		{
			glMatrixMode(GL_MODELVIEW);
			rotateRelative(pivot, -rotationValue, {0.f, 1.f, 0.f});
			display();
			break;
		}
		case 'z':
		case 'Z': // Rotating around Z-axis clockwise
		{
			glMatrixMode(GL_MODELVIEW);
			rotateRelative(pivot, -rotationValue, {0.f, 0.f, 1.f});
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
			glTranslated(0, translationValue, 0/*1*/); // will be strange with no lookAt
			display();
			break;
		}
		case GLUT_KEY_DOWN:
		{
			glMatrixMode(GL_MODELVIEW);
			glTranslated(0, -translationValue, 0/*1*/); // will be strange if no lookAt
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
		case GLUT_KEY_HOME: // Rotating around X-axis counter-clockwise
		{
			glMatrixMode(GL_MODELVIEW);
			rotateRelative(pivot, rotationValue, {1.f, 0.f, 0.f});
			display();
			break;
		}
		case GLUT_KEY_END: // Rotating around Y-axis counter-clockwise
		{
			glMatrixMode(GL_MODELVIEW);
			rotateRelative(pivot, rotationValue, {0.f, 1.f, 0.f});
			display();
			break;
		}
		case GLUT_KEY_PAGE_UP: // Rotating around all-axis clockwise
		{
			rotateRelative(pivot, -rotationValue, {1.f, 1.f, 1.f});
			display();
			break;
		}
		case GLUT_KEY_PAGE_DOWN: // Rotating around all-axis counter-clockwise
		{
			rotateRelative(pivot, rotationValue, {1.f, 1.f, 1.f});
			display();
			break;
		}
	}
}

void drawObject()
{
	//glLineWidth(2);

	constexpr float grayscale = 0.6f;
	glColor3f(grayscale, grayscale, grayscale);
	glutWireTeapot(3);
	
	//glutSolidSphere(
	//	1,		// radius
	//	16,		// slices - divisions around z-axis
	//	64);	// stacks - divisions along z-axis
	//glutWireSphere(1, 12, 12);
	
	//glutSolidCube(1); // size = 1
	//glutWireCube(2);  // size = 2

	//glutSolidCone(		// draws along z-axis
	//	12, // base		- radius of a basement (z=0 plane)
	//	5,  // height
	//	5,  // slices
	//	5); // stacks
	//glutWireCone(12, 5, 5, 5);
	
	//glutSolidTorus(
	//	1,	// inner radius
	//	2,	// outer radius
	//	16,	// nsides - count of faces in a rings
	//	16	// rings - count of radial divisions of a torus
	//);
	//glutWireTorus(1, 2, 16, 16);

	//glutSolidTetrahedron(); // radius equals to 1
	//glutWireTetrahedron();

	//glutSolidOctahedron(); // radius equals to 1
	//glutWireOctahedron();

	//glutSolidDodecahedron();	// radius equals to sqrt(3)
	//glutWireDodecahedron();

	//glutSolidIcosahedron();	// radius equals to 1
	//glutWireIcosahedron();
}

void drawAxis()
{
	constexpr int axisLength = 10;

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
