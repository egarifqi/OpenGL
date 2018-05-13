/*
 * OGL01Shape3D.cpp: 3D Shapes
 */
#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <math.h>
#include "RgbImage.h"
#include "RgbImage.cpp"
 
/* Global variables */
char title[] = "3D Shapes";
float x=0.0f,y=0.0f, z=5.0f;
float deltaMoveX = 0, deltaMoveY = 0, deltaMoveZ = 0;
float lx=0.1f, ly=0.1f, lz=-5.0f;
float angle = 0.0f;
float deltaAngle=0.0f;
int xOrigin = -1;
char filename[] = "./salt_on_spoon.bmp";
GLuint   texture[1];

//LIGHTING FUNCTION
void AmbientLighting();
void PointLight(const float x, const float y, const float z, const float amb, const float diff, const float spec);
void spotlight();

void AmbientLighting(){
	glEnable(GL_LIGHTING);
	
	double amb = .2;
	GLfloat global_ambient[] = {amb,amb,amb,1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	
}

void PointLight(const float x, const float y, const float z, const float amb, const float diff, const float spec){
	glEnable(GL_LIGHTING);
	
	GLfloat light_ambient[] = { amb,amb,amb, 1.0 };
	GLfloat light_diffuse[] = {diff, diff, diff, 1.0 };
	GLfloat light_specular[] = {spec, spec, spec, 1.0 };
	
	GLfloat light_position[] = {x,y,z, 0.0 };
	 
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	glEnable(GL_LIGHT0); //enable the light after setting the properties
}

void spotlight(){
	GLfloat light_ambient[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat _light_position[4];
	GLfloat _spotlight_position[4];
	_light_position[0] =  0.0;
	_light_position[1] = 0.0;
	_light_position[2] = 0.0;
	_light_position[3] = 1.0;
	
	_spotlight_position[0] = 0.0;
	_spotlight_position[1] = -1.0;
	_spotlight_position[2] = 0.0;
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	
	glEnable(GL_LIGHT7);
    glLightfv(GL_LIGHT7, GL_POSITION, _light_position);
}

/* Initialize OpenGL Graphics */
void initGL() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
   glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glShadeModel(GL_SMOOTH);   // Enable smooth shading
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
   
   glEnable(GL_NORMALIZE);
}

void loadTextureFromFile(char *filename)
{   
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);

    RgbImage theTexMap( filename );

    // Pixel alignment: each row is word aligned (aligned to a 4 byte boundary)
    //    Therefore, no need to call glPixelStore( GL_UNPACK_ALIGNMENT, ... );

    glGenTextures(1, &texture[0]);               // Create The Texture
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Typical Texture Generation Using Data From The Bitmap
    glTexImage2D(GL_TEXTURE_2D, 0, 3, theTexMap.GetNumCols(), theTexMap.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE, theTexMap.ImageData() );
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
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, texture[0]);
   glLoadIdentity();                 // Reset the model-view matrix
   glTranslatef(0.0f, -0.5f, -7.0f);  // Move right and into the screen
 
    gluLookAt(x,y,z,
            x+lx,y+ly,z+lz,
            0.0f,1.0f,0.0f);
   glBegin(GL_QUADS);
   		//tembok kiri
   		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-8.0f, -0.7f, 1.2f);
		glVertex3f(-3.0f, -0.7f, 1.2f);
		glVertex3f(-3.0f,  3.0f, 1.2f);
		glVertex3f(-8.0f,  3.0f, 1.2f);
		
		//tembok kanan
		glVertex3f(-8.0f, -0.7f, -7.2f);
		glVertex3f(-3.0f, -0.7f, -7.2f);
		glVertex3f(-3.0f,  3.0f, -7.2f);
		glVertex3f(-8.0f,  3.0f, -7.2f);
		
		
		//lantai
		glVertex3f(-8.0f, -0.7f,  1.2f);
		glVertex3f(-3.0f, -0.7f,  1.2f);
		glVertex3f(-3.0f, -0.7f, -7.2f);
		glVertex3f(-8.0f, -0.7f, -7.2f);
		
		//dasar
		/*glVertex3f(-8.0f, -0.72f,  7.5f);
		glVertex3f(8.0f, -0.72f,  7.5f);
		glVertex3f(8.0f, -0.72f, -9.5f);
		glVertex3f(-8.0f, -0.72f, -9.5f);*/
		
		//atap kiri
		glVertex3f(-8.0f, 6.0f, -3.2f);
		glVertex3f(-3.0f, 6.0f, -3.2f);
		glVertex3f(-3.0f, 2.1f,  2.4f);
		glVertex3f(-8.0f, 2.1f,  2.4f);
		
		//atap kanan
		glVertex3f(-8.0f, 6.0f, -3.2f);
		glVertex3f(-3.0f, 6.0f, -3.2f);
		glVertex3f(-3.0f, 2.1f, -8.4f);
		glVertex3f(-8.0f, 2.1f, -8.4f);
		
		//cerobong asap
		glVertex3f(-6.0f, 3.9f, -0.8f);
		glVertex3f(-5.0f, 3.9f, -0.8f);
		glVertex3f(-5.0f, 6.4f, -0.8f);
		glVertex3f(-6.0f, 6.4f, -0.8f);
		
		glVertex3f(-6.0f, 3.9f, -1.8f);
		glVertex3f(-5.0f, 3.9f, -1.8f);
		glVertex3f(-5.0f, 6.4f, -1.8f);
		glVertex3f(-6.0f, 6.4f, -1.8f);
		
		glVertex3f(-6.0f, 3.9f, -1.8f);
		glVertex3f(-6.0f, 6.4f, -1.8f);
		glVertex3f(-6.0f, 6.4f, -0.8f);
		glVertex3f(-6.0f, 3.9f, -0.8f);
		
		glVertex3f(-5.0f, 3.9f, -1.8f);
		glVertex3f(-5.0f, 6.4f, -1.8f);
		glVertex3f(-5.0f, 6.4f, -0.8f);
		glVertex3f(-5.0f, 3.9f, -0.8f);
		
		//pohon
		glVertex3f(6.0f, -0.7f,  0.3f);
		glVertex3f(6.5f, -0.7f,  0.3f);
		glVertex3f(6.5f, 2.2f, 0.3f);
		glVertex3f(6.0f, 2.2f, 0.3f);
		
		glVertex3f(6.0f, -0.7f,  -0.2f);
		glVertex3f(6.5f, -0.7f,  -0.2f);
		glVertex3f(6.5f, 2.2f, -0.2f);
		glVertex3f(6.0f, 2.2f, -0.2f);
		
		glVertex3f(6.0f, -0.7f,  0.3f);
		glVertex3f(6.0f, -0.7f,  -0.2f);
		glVertex3f(6.0f, 2.2f, -0.2f);
		glVertex3f(6.0f, 2.2f, 0.3f);
		
		glVertex3f(6.5f, -0.7f,  0.3f);
		glVertex3f(6.5f, -0.7f,  -0.2f);
		glVertex3f(6.5f, 2.2f, -0.2f);
		glVertex3f(6.5f, 2.2f, 0.3f);
		
		glVertex3f(5.0f, 2.2f,  1.3f);
		glVertex3f(7.5f, 2.2f,  1.3f);
		glVertex3f(7.5f, 4.7f, 1.3f);
		glVertex3f(5.0f, 4.7f, 1.3f);
		
		glVertex3f(5.0f, 2.2f,  -1.2f);
		glVertex3f(7.5f, 2.2f,  -1.2f);
		glVertex3f(7.5f, 4.7f, -1.2f);
		glVertex3f(5.0f, 4.7f, -1.2f);
		
		glVertex3f(5.0f, 2.2f,  1.3f);
		glVertex3f(5.0f, 2.2f, -1.2f);
		glVertex3f(5.0f, 4.7f, -1.2f);
		glVertex3f(5.0f, 4.7f, 1.3f);
		
		glVertex3f(7.5f, 2.2f,  1.3f);
		glVertex3f(7.5f, 2.2f,  -1.2f);
		glVertex3f(7.5f, 4.7f, -1.2f);
		glVertex3f(7.5f, 4.7f, 1.3f);
		
		
		
		
   glEnd();
   
   glBegin(GL_POLYGON);  
   		//tembok belakang
   		glVertex3f(-8.0f, -0.7f,  1.2f);
   		glVertex3f(-8.0f, -0.7f, -7.2f);
   		glVertex3f(-8.0f,  3.0f, -7.2f);
   		glVertex3f(-8.0f,  6.0f, -3.2f);
   		glVertex3f(-8.0f,  3.0f,  1.2f);
   		
   		//tembok depan
   		glVertex3f(-3.0f, -0.7f,  1.2f);
   		glVertex3f(-3.0f, -0.7f, -7.2f);
   		glVertex3f(-3.0f,  3.0f, -7.2f);
   		glVertex3f(-3.0f,  6.0f, -3.2f);
   		glVertex3f(-3.0f,  3.0f,  1.2f);
   		
   glEnd();
   
   glBegin(GL_QUAD_STRIP);
   	  //FACE
   	  
   	  // Red
      //glColor3f(1.0f, 0.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.3f, -0.5f, 1.0f);
      glTexCoord2f(0.0f, .4f); glVertex3f(-1.3f,  0.3f, 1.0f);
      glTexCoord2f(.3f, 0.0f); glVertex3f(-0.7f, -0.5f, 1.0f);
      glTexCoord2f(.3f, .4f); glVertex3f(-0.7f,  0.3f, 1.0f);
      glTexCoord2f(.3f, 0.0f); glVertex3f(-0.7f, -0.5f, 1.0f);
      glTexCoord2f(.3f, 1.0f); glVertex3f(-0.7f,  1.0f, 1.0f);
      glTexCoord2f(.7f, 0.0f); glVertex3f( 0.7f, -0.5f, 1.0f);
      glTexCoord2f(.7f, 1.0f); glVertex3f( 0.7f,  1.0f, 1.0f);
      glTexCoord2f(.7f, 0.0f); glVertex3f( 0.7f, -0.5f, 1.0f);
      glTexCoord2f(.7f, .4f); glVertex3f( 0.7f,  0.3f, 1.0f);
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.3f, -0.5f, 1.0f);
      glTexCoord2f(1.0f, .4f); glVertex3f( 1.3f,  0.3f, 1.0f);
      
      //BACKFACE
      //glColor3f(1.0f, 0.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.3f, -0.5f, -1.0f);
      glTexCoord2f(0.0f, 0.4f); glVertex3f(-1.3f,  0.3f, -1.0f);
      glTexCoord2f(0.3f, 0.0f); glVertex3f(-0.7f, -0.5f, -1.0f);
      glTexCoord2f(0.3f, 0.4f); glVertex3f(-0.7f,  0.3f, -1.0f);
      glTexCoord2f(0.3f, 0.0f); glVertex3f(-0.7f, -0.5f, -1.0f);
      glTexCoord2f(0.3f, 1.0f); glVertex3f(-0.7f,  1.0f, -1.0f);
      glTexCoord2f(0.7f, 0.0f); glVertex3f( 0.7f, -0.5f, -1.0f);
      glTexCoord2f(0.7f, 1.0f); glVertex3f( 0.7f,  1.0f, -1.0f);
      glTexCoord2f(0.7f, 0.0f); glVertex3f( 0.7f, -0.5f, -1.0f);
      glTexCoord2f(0.7f, 0.4f); glVertex3f( 0.7f,  0.3f, -1.0f);
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.3f, -0.5f, -1.0f);
      glTexCoord2f(1.0f, 0.4f); glVertex3f( 1.3f,  0.3f, -1.0f);
   glEnd();
   
   glBegin(GL_QUADS);// Begin drawing the color cube with 6 quads
      // Kap atas
      // Define vertices in counter-clockwise (CCW) order with normal pointing out
      //glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.7f, 1.0f, -1.0f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.7f, 1.0f, -1.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.7f, 1.0f,  1.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.7f, 1.0f,  1.0f);
 
      // Body Bawah
      //glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      glVertex3f( 1.3f, -0.5f,  1.0f);
      glVertex3f(-1.3f, -0.5f,  1.0f);
      glVertex3f(-1.3f, -0.5f, -1.0f);
      glVertex3f( 1.3f, -0.5f, -1.0f);
      
      // Kap Depan
      //glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.3f, 0.3f, -1.0f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.7f, 0.3f, -1.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.7f, 0.3f,  1.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.3f, 0.3f,  1.0f);
      
      // Kap Belakang
      //glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      glTexCoord2f(1.0f, 0.0f); glVertex3f( -1.3f, 0.3f, -1.0f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f( -0.7f, 0.3f, -1.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f( -0.7f, 0.3f,  1.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f( -1.3f, 0.3f,  1.0f);
      
      // Kaca Depan
      //glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
      glVertex3f(0.7f,  1.0f, -1.0f);
      glVertex3f(0.7f,  1.0f,  1.0f);
      glVertex3f(0.7f,  0.3f,  1.0f);
      glVertex3f(0.7f,  0.3f, -1.0f);
      
      // Kaca Depan
      //glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
      glVertex3f(-0.7f,  1.0f, -1.0f);
      glVertex3f(-0.7f,  1.0f,  1.0f);
      glVertex3f(-0.7f,  0.3f,  1.0f);
      glVertex3f(-0.7f,  0.3f, -1.0f);
          
 
      // Bemper Belakang
      //glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(-1.3f,  0.3f,  1.0f);
      glVertex3f(-1.3f,  0.3f, -1.0f);
      glVertex3f(-1.3f, -0.5f, -1.0f);
      glVertex3f(-1.3f, -0.5f,  1.0f);
 
      // Bemper Depan
      //glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(1.3f,  0.3f, -1.0f);
      glVertex3f(1.3f,  0.3f,  1.0f);
      glVertex3f(1.3f, -0.5f,  1.0f);
      glVertex3f(1.3f, -0.5f, -1.0f);
      
      //Ban Kanan Depan
      //glColor3f(0.3f, 0.1f, 0.0f);
      glVertex3f(0.6f, -0.3f, 1.05f);
      glVertex3f(1.0f, -0.3f, 1.05f);
      glVertex3f(1.0f, -0.7f, 1.05f);
      glVertex3f(0.6f, -0.7f, 1.05f);
      
      glVertex3f(0.6f, -0.3f, 0.8f);
      glVertex3f(1.0f, -0.3f, 0.8f);
      glVertex3f(1.0f, -0.7f, 0.8f);
      glVertex3f(0.6f, -0.7f, 0.8f);
      
      glVertex3f(0.6f, -0.7f, 0.8f);
      glVertex3f(1.0f, -0.7f, 1.05f);
      glVertex3f(0.6f, -0.7f, 1.05f);
      glVertex3f(1.0f, -0.7f, 0.8f);
      
      glVertex3f(1.0f, -0.3f, 0.8f);
      glVertex3f(1.0f, -0.7f, 1.05f);
      glVertex3f(1.0f, -0.3f, 1.05f);
      glVertex3f(1.0f, -0.7f, 0.8f);
      
      glVertex3f(0.6f, -0.3f, 0.8f);
      glVertex3f(0.6f, -0.7f, 1.05f);
      glVertex3f(0.6f, -0.3f, 1.05f);
      glVertex3f(0.6f, -0.7f, 0.8f);
      
      //Ban Kiri Depan
      //glColor3f(0.3f, 0.1f, 0.0f);
      glVertex3f(0.6f, -0.3f, -1.05f);
      glVertex3f(1.0f, -0.3f, -1.05f);
      glVertex3f(1.0f, -0.7f, -1.05f);
      glVertex3f(0.6f, -0.7f, -1.05f);
      
      glVertex3f(0.6f, -0.3f, -0.8f);
      glVertex3f(1.0f, -0.3f, -0.8f);
      glVertex3f(1.0f, -0.7f, -0.8f);
      glVertex3f(0.6f, -0.7f, -0.8f);
      
      glVertex3f(0.6f, -0.7f, -0.8f);
      glVertex3f(1.0f, -0.7f, -1.05f);
      glVertex3f(0.6f, -0.7f, -1.05f);
      glVertex3f(1.0f, -0.7f, -0.8f);
      
      glVertex3f(1.0f, -0.3f, -0.8f);
      glVertex3f(1.0f, -0.7f, -1.05f);
      glVertex3f(1.0f, -0.3f, -1.05f);
      glVertex3f(1.0f, -0.7f, -0.8f);
      
      glVertex3f(0.6f, -0.3f, -0.8f);
      glVertex3f(0.6f, -0.7f, -1.05f);
      glVertex3f(0.6f, -0.3f, -1.05f);
      glVertex3f(0.6f, -0.7f, -0.8f);
      
      //Ban Kanan Blkg
      //glColor3f(0.3f, 0.1f, 0.0f);
      glVertex3f(-0.6f, -0.3f, 1.05f);
      glVertex3f(-1.0f, -0.3f, 1.05f);
      glVertex3f(-1.0f, -0.7f, 1.05f);
      glVertex3f(-0.6f, -0.7f, 1.05f);
      
      glVertex3f(-0.6f, -0.3f, 0.8f);
      glVertex3f(-1.0f, -0.3f, 0.8f);
      glVertex3f(-1.0f, -0.7f, 0.8f);
      glVertex3f(-0.6f, -0.7f, 0.8f);
      
      glVertex3f(-0.6f, -0.7f, 0.8f);
      glVertex3f(-1.0f, -0.7f, 1.05f);
      glVertex3f(-0.6f, -0.7f, 1.05f);
      glVertex3f(-1.0f, -0.7f, 0.8f);
      
      glVertex3f(-1.0f, -0.3f, 0.8f);
      glVertex3f(-1.0f, -0.7f, 1.05f);
      glVertex3f(-1.0f, -0.3f, 1.05f);
      glVertex3f(-1.0f, -0.7f, 0.8f);
      
      glVertex3f(-0.6f, -0.3f, 0.8f);
      glVertex3f(-0.6f, -0.7f, 1.05f);
      glVertex3f(-0.6f, -0.3f, 1.05f);
      glVertex3f(-0.6f, -0.7f, 0.8f);
      
      //Ban Kiri Blkg
      //glColor3f(0.3f, 0.1f, 0.0f);
      glVertex3f(-0.6f, -0.3f, -1.05f);
      glVertex3f(-1.0f, -0.3f, -1.05f);
      glVertex3f(-1.0f, -0.7f, -1.05f);
      glVertex3f(-0.6f, -0.7f, -1.05f);
      
      glVertex3f(-0.6f, -0.3f, -0.8f);
      glVertex3f(-1.0f, -0.3f, -0.8f);
      glVertex3f(-1.0f, -0.7f, -0.8f);
      glVertex3f(-0.6f, -0.7f, -0.8f);
      
      glVertex3f(-0.6f, -0.7f, -0.8f);
      glVertex3f(-1.0f, -0.7f, -1.05f);
      glVertex3f(-0.6f, -0.7f, -1.05f);
      glVertex3f(-1.0f, -0.7f, -0.8f);
      
      glVertex3f(-1.0f, -0.3f, -0.8f);
      glVertex3f(-1.0f, -0.7f, -1.05f);
      glVertex3f(-1.0f, -0.3f, -1.05f);
      glVertex3f(-1.0f, -0.7f, -0.8f);
      
      glVertex3f(-0.6f, -0.3f, -0.8f);
      glVertex3f(-0.6f, -0.7f, -1.05f);
      glVertex3f(-0.6f, -0.3f, -1.05f);
      glVertex3f(-0.6f, -0.7f, -0.8f);
   	  glEnd();
 
    //glFlush();
   glDisable(GL_TEXTURE_2D);

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
   //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Enable double buffered mode
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize(640, 480);   // Set the window's initial width & height
   glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
   glutCreateWindow(title);          // Create window with the given title
   loadTextureFromFile( filename );
   
   glutDisplayFunc(display);       // Register callback handler for window re-paint event
   initGL();                       // Our own OpenGL initialization
   AmbientLighting();
   PointLight(1,1,1, 0, 1, 1);
   //spotlight();
   glutReshapeFunc(reshape);       // Register callback handler for window re-size event
   glutIdleFunc(display);
   glutSpecialFunc(pressKey);
   glutIgnoreKeyRepeat(1);
   glutSpecialUpFunc(releaseKey);
   glutMouseFunc(mouseButton);
   glutMotionFunc(mouseMove);

   
   glutMainLoop();                 // Enter the infinite event-processing loop
   return 0;
}
