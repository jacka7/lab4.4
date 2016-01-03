#pragma comment(lib,"glaux.lib")
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <gl/GLAux.h>
#include <GL/glut.h>
using namespace std;

float WinWid = 600.0;
float WinHei = 600.0;
int countOfStrips = 20;

GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };    // Ambient light
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };    // Diffuse light
GLfloat LightPosition[] = { 50.0f, 0.0f, 20.0f, 1.0f }; // Light position

GLuint texture[4]; // Array to store textures

int  angle1 = 0, angle2 = 0;// Angle of rotation

double radius = 5; // Radius of the cylinder

struct ver
{
	float x, y, z;
};

struct sq
{
	ver fir, sec, thi, fou;
};


vector<sq> flag;
vector<sq> cyl;

void LoadGLTextures(int num, LPCSTR s)
{
	AUX_RGBImageRec *texture1;
	texture1 = auxDIBImageLoad(s);

	glGenTextures(1, &texture[num]);
	glBindTexture(GL_TEXTURE_2D, texture[num]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, texture1->sizeX, texture1->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, texture1->data);
}

void Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glRotatef(angle1, 0, 1, 0);
	glRotatef(angle2, 1, 0, 0);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);

	// FLAG
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	float coef = 0;
	for (int i = 0; i < flag.size(); i++)
	{
		glTexCoord2f(0.0f + coef, 0.0f);
		glVertex3f(flag[i].fir.x, flag[i].fir.y, flag[i].fir.z);
		glTexCoord2f(1.0f / flag.size() + coef, 0.0f);
		glVertex3f(flag[i].sec.x, flag[i].sec.y, flag[i].sec.z);
		glTexCoord2f(1.0f / flag.size() + coef, 1.0f);
		glVertex3f(flag[i].fou.x, flag[i].fou.y, flag[i].fou.z);
		glTexCoord2f(0.0f + coef, 1.0f);
		glVertex3f(flag[i].thi.x, flag[i].thi.y, flag[i].thi.z);

		coef += 1.0f / flag.size();
	}
	glEnd();

	// FIELD
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(300, -200, -300);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(300, -200, 300);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-300, -200, 300);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-300, -200, -300);
	glEnd();

	// CYLINDER
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin(GL_QUADS);
	coef = 0;
	for (int i = 0; i < cyl.size(); i++)
	{
		glTexCoord2f(0.0f + coef, 0.0f);	  glVertex3f(cyl[i].fir.x, cyl[i].fir.y, cyl[i].fir.z);
		glTexCoord2f(1.0f / 36.0 + coef, 0.0f); glVertex3f(cyl[i].sec.x, cyl[i].sec.y, cyl[i].sec.z);
		glTexCoord2f(1.0f / 36.0 + coef, 1.0f); glVertex3f(cyl[i].fou.x, cyl[i].fou.y, cyl[i].fou.z);
		glTexCoord2f(0.0f + coef, 1.0f);      glVertex3f(cyl[i].thi.x, cyl[i].thi.y, cyl[i].thi.z);
		coef += 1.0 / 36.0;
	}
	glEnd();

	// top of cylinder
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_TRIANGLES);
	coef = 0;
	for (int i = 0; i < cyl.size(); i++)
	{
		glTexCoord2f(0.5f, 0.5f);											      glVertex3f(0, 170, 0);
		glTexCoord2f(0.5f + cyl[i].fou.x / radius / 2, 0.5f + cyl[i].fou.z / radius / 2); glVertex3f(cyl[i].fou.x, cyl[i].fou.y, cyl[i].fou.z);
		glTexCoord2f(0.5f + cyl[i].thi.x / radius / 2, 0.5f + cyl[i].thi.z / radius / 2); glVertex3f(cyl[i].thi.x, cyl[i].thi.y, cyl[i].thi.z);
	}
	glEnd();

	glPopMatrix();

	glutSwapBuffers();
}

void SKeyboard(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		angle1 += 2;
		break;
	case GLUT_KEY_RIGHT:
		angle1 -= 2;
		break;
	case GLUT_KEY_DOWN:
		angle2 -= 2;
		break;
	case GLUT_KEY_UP:
		angle2 += 2;
		break;
	}
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'd':
		LightPosition[0] += 5;
		break;
	case 'a':
		LightPosition[0] -= 5;
		break;
	case 'w':
		LightPosition[1] += 5;
		break;
	case 's':
		LightPosition[1] -= 5;
		break;
	case 'x':
		LightPosition[2] += 5;
		break;
	case 'z':
		LightPosition[2] -= 5;
		break;
	}
}

const bool STRAIGHT = true;
const bool REVERSE = false;

void Timer(int value)
{
	static int k = 0;
	static bool direction = STRAIGHT;
	static int b = 5;

	if (direction == STRAIGHT && k < b)
		k += 4;
	else if (direction == REVERSE && k > -10)
		k -= 4;
	else
	{
		direction = !direction;
		k = 0;
		b = 10;
	}

	for (int i = 0; i < flag.size(); i++)
	{
		flag[i].fir.z += sin((flag[i].fir.x) / flag.size()) * k;

		flag[i].thi.z += sin((flag[i].thi.x) / flag.size()) * k;

		flag[i].sec.z += sin((flag[i].sec.x) / flag.size()) * k;

		flag[i].fou.z += sin((flag[i].fou.x) / flag.size()) * k;
	}

	glutPostRedisplay();
	glutTimerFunc(100.0, Timer, 0);
}

void Initialize()
{
	// Texture loading
	LPCSTR s = "flag.bmp";
	LoadGLTextures(0, s);

	s = "floorGrass.bmp";
	LoadGLTextures(1, s);

	s = "tree.bmp";
	LoadGLTextures(2, s);

	s = "treeUp.bmp";
	LoadGLTextures(3, s);

	glEnable(GL_TEXTURE_2D);


	// Initialization
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);

	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-WinWid / 2, WinWid / 2, -WinHei / 2, WinHei / 2, -500.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
	glRotatef(20, 1, 0, 0);
}

int main(int argc, char** argv)
{

	// FLAG'S QUADS

	int xSh = 0; // Shift of each quad

	for (int i = 0; i < countOfStrips; i++, xSh += float(10 * 20 / countOfStrips))
	{
		sq quad;

		quad.fir.x = xSh;
		quad.fir.y = 0;
		quad.fir.z = 0;

		quad.sec.x = xSh + float(10*20/countOfStrips);
		quad.sec.y = 0;
		quad.sec.z = 0;

		quad.thi.x = xSh;
		quad.thi.y = 180;
		quad.thi.z = 0;

		quad.fou.x = xSh + float(10 * 20 / countOfStrips);
		quad.fou.y = 180;
		quad.fou.z = 0;

		flag.push_back(quad);
	}


	// CYLINDER'S QUAD
	for (float i = 0.0; i < 2 * 3.14; i += 3.14 / 18)
	{
		sq temp;

		temp.fir.x = radius*sin(i);
		temp.fir.y = -200;
		temp.fir.z = radius*cos(i);

		temp.sec.x = radius*sin(i + 3.14 / 18);
		temp.sec.y = -200;
		temp.sec.z = radius*cos(i + 3.14 / 18);

		temp.thi.x = radius*sin(i);
		temp.thi.y = 200;
		temp.thi.z = radius*cos(i);

		temp.fou.x = radius*sin(i + 3.14 / 18);
		temp.fou.y = 200;
		temp.fou.z = radius*cos(i + 3.14 / 18);

		cyl.push_back(temp);
	}

	// Initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WinWid, WinHei);
	glutInitWindowPosition(300, 50);
	glutCreateWindow("lab4");

	// Registration
	glutDisplayFunc(Draw);
	glutTimerFunc(50.0, Timer, 0);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SKeyboard);
	Initialize();
	glutMainLoop();
	return 0;
}