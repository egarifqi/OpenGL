#include <GL/glut.h>
#include <math.h>
#include <windows.h>
#include <unistd.h>


float angle1 = 0.0f;

char title[] = "3D Shapes";

void initGL(){
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

float angle2 = 0.0f;
void drawCircle(GLfloat x, GLfloat y, GLfloat radius)
{
	int i;
	int PI = 3.14159;
	int triangleAmount = 100;
	GLfloat twicePi = 2.0f * PI;
	
	//glRotatef(angle2, 1.0f, 0.0f, 0.0f);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(10.0);
	
	glBegin(GL_LINES);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	for(i = 0; i <= triangleAmount; i++)
	{
		glVertex2f( x, y);
		glVertex2f(x + (radius * cos(i * twicePi / triangleAmount)), y + (radius * sin(i * twicePi / triangleAmount)));
	}
	glEnd();
	//angle2+=0.1f;
}

void drawBody(){
	glColor3f(0,1,0.8);
	glBegin(GL_POLYGON);
	glVertex2f(150,350);
	glVertex2f(150,300);
	glVertex2f(490,300);
	glVertex2f(490,350);
	glVertex2f(440,350);
	glVertex2f(440,380);
	glVertex2f(200,380);
	glVertex2f(200,350);
	glEnd();
}

//float angle = 0.0f;
int x1 = 250;
int x2 = 390;


void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentitty();
	glTranslatef(1.5f, 0.0f, -7.0f);
	//glLoadIdentity();
	
	drawBody();
	//glRotatef(angle, 1.0f, 0.0f, 0.0f);
	drawCircle(250,300,30);
	drawCircle(390,300,30);
	/*
	
	glColor3f(0,1,0.8);
	glBegin(GL_POLYGON);
	glVertex2f(0.1,0.2);
	glVertex2f(0.1,0.1);
	glVertex2f(0.5,0.1);
	glVertex2f(0.5,0.2);
	glVertex2f(0.5,0.2);
	glVertex2f(0.4,0.3);
	glVertex2f(0.2,0.3);
	glVertex2f(0.2,0.2);
	glVertex2f(0.1,0.2);
	glEnd();*/
	//angle+=0.1f;
	glutSwapBuffers();
}

void rotate(){

	//glRotatef(angle, 1.0f, 0.0f, 0.0f);
	drawCircle2(250,300,30);
	drawCircle2(390,300,30);
	drawCircle1(250,300,30);
	drawCircle1(390,300,30);

	glutSwapBuffers();
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1280,1280);
	glutCreateWindow(title);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMainLoop();
	return 0;
}
