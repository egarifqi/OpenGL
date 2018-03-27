#include <GL/glut.h>
#include <math.h>



void drawCircle1(GLfloat x, GLfloat y, GLfloat radius)
{
	int i;
	int PI = 3.14159;
	int triangleAmount = 10;
	GLfloat twicePi = 2.0f * PI;
	
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.0);
	
	glBegin(GL_LINES);
	glColor4f(0.0, 0.0, 0.0, 1.0);
	for(i = 0; i <= triangleAmount; i++)
	{
		glVertex2f( x, y);
		glVertex2f(x + (radius * cos(i * twicePi / triangleAmount)), y + (radius * sin(i * twicePi / triangleAmount)));
	}
	glEnd();
}

void drawCircle2(GLfloat x, GLfloat y, GLfloat radius)
{
	int i;
	int PI = 3.14159;
	int triangleAmount = 100;
	GLfloat twicePi = 2.0f * PI;
	
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

void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	drawBody();
	drawCircle2(250,300,30);
	drawCircle2(390,300,30);
	drawCircle1(250,300,30);
	drawCircle1(390,300,30);
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
	
	glFlush();
	glutSwapBuffers();
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(640,640);
	glutCreateWindow("Open GL Test");
	glutDisplayFunc(display);
	gluOrtho2D(0,640,0,640);
	glClearColor(0.5,0.7,0.5,0);
	glutMainLoop();
	return 0;
}
