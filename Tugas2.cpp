/*
 * OGL01Shape3D.cpp: 3D Shapes
 */
#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <math.h>
 
/* Global variables */
char title[] = "3D Shapes";
float x=0.0f,y=0.0f, z=5.0f;
float deltaMoveX = 0, deltaMoveY = 0, deltaMoveZ = 0;
float lx=0.1f, ly=0.1f, lz=-5.0f;
float angle = 0.0f;
float deltaAngle=0.0f;
int xOrigin = -1;

/* Initialize OpenGL Graphics */
void initGL() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
   glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glShadeModel(GL_SMOOTH);   // Enable smooth shading
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}
 
void computePos(){
	x += deltaMoveX * lx * 0.1f;
    y += deltaMoveY * ly * 0.1f;
	z += deltaMoveZ * lz * 0.1f;
}
/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {

    computePos();

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
   glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
 
   // Render a color-cube consisting of 6 quads with different colors
   glLoadIdentity();                 // Reset the model-view matrix
   glTranslatef(0.0f, -0.5f, -7.0f);  // Move right and into the screen
 
    gluLookAt(x,y,z,
            x+lx,y+ly,z+lz,
            0.0f,1.0f,0.0f);

   glBegin(GL_QUAD_STRIP);
   	  //FACE
   	  
   	  // Red
      glColor3f(1.0f, 0.0f, 0.0f);
      glVertex3f(-1.3f, -0.5f, 1.0f);
      glVertex3f(-1.3f,  0.3f, 1.0f);
      glVertex3f(-0.7f, -0.5f, 1.0f);
      glVertex3f(-0.7f,  0.3f, 1.0f);
      glVertex3f(-0.7f, -0.5f, 1.0f);
      glVertex3f(-0.7f,  1.0f, 1.0f);
      glVertex3f( 0.7f, -0.5f, 1.0f);
      glVertex3f( 0.7f,  1.0f, 1.0f);
      glVertex3f( 0.7f, -0.5f, 1.0f);
      glVertex3f( 0.7f,  0.3f, 1.0f);
      glVertex3f( 1.3f, -0.5f, 1.0f);
      glVertex3f( 1.3f,  0.3f, 1.0f);
      
      //BACKFACE
      glColor3f(1.0f, 0.0f, 0.0f);
      glVertex3f(-1.3f, -0.5f, -1.0f);
      glVertex3f(-1.3f,  0.3f, -1.0f);
      glVertex3f(-0.7f, -0.5f, -1.0f);
      glVertex3f(-0.7f,  0.3f, -1.0f);
      glVertex3f(-0.7f, -0.5f, -1.0f);
      glVertex3f(-0.7f,  1.0f, -1.0f);
      glVertex3f( 0.7f, -0.5f, -1.0f);
      glVertex3f( 0.7f,  1.0f, -1.0f);
      glVertex3f( 0.7f, -0.5f, -1.0f);
      glVertex3f( 0.7f,  0.3f, -1.0f);
      glVertex3f( 1.3f, -0.5f, -1.0f);
      glVertex3f( 1.3f,  0.3f, -1.0f);
   glEnd();
   
   glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
      // Kap atas
      // Define vertices in counter-clockwise (CCW) order with normal pointing out
      glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      glVertex3f( 0.7f, 1.0f, -1.0f);
      glVertex3f(-0.7f, 1.0f, -1.0f);
      glVertex3f(-0.7f, 1.0f,  1.0f);
      glVertex3f( 0.7f, 1.0f,  1.0f);
 
      // Body Bawah
      glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      glVertex3f( 1.3f, -0.5f,  1.0f);
      glVertex3f(-1.3f, -0.5f,  1.0f);
      glVertex3f(-1.3f, -0.5f, -1.0f);
      glVertex3f( 1.3f, -0.5f, -1.0f);
      
      // Kap Depan
      glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      glVertex3f( 1.3f, 0.3f, -1.0f);
      glVertex3f( 0.7f, 0.3f, -1.0f);
      glVertex3f( 0.7f, 0.3f,  1.0f);
      glVertex3f( 1.3f, 0.3f,  1.0f);
      
      // Kap Belakang
      glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      glVertex3f( -1.3f, 0.3f, -1.0f);
      glVertex3f( -0.7f, 0.3f, -1.0f);
      glVertex3f( -0.7f, 0.3f,  1.0f);
      glVertex3f( -1.3f, 0.3f,  1.0f);
      
      // Kaca Depan
      glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
      glVertex3f(0.7f,  1.0f, -1.0f);
      glVertex3f(0.7f,  1.0f,  1.0f);
      glVertex3f(0.7f,  0.3f,  1.0f);
      glVertex3f(0.7f,  0.3f, -1.0f);
      
      // Kaca Depan
      glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
      glVertex3f(-0.7f,  1.0f, -1.0f);
      glVertex3f(-0.7f,  1.0f,  1.0f);
      glVertex3f(-0.7f,  0.3f,  1.0f);
      glVertex3f(-0.7f,  0.3f, -1.0f);
          
 
      // Bemper Belakang
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(-1.3f,  0.3f,  1.0f);
      glVertex3f(-1.3f,  0.3f, -1.0f);
      glVertex3f(-1.3f, -0.5f, -1.0f);
      glVertex3f(-1.3f, -0.5f,  1.0f);
 
      // Bemper Depan
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(1.3f,  0.3f, -1.0f);
      glVertex3f(1.3f,  0.3f,  1.0f);
      glVertex3f(1.3f, -0.5f,  1.0f);
      glVertex3f(1.3f, -0.5f, -1.0f);
   glEnd();  // End of drawing color-cube
   
 
   glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}
 
/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear and zFar
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);
   glMatrixMode(GL_MODELVIEW);
}

void pressKey(int key, int xx, int yy) {

	switch (key) {
		case GLUT_KEY_LEFT : deltaMoveX = -0.5f; break;
		case GLUT_KEY_RIGHT : deltaMoveX = 0.5f; break;
		case GLUT_KEY_UP : deltaMoveY = 0.5f; break;
		case GLUT_KEY_DOWN : deltaMoveY = -0.5f; break;
        case GLUT_KEY_PAGE_UP : deltaMoveZ = 0.2f; break;
        case GLUT_KEY_PAGE_DOWN : deltaMoveZ = -0.2f; break;
	}
}

void releaseKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT :
		case GLUT_KEY_RIGHT : 
		case GLUT_KEY_UP :
		case GLUT_KEY_DOWN : 
        case GLUT_KEY_PAGE_UP : 
        case GLUT_KEY_PAGE_DOWN : deltaMoveX = 0;deltaMoveY = 0;deltaMoveZ = 0;break;
	}
}

void mouseMove(int x, int y) { 	

    // this will only be true when the left button is down
    if (xOrigin >= 0) {

		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.001f;
		// update camera's direction
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	}
}

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			angle += deltaAngle;
			xOrigin = -1;
		}
		else  {// state = GLUT_DOWN
			xOrigin = x;
		}
	}
}
 
/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
   glutInitWindowSize(640, 480);   // Set the window's initial width & height
   glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
   glutCreateWindow(title);          // Create window with the given title
   
   glutDisplayFunc(display);       // Register callback handler for window re-paint event
   glutReshapeFunc(reshape);       // Register callback handler for window re-size event
   glutIdleFunc(display);
   glutSpecialFunc(pressKey);
   glutIgnoreKeyRepeat(1);
   glutSpecialUpFunc(releaseKey);
   glutMouseFunc(mouseButton);
   glutMotionFunc(mouseMove);

   initGL();                       // Our own OpenGL initialization
   glutMainLoop();                 // Enter the infinite event-processing loop
   return 0;
}
