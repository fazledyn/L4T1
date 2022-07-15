#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#include <unistd.h>
#include <GL/glut.h>

#define pi (2 * acos(0.0))
#define toRad(deg) (double)(deg * pi) / 180

#define CAMERA_ROTATE_STEP 10
#define CAMERA_MOVE_STEP 10

#define N_STACKS 100
#define N_SLICES 100

#define RADIUS 40.0
#define PADDING 0.0

struct point
{
	double x, y, z;
};

struct unit_vector
{
	double x, y, z;
};

struct point pos;
struct unit_vector l, r, u;

int drawgrid;
int drawaxes;

double padding;
double radius;

void drawAxes()
{
	if (drawaxes == 1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		{
			glVertex3f(100, 0, 0);
			glVertex3f(-100, 0, 0);

			glVertex3f(0, -100, 0);
			glVertex3f(0, 100, 0);

			glVertex3f(0, 0, 100);
			glVertex3f(0, 0, -100);
		}
		glEnd();
	}
}

void drawGrid()
{
	int i;
	if (drawgrid == 1)
	{
		glColor3f(0.6, 0.6, 0.6); // grey
		glBegin(GL_LINES);
		{
			for (i = -8; i <= 8; i++)
			{
				if (i == 0)
					continue; // SKIP the MAIN axes

				// lines parallel to Y-axis
				glVertex3d(i * 10, -90, 0);
				glVertex3d(i * 10, 90, 0);

				// lines parallel to X-axis
				glVertex3d(-90, i * 10, 0);
				glVertex3d(90, i * 10, 0);
			}
		}
		glEnd();
	}
}

void drawSquare(double a)
{
	glBegin(GL_QUADS);
	{
		glVertex3d(a, a, 0);
		glVertex3d(a, -a, 0);
		glVertex3d(-a, -a, 0);
		glVertex3d(-a, a, 0);
	}
	glEnd();
}

void drawOneFourthCylinder(double radius, double height)
{
	struct point points[N_STACKS + 1][N_SLICES + 1];
	double h, r;

	//	Generate Points
	for (int i = 0; i <= N_STACKS; i++)
	{
		h = height * sin((double)i / (double)N_STACKS * (pi / 2));
		r = radius;
		for (int j = 0; j <= N_SLICES; j++)
		{
			points[i][j].x = r * cos((double)j / (double)N_SLICES * (pi / 2));
			points[i][j].y = r * sin((double)j / (double)N_SLICES * (pi / 2));
			points[i][j].z = h;
		}
	}

	//	Generate Quads
	for (int i = 0; i < N_STACKS; i++)
	{
		for (int j = 0; j < N_SLICES; j++)
		{
			glBegin(GL_QUADS);
			{
				//	Upper
				glVertex3d(points[i][j].x, points[i][j].y, points[i][j].z);
				glVertex3d(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
				glVertex3d(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
				glVertex3d(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);

				//	Lower
				glVertex3d(points[i][j].x, points[i][j].y, -points[i][j].z);
				glVertex3d(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
				glVertex3d(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
				glVertex3d(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
			}
			glEnd();
		}
	}
}

void drawFullCylinder(double padding, double radius)
{
	int mainAngle[4] = {0, 90, 180, 270};
	double x, y, z;
	int angle;

	//	For each rotation
	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		{
			glRotated(mainAngle[i], 0, 0, 1);
			//	1
			x = padding * 1;
			y = padding * 1;
			z = padding * 0;
			angle = 0;

			glPushMatrix();
			{
				glTranslated(x, y, z);
				glRotated(angle, 0, 0, 1);
				drawOneFourthCylinder(radius, padding);
			}
			glPopMatrix();

			//	2
			x = padding * 1;
			y = padding * -1;
			z = padding * 0;
			angle = 270;

			glPushMatrix();
			{
				glTranslated(x, y, z);
				glRotated(angle, 0, 0, 1);
				drawOneFourthCylinder(radius, padding);
			}
			glPopMatrix();

			//	3
			x = padding * 1;
			y = padding * 1;
			z = padding * 0;
			angle = 0;

			glPushMatrix();
			{
				glRotated(90, 1, 0, 0);
				glTranslated(x, y, z);
				glRotated(angle, 0, 0, 1);
				drawOneFourthCylinder(radius, padding);
			}
			glPopMatrix();

			//	4
			x = padding * 1;
			y = padding * -1;
			z = padding * 0;
			angle = 270;

			glPushMatrix();
			{
				glRotated(90, 1, 0, 0);
				glTranslated(x, y, z);
				glRotated(angle, 0, 0, 1);
				drawOneFourthCylinder(radius, padding);
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
}

void drawOneEighthSphere(double radius)
{
	struct point points[N_STACKS + 1][N_SLICES + 1];
	double h, r;

	//	Generate Points
	for (int i = 0; i <= N_STACKS; i++)
	{
		h = radius * sin(((double)i / (double)N_STACKS) * (pi / 2));
		r = radius * cos(((double)i / (double)N_STACKS) * (pi / 2));
		for (int j = 0; j <= N_SLICES; j++)
		{
			points[i][j].x = r * cos(((double)j / (double)N_SLICES) * 0.5 * (pi));
			points[i][j].y = r * sin(((double)j / (double)N_SLICES) * 0.5 * (pi));
			points[i][j].z = h;
		}
	}
	//	Draw Quads
	for (int i = 0; i < N_STACKS; i++)
	{
		for (int j = 0; j < N_SLICES; j++)
		{
			glBegin(GL_QUADS);
			{
				glVertex3d(points[i][j].x, points[i][j].y, points[i][j].z);
				glVertex3d(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
				glVertex3d(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
				glVertex3d(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
			}
			glEnd();
		}
	}
}

void drawHalfSphere(double padding, double radius)
{
	int angle[4] = {0, 270, 90, 180};
	int translation[4][3] = {
		{1, 1, 1},
		{1, -1, 1},
		{-1, 1, 1},
		{-1, -1, 1},
	};

	for (int i = 0; i < 4; i++)
	{
		for (int i = 0; i < 4; i++)
		{
			double x = translation[i][0] * padding;
			double y = translation[i][1] * padding;
			double z = translation[i][2] * padding;
			glPushMatrix();
			{
				glTranslated(x, y, z);
				glRotated(angle[i], 0, 0, 1);
				drawOneEighthSphere(radius);
			}
			glPopMatrix();
		}
	}
}

void drawAllSquare(double padding, double width)
{
	int direction[6][3] = {
		{1, 0, 0},
		{-1, 0, 0},
		{0, 1, 0},
		{0, -1, 0},
		{0, 0, 1},
		{0, 0, -1}};
	int rotation[6][3] = {{0, 1, 0}, {0, 1, 0}, {1, 0, 0}, {1, 0, 0}, {0, 0, 0}, {0, 0, 0}};
	double angle[6] = {90, 90, 90, 90, 90, 90};

	for (int i = 0; i < 6; i++)
	{
		double x = direction[i][0] * (padding + width);
		double y = direction[i][1] * (padding + width);
		double z = direction[i][2] * (padding + width);
		glPushMatrix();
		{
			glTranslated(x, y, z);
			glRotated(angle[i], rotation[i][0], rotation[i][1], rotation[i][2]);
			drawSquare(padding);
		}
		glPopMatrix();
	}
}

void drawSphere(double padding, double radius)
{
	drawHalfSphere(padding, radius);
	glPushMatrix();
	{
		glRotated(180, 1, 0, 0);
		drawHalfSphere(padding, radius);
	}
	glPopMatrix();
}

void drawSmoothCube()
{
	glColor3f(1, 0, 0);
	drawSphere(padding, radius);
	glColor3f(1, 1, 1);
	drawAllSquare(padding, radius);
	glColor3f(0, 1, 0);
	drawFullCylinder(padding, radius);
}

void keyboardListener(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		//	Rotate Left
		l.x = l.x * cos(toRad(CAMERA_ROTATE_STEP)) - r.x * sin(toRad(CAMERA_ROTATE_STEP));
		l.y = l.y * cos(toRad(CAMERA_ROTATE_STEP)) - r.y * sin(toRad(CAMERA_ROTATE_STEP));
		l.z = l.z * cos(toRad(CAMERA_ROTATE_STEP)) - r.z * sin(toRad(CAMERA_ROTATE_STEP));

		r.x = r.x * cos(toRad(CAMERA_ROTATE_STEP)) + l.x * sin(toRad(CAMERA_ROTATE_STEP));
		r.y = r.y * cos(toRad(CAMERA_ROTATE_STEP)) + l.y * sin(toRad(CAMERA_ROTATE_STEP));
		r.z = r.z * cos(toRad(CAMERA_ROTATE_STEP)) + l.z * sin(toRad(CAMERA_ROTATE_STEP));
		break;

	case '2':
		//	Rorate right
		l.x = l.x * cos(toRad(CAMERA_ROTATE_STEP)) - r.x * sin(-toRad(CAMERA_ROTATE_STEP));
		l.y = l.y * cos(toRad(CAMERA_ROTATE_STEP)) - r.y * sin(-toRad(CAMERA_ROTATE_STEP));
		l.z = l.z * cos(toRad(CAMERA_ROTATE_STEP)) - r.z * sin(-toRad(CAMERA_ROTATE_STEP));

		r.x = r.x * cos(toRad(CAMERA_ROTATE_STEP)) + l.x * sin(-toRad(CAMERA_ROTATE_STEP));
		r.y = r.y * cos(toRad(CAMERA_ROTATE_STEP)) + l.y * sin(-toRad(CAMERA_ROTATE_STEP));
		r.z = r.z * cos(toRad(CAMERA_ROTATE_STEP)) + l.z * sin(-toRad(CAMERA_ROTATE_STEP));
		break;

	case '3':
		//	Rotate Up
		u.x = u.x * cos(toRad(CAMERA_ROTATE_STEP)) - l.x * sin(toRad(CAMERA_ROTATE_STEP));
		u.y = u.y * cos(toRad(CAMERA_ROTATE_STEP)) - l.y * sin(toRad(CAMERA_ROTATE_STEP));
		u.z = u.z * cos(toRad(CAMERA_ROTATE_STEP)) - l.z * sin(toRad(CAMERA_ROTATE_STEP));

		l.x = l.x * cos(toRad(CAMERA_ROTATE_STEP)) + u.x * sin(toRad(CAMERA_ROTATE_STEP));
		l.y = l.y * cos(toRad(CAMERA_ROTATE_STEP)) + u.y * sin(toRad(CAMERA_ROTATE_STEP));
		l.z = l.z * cos(toRad(CAMERA_ROTATE_STEP)) + u.z * sin(toRad(CAMERA_ROTATE_STEP));
		break;

	case '4':
		//	Rotate Down
		u.x = u.x * cos(toRad(CAMERA_ROTATE_STEP)) - l.x * sin(-toRad(CAMERA_ROTATE_STEP));
		u.y = u.y * cos(toRad(CAMERA_ROTATE_STEP)) - l.y * sin(-toRad(CAMERA_ROTATE_STEP));
		u.z = u.z * cos(toRad(CAMERA_ROTATE_STEP)) - l.z * sin(-toRad(CAMERA_ROTATE_STEP));

		l.x = l.x * cos(toRad(CAMERA_ROTATE_STEP)) + u.x * sin(-toRad(CAMERA_ROTATE_STEP));
		l.y = l.y * cos(toRad(CAMERA_ROTATE_STEP)) + u.y * sin(-toRad(CAMERA_ROTATE_STEP));
		l.z = l.z * cos(toRad(CAMERA_ROTATE_STEP)) + u.z * sin(-toRad(CAMERA_ROTATE_STEP));
		break;

	case '5':
		//	Rotate Clockwise
		r.x = r.x * cos(toRad(CAMERA_ROTATE_STEP)) - u.x * sin(toRad(CAMERA_ROTATE_STEP));
		r.y = r.y * cos(toRad(CAMERA_ROTATE_STEP)) - u.y * sin(toRad(CAMERA_ROTATE_STEP));
		r.z = r.z * cos(toRad(CAMERA_ROTATE_STEP)) - u.z * sin(toRad(CAMERA_ROTATE_STEP));

		u.x = u.x * cos(toRad(CAMERA_ROTATE_STEP)) + r.x * sin(toRad(CAMERA_ROTATE_STEP));
		u.y = u.y * cos(toRad(CAMERA_ROTATE_STEP)) + r.y * sin(toRad(CAMERA_ROTATE_STEP));
		u.z = u.z * cos(toRad(CAMERA_ROTATE_STEP)) + r.z * sin(toRad(CAMERA_ROTATE_STEP));
		break;

	case '6':
		//	Rotate CounterClockwise
		r.x = r.x * cos(toRad(CAMERA_ROTATE_STEP)) - u.x * sin(-toRad(CAMERA_ROTATE_STEP));
		r.y = r.y * cos(toRad(CAMERA_ROTATE_STEP)) - u.y * sin(-toRad(CAMERA_ROTATE_STEP));
		r.z = r.z * cos(toRad(CAMERA_ROTATE_STEP)) - u.z * sin(-toRad(CAMERA_ROTATE_STEP));

		u.x = u.x * cos(toRad(CAMERA_ROTATE_STEP)) + r.x * sin(-toRad(CAMERA_ROTATE_STEP));
		u.y = u.y * cos(toRad(CAMERA_ROTATE_STEP)) + r.y * sin(-toRad(CAMERA_ROTATE_STEP));
		u.z = u.z * cos(toRad(CAMERA_ROTATE_STEP)) + r.z * sin(-toRad(CAMERA_ROTATE_STEP));
		break;

	default:
		break;
	}
}

void specialKeyListener(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_DOWN:
		//	Move Back
		pos.x -= l.x * CAMERA_MOVE_STEP;
		pos.y -= l.y * CAMERA_MOVE_STEP;
		pos.z -= l.z * CAMERA_MOVE_STEP;
		break;

	case GLUT_KEY_UP:
		//	Move Front
		pos.x += l.x * CAMERA_MOVE_STEP;
		pos.y += l.y * CAMERA_MOVE_STEP;
		pos.z += l.z * CAMERA_MOVE_STEP;
		break;

	case GLUT_KEY_RIGHT:
		//	Move Right
		pos.x += r.x * CAMERA_MOVE_STEP;
		pos.y += r.y * CAMERA_MOVE_STEP;
		pos.z += r.z * CAMERA_MOVE_STEP;
		break;

	case GLUT_KEY_LEFT:
		//	Move Left
		pos.x -= r.x * CAMERA_MOVE_STEP;
		pos.y -= r.y * CAMERA_MOVE_STEP;
		pos.z -= r.z * CAMERA_MOVE_STEP;
		break;

	case GLUT_KEY_PAGE_UP:
		//	Move Up
		pos.x += u.x * CAMERA_MOVE_STEP;
		pos.y += u.y * CAMERA_MOVE_STEP;
		pos.z += u.z * CAMERA_MOVE_STEP;
		break;

	case GLUT_KEY_PAGE_DOWN:
		//	Move Down
		pos.x -= u.x * CAMERA_MOVE_STEP;
		pos.y -= u.y * CAMERA_MOVE_STEP;
		pos.z -= u.z * CAMERA_MOVE_STEP;
		break;

	case GLUT_KEY_HOME:
		if (padding != 0)
		{
			padding -= 1;
			radius += 1;
		}
		break;

	case GLUT_KEY_END:
		if (radius != 0)
		{
			radius -= 1;
			padding += 1;
		}
		break;

	default:
		break;
	}
}

void mouseListener(int button, int state, int x, int y)
{
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) {
				drawaxes = 1 - drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN) {
				drawgrid = 1 - drawgrid;
			}
			break;

		default:
			break;
	}
}

void display()
{
	// clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0); // color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	// load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	// initialize the matrix
	glLoadIdentity();

	// now give three info
	// 1. where is the camera (viewer)?
	// 2. where is the camera looking?
	// 3. Which direction is the camera's UP direction?

	gluLookAt(pos.x, pos.y, pos.z,
			  pos.x + l.x, pos.y + l.y, pos.z + l.z,
			  u.x, u.y, u.z);

	// again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

	drawAxes();
	drawGrid();
	drawSmoothCube();

	glutSwapBuffers();
}

void animate()
{
	// codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init()
{
	// codes for initialization
	drawgrid = 1;
	drawaxes = 1;

	// clear the screen
	glClearColor(0, 0, 0, 0);

	/************************
	/ set-up projection here
	************************/
	padding = PADDING;
	radius = RADIUS;

	pos.x = 150;
	pos.y = 150;
	pos.z = 40;

	l.x = -1 / sqrt(2.00);
	l.y = -1 / sqrt(2.00);
	l.z = 0;

	u.x = 0;
	u.y = 0;
	u.z = 1;

	r.x = -1 / sqrt(2.00);
	r.y = 1 / sqrt(2.00);
	r.z = 0;

	//	Initialization Done

	// load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	// initialize the matrix
	glLoadIdentity();

	// give PERSPECTIVE parameters
	gluPerspective(80, 1, 1, 1000.0);
	// field of view in the Y (vertically)
	// aspect ratio that determines the field of view in the X direction (horizontally)
	// near distance
	// far distance
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Depth, Double buffer, RGB color

	glutCreateWindow("1705066 - Task 1");
	init();

	glEnable(GL_DEPTH_TEST); // enable Depth Testing

	glutDisplayFunc(display); // display callback function
	glutIdleFunc(animate);	  // what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop(); // The main loop of OpenGL

	return 0;
}
