#include "GlHandle.h"

using namespace std;

vector<Face> faces;
vector<SubdModelPoint> points;

Subdivision subdivision = Subdivision();

//input handle
bool mouseLeftDown = false;
bool mouseRightDown = false;
double mouseX, mouseY;
double cameraDistanceX;
double cameraDistanceY;
double cameraAngleX;
double cameraAngleY;
double scalefTimes = 1;


void Flat(void)
{
	for (int i = 0; i < faces.size(); i++)
	{
		if (!faces[i].IsDeleted)
		{
			GLfloat showcolor_amb[] = { 0.0f, 0.3f, 0.7f, 1.0f };
			GLfloat showcolor[] = { 0.0f, 0.3f, 0.7f, 1.0f };
			GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glMaterialfv(GL_FRONT, GL_AMBIENT, showcolor_amb);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, showcolor);
			glMaterialfv(GL_FRONT, GL_SPECULAR, white);
			GLfloat shininess[] = { 10 };
			glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

			glBegin(GL_QUADS);
			glNormal3d(faces[i].Normal.X, -faces[i].Normal.Y, faces[i].Normal.Z);
			glVertex3f(points[ faces[i].Point[0].Index ].X, -points[ faces[i].Point[0].Index ].Y, points[ faces[i].Point[0].Index ].Z);
			glVertex3f(points[ faces[i].Point[1].Index ].X, -points[ faces[i].Point[1].Index ].Y, points[ faces[i].Point[1].Index ].Z);
			glVertex3f(points[ faces[i].Point[2].Index ].X, -points[ faces[i].Point[2].Index ].Y, points[ faces[i].Point[2].Index ].Z);
			glVertex3f(points[ faces[i].Point[3].Index ].X, -points[ faces[i].Point[3].Index ].Y, points[ faces[i].Point[3].Index ].Z);
			glEnd();

		}
	}
	glFlush();

}

void display(void)
{
	Flat();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutPostRedisplay();
	glFlush();
}


void init(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-200, 200, 200, -200, -150.0, 150.0);
	glMatrixMode(GL_MODELVIEW);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Create light components.
	GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f};
	GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f };
	GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat position[] = { -100.0f, -100.0f, -100.0f, 1.0f };

	// Assign created components to GL_LIGHT0.
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	//glScalef(1.0f, 1.0f, 1.0f);
	display();

}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h) 
	{
		glOrtho(-1.5, 1.5, 1.5 * (GLfloat)h / (GLfloat)w, -1.5 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	}	
	else 
	{
		glOrtho(-1.5*(GLfloat)w / (GLfloat)h, 1.5*(GLfloat)w / (GLfloat)h, 1.5, -1.5, -10.0, 10.0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouseClickHandle(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseLeftDown = true;
		}
		else if (state == GLUT_UP)
		{
			mouseLeftDown = false;
		}
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseRightDown = true;
		}
		else if (state == GLUT_UP)
		{
			mouseRightDown = false;
		}
	}
}

void mouseMotionHandle(int x, int y)
{
	cameraAngleX = cameraAngleY = 0;
	cameraDistanceX = cameraDistanceY = 0;

	if (mouseLeftDown)
	{
		cameraAngleY += (x - mouseX) * 0.1f;
		cameraAngleX += (y - mouseY) * 0.1f;
		mouseX = x;
		mouseY = y;
	}
	if (mouseRightDown)
	{
		cameraDistanceX = (x - mouseX) * 0.002f;
		cameraDistanceY = (y - mouseY) * 0.002f;
		mouseY = y;
		mouseX = x;
	}
	glTranslatef(cameraDistanceX, cameraDistanceY, 0);
	glRotatef(cameraAngleX, 1, 0, 0);
	glRotatef(cameraAngleY, 0, 1, 0);

}

void keyboardHandle(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ' ': //subdivide
	case 's':
	case 'S':
		clock_t t = clock();

		subdivision.SetVar(faces, points);
		subdivision.CCsubDivide();
		faces = subdivision.GetFaces();
		points = subdivision.GetPts();

		cout << "finished! costing:" << clock() - t<< "ms" << endl;
		break;
	}
}

void specialKeysHandle(int key, int X, int Y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		scalefTimes = 0.01f + 1;
		glScalef(scalefTimes, scalefTimes, scalefTimes);
		break;
	case GLUT_KEY_DOWN:
		scalefTimes = -0.01f + 1;
		glScalef(scalefTimes, scalefTimes, scalefTimes);
		break;
	case GLUT_KEY_RIGHT:
		break;
	case GLUT_KEY_LEFT:
		break;
	}
}


void GLHandle::Run(int argc, char* argv[])
{
	string filename = "cube-quad.obj"; 
	// cube-
	// bunny-
	if (2 == argc && NULL != argv[1] ) {
		filename = string(argv[1]);
		cout << filename << endl;
	}
	cout << filename << endl;
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("GemDemo4GMSub");

	LoadObject(filename, points, faces);
	subdivision.SetVar(faces, points);
	init();

	glutMouseFunc(mouseClickHandle);
	glutMotionFunc(mouseMotionHandle);
	glutKeyboardFunc(keyboardHandle);
	glutSpecialFunc(specialKeysHandle);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
}
