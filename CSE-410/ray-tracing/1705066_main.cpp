#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#include <unistd.h>
#include <GL/glut.h>

#include "1705066_classes.hpp"
#include "bitmap_image.hpp"


#define pi (2 * acos(0.0))
#define toRad(deg) (double)(deg * pi) / 180

#define CAMERA_ROTATE_STEP 10
#define CAMERA_MOVE_STEP 10

#define N_STACKS 100
#define N_SLICES 100

#define INPUT_FILE "./scene.txt"


point pos;
unit_vector l, r, u;

int drawaxes;


vector< Object* > objects;
vector< PointLight > pointLights;
vector< SpotLight > spotLights;

void drawAxes()
{
	if (drawaxes == 1)
	{
		glColor3f(1.0, 0, 0);
		glBegin(GL_LINES);
		{
			glVertex3f(500, 0, 0);
			glVertex3f(-500, 0, 0);
		}
		glEnd();
	
		glColor3f(0, 1.0, 0);
		glBegin(GL_LINES);
		{
			glVertex3f(0, 500, 0);
			glVertex3f(0, -500, 0);
		}
		glEnd();
	
		glColor3f(0, 0, 1.0);
		glBegin(GL_LINES);
		{
			glVertex3f(0, 0, 500);
			glVertex3f(0, 0, -500);
		}
		glEnd();	
	}
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
		break;

	case GLUT_KEY_END:
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

	Floor floor(200, 20);
	floor.draw();

	for (Object* each : objects) {
		each->draw();
	}

	for (PointLight light : pointLights) {
		light.draw();
	}

	for (SpotLight light : spotLights) {
		light.draw();
	}

	// Sphere sp(Vector(100, 100, -200), 20);
	// sp.draw();

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
	drawaxes = 1;

	// clear the screen
	glClearColor(0, 0, 0, 0);

	/************************
	/ set-up projection here
	************************/
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


int nRecursion;
int imageWidth;

void loadData()
{
	ifstream input;
	input.open(INPUT_FILE);	

	if (!input.is_open()) {
		cout << "The file '" << INPUT_FILE << "' doesn't exist!" << endl;
		exit(0);
	}

	input >> nRecursion >> imageWidth;

	Object* object;
	string objectType;

	int nObjects;
	input >> nObjects;

	for (int i=0; i < nObjects; i++)
	{
		input >> objectType;

		if (objectType == "sphere")
		{
			Vector center;
			input >> center;
			cout << "(Center): " << center << endl;

			double radius;
			input >> radius;

			Color color;
			input >> color;
			cout << "(Color): " << color << endl;

			double ambient, diffuse, specular, reflection;
			input >> ambient >> diffuse >> specular >> reflection;

			int shine;
			input >> shine;

			object = new Sphere(center, radius);
			object->setColor(color);
			object->setCoEff(ambient, diffuse, specular, reflection);
			object->setShine(shine);

			objects.push_back(object);
		}
		else if (objectType == "triangle")
		{
			Vector a, b, c;
			input >> a >> b >> c;
			
			Color color;
			input >> color;

			double ambient, diffuse, specular, reflection;
			input >> ambient >> diffuse >> specular >> reflection;

			int shine;
			input >> shine;

			object = new Triangle(a, b, c);
			object->setColor(color);
			object->setCoEff(ambient, diffuse, specular, reflection);
			object->setShine(shine);

			objects.push_back(object);
		}
		else if (objectType == "general")
		{
			double A, B, C, D, E, F, G, H, I, J;
			input >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;

			Vector point;
			input >> point;

			double length, width, height;
			input >> length >> width >> height;

			Color color;
			input >> color;
			
			double ambient, diffuse, specular, reflection;
			input >> ambient >> diffuse >> specular >> reflection;

			int shine;
			input >> shine;

			object = new General(A, B, C, D, E, F, G, H, I, J);
			object->setColor(color);
			object->setCoEff(ambient, diffuse, specular, reflection);
			object->setShine(shine);

			objects.push_back(object);
		}
		else
		{
			cout << "Object Type '" << objectType << "' is invalid!" << endl;
			break;
		}
	}

	int nPointLights;
	input >> nPointLights;

	for (int i=0; i < nPointLights; i++)
	{
		Vector position;
		input >> position;

		Color color;
		input >> color;

		PointLight light(position, color);
		pointLights.push_back(light);
	}

	int nSpotLights;
	input >> nSpotLights;

	for (int i=0; i < nSpotLights; i++)
	{
		Vector position;
		input >> position;

		cout << "SL position: " << position << endl;

		Color color;
		input >> color;

		Vector direction;
		input >> direction;

		double cutoff_angle;
		input >> cutoff_angle;

		SpotLight light(position, color, direction, cutoff_angle);
		spotLights.push_back(light);
	}

	cout << ">>> LOAD DATA COMPLETED! <<<" << endl;
}

int main(int argc, char **argv)
{
	loadData();
	cout << "# of objects: " << objects.size() << endl;
	cout << "# of pointLights: " << pointLights.size() << endl;
	cout << "# of spotLights: " << spotLights.size() << endl;

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

