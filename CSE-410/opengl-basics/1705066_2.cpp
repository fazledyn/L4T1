#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#include <unistd.h>
#include <GL/glut.h>

#define pi (2 * acos(0.0))
#define toRad(deg) (double)(deg * pi) / 180

#define CAMERA_ROTATE_STEP 5
#define CAMERA_MOVE_STEP 10

#define WHEEL_RADIUS 40.0
#define WHEEL_WIDTH 10.0

#define WHEEL_ROTATE_STEP 2
#define WHEEL_MOVE_STEP 5

#define N_SEGMENTS 100

struct point
{
	double x, y, z;
};

struct unit_vector
{
	double x, y, z;
};

int drawgrid;
int drawaxes;

double cameraDistance;
double cameraHeight;
double cameraAngle;
double angle;

struct point center;

//	`wheelMoveAngle` refers to wheel's angle in the XY plane. For movement using 'A' and 'D'
double wheelMoveAngle;
//	`wheelRotateAngle` refers to wheel's angle along the Z axis. For rotation using 'W' and 'S'
double wheelRotateAngle;

void drawAxes()
{
	if (drawaxes == 1)
	{
		glBegin(GL_LINES);
		{
			glColor3f(1, 0, 0);
			glVertex3f(200, 0, 0);
			glVertex3f(-200, 0, 0);

			glColor3f(0, 1, 0);
			glVertex3f(0, -200, 0);
			glVertex3f(0, 200, 0);

			glColor3f(0, 0, 1);
			glVertex3f(0, 0, 200);
			glVertex3f(0, 0, -200);
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
			for (i = -15; i <= 15; i++)
			{
				if (i == 0)
					continue; // SKIP the MAIN axes

				// lines parallel to Y-axis
				glVertex3f(i * 10, -150, 0);
				glVertex3f(i * 10, 150, 0);

				// lines parallel to X-axis
				glVertex3f(-150, i * 10, 0);
				glVertex3f(150, i * 10, 0);
			}
		}
		glEnd();
	}
}

void drawRectangle(double height, double width)
{
	glBegin(GL_QUADS);
	{
		glVertex3f(width / 2, height / 2, 0);
		glVertex3f(width / 2, -height / 2, 0);
		glVertex3f(-width / 2, -height / 2, 0);
		glVertex3f(-width / 2, height / 2, 0);
	}
	glEnd();
}

void drawTire()
{
	struct point points[N_SEGMENTS + 1];

	//	Generate Points
	for (int i = 0; i <= N_SEGMENTS; i++)
	{
		points[i].x = WHEEL_RADIUS * cos((double)i / (double)N_SEGMENTS * 2 * pi);
		points[i].y = WHEEL_RADIUS * sin((double)i / (double)N_SEGMENTS * 2 * pi);
	}

	glBegin(GL_QUADS);

	//	Draw Quads
	for (int i = 0; i < N_SEGMENTS; i++)
	{
		double shade;
		if (i < N_SEGMENTS / 2)
			shade = 2 * (double)i / (double)N_SEGMENTS;
		else
			shade = 2 * (1.0 - (double)i / (double)N_SEGMENTS);

		glColor3f(shade, shade, shade);

		glVertex3f(points[i + 1].x, points[i + 1].y, WHEEL_WIDTH / 2);
		glVertex3f(points[i].x, points[i].y, WHEEL_WIDTH / 2);

		glVertex3f(points[i].x, points[i].y, -WHEEL_WIDTH / 2);
		glVertex3f(points[i + 1].x, points[i + 1].y, -WHEEL_WIDTH / 2);
	}
	glEnd();
}

void drawWheel()
{
	//	Wheel Axel
	glPushMatrix();
	{
		glRotated(90, 1, 0, 0);
		glRotated(90, 0, 1, 0);
		glRotated(90, 0, 0, 1);

		glColor3f(0.4, 0.4, 0.4);
		drawRectangle(WHEEL_WIDTH, 2 * WHEEL_RADIUS);
	}
	glPopMatrix();

	//	Wheel Axel
	glPushMatrix();
	{
		glRotated(180, 0, 0, 1);

		glColor3f(0.4, 0.4, 0.4);
		drawRectangle(WHEEL_WIDTH, 2 * WHEEL_RADIUS);
	}
	glPopMatrix();

	//	Wheel Tire
	glPushMatrix();
	{
		//	Rotate so that it's perpendicular to XY plane
		glRotated(90, 1, 0, 0);
		drawTire();
	}
	glPopMatrix();
}

void keyboardListener(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		center.x += WHEEL_MOVE_STEP * cos(toRad(wheelMoveAngle));
		center.y += WHEEL_MOVE_STEP * sin(toRad(wheelMoveAngle));
		wheelRotateAngle += 360 * (WHEEL_MOVE_STEP / (2 * pi * WHEEL_RADIUS));
		break;

	case 's':
		center.x -= WHEEL_MOVE_STEP * cos(toRad(wheelMoveAngle));
		center.y -= WHEEL_MOVE_STEP * sin(toRad(wheelMoveAngle));
		wheelRotateAngle -= 360 * (WHEEL_MOVE_STEP / (2 * pi * WHEEL_RADIUS));
		break;

	case 'a':
		wheelMoveAngle += WHEEL_ROTATE_STEP;
		break;

	case 'd':
		wheelMoveAngle -= WHEEL_ROTATE_STEP;
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
		cameraHeight -= CAMERA_MOVE_STEP;
		break;
	case GLUT_KEY_UP:
		cameraHeight += CAMERA_MOVE_STEP;
		break;

	case GLUT_KEY_RIGHT:
		cameraAngle += CAMERA_ROTATE_STEP;
		break;
	case GLUT_KEY_LEFT:
		cameraAngle -= CAMERA_ROTATE_STEP;
		break;

	case GLUT_KEY_PAGE_UP:
		cameraDistance -= CAMERA_MOVE_STEP;
		break;
	case GLUT_KEY_PAGE_DOWN:
		cameraDistance += CAMERA_MOVE_STEP;
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
		if (state == GLUT_DOWN)
		{
			drawaxes = 1 - drawaxes;
		}
		break;

	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
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
	gluLookAt(cameraDistance * cos(toRad(cameraAngle)), cameraDistance * sin(toRad(cameraAngle)), cameraHeight, 0, 0, 0, 0, 0, 1);

	// again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

	/****************************
	/ Add your objects from here
	****************************/
	// add objects
	drawAxes();
	glColor3f(1, 1, 1);
	drawGrid();

	//	Translate the wheel to (0, 0, 0)
	glTranslated(center.x, center.y, center.z);
	//	Translate it so that it stays on XY plane. Not crosses it.
	glTranslated(0, 0, WHEEL_RADIUS);

	//	Movement angle along XY plane
	glRotated(wheelMoveAngle, 0, 0, 1);
	//	Rotate angle along Z axis
	glRotated(wheelRotateAngle, 0, 1, 0);
	drawWheel();

	glutSwapBuffers();
}

void animate()
{
	angle += 0.05;
	// codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init()
{
	// codes for initialization
	drawgrid = 1;
	drawaxes = 1;

	cameraDistance = 200.0;
	cameraHeight = 150.0;
	cameraAngle = 60.0;
	angle = 0;

	//	Initialization
	wheelMoveAngle = 0;
	wheelRotateAngle = 0;

	center.x = 0;
	center.y = 0;
	center.z = 0;

	// clear the screen
	glClearColor(0, 0, 0, 0);

	/************************
	/ set-up projection here
	************************/
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
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Depth, Double buffer, RGB color

	glutCreateWindow("1705066 - Task 2");

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
