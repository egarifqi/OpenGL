/*
 * OGL01Shape3D.cpp: 3D Shapes
 */
#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <math.h>
#include "RgbImage.h"
#include "RgbImage.cpp"

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <time.h>
 
#define MAX_PARTICLES 1000
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
float slowdown = 2.0;
float velocity = 0.0;
int loop;
int fall;

/////////////////////////////////////////////////////////////////////

int wsize = 750;
bool paused = false;
int squaresize = 16;
GLubyte square[16][16][2];
GLuint textureObj;
std::vector<int> arrayOfSquares;  // contains x- and y-coords, x- and y- velocity, current rotation, rotation velocity, and frames since creation
int smokeAmt=5,varAmt=0;  // control amount and placement of smoke
int wind=0;

// creates random, smoothed noise texture to use as the basis of the smoke
void makeSquare(){
  int i,j,k,coeffs[6];
  double offsets[6];
  for(i=0;i<6;i++){
    offsets[i] = 2*M_PI * (rand()%1000)/1000.0;
  }

  for(i=0;i<squaresize;i++){
    for(j=0;j<squaresize;j++){
      square[i][j][0] = 0;
      for(k=0;k<6;k++) square[i][j][0]+=40*sin(sin(i+offsets[k])*(j+1)*2);
      square[i][j][1] = 255*(1-pow(hypot(squaresize/2-i,squaresize/2-j)/hypot(squaresize/2,squaresize/2),.5));
    }
  }

  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glGenTextures(1,&textureObj);
  glBindTexture(GL_TEXTURE_2D,textureObj);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE_ALPHA,squaresize,squaresize,0,GL_LUMINANCE_ALPHA,GL_UNSIGNED_BYTE,square);
}

void reshapeSmoke(int w,int h){
  glViewport(0,0,w,h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0,wsize,0,wsize);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void displaySmoke(){
  int i;
  double alpha,size;
  glClear(GL_COLOR_BUFFER_BIT);

  glBindTexture(GL_TEXTURE_2D,textureObj);
  glMatrixMode(GL_MODELVIEW_MATRIX);
  glLoadIdentity();

  for(i=arrayOfSquares.size()-7;i>=0;i-=7){
    // place
    glLoadIdentity();
    glTranslatef(arrayOfSquares[i],arrayOfSquares[i+1],0.0);
    glRotatef(arrayOfSquares[i+4],0.0,0.0,1.0);

    // set apparent alpha based on time since creation
    alpha = 1.0 - arrayOfSquares[i+6]/60.0;
    glColor4f(alpha,alpha,alpha,alpha);

    // draw with size based on time
    size = .075 * (1+arrayOfSquares[i+6]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0,0.0);
    glVertex2f(-squaresize*size,-squaresize*size);
    glTexCoord2f(1.0,0.0);
    glVertex2f(squaresize*size,-squaresize*size);
    glTexCoord2f(1.0,1.0);
    glVertex2f(squaresize*size,squaresize*size);
    glTexCoord2f(0.0,1.0);
    glVertex2f(-squaresize*size,squaresize*size);
    glEnd();

    // reset location
    glLoadIdentity;
  }

  glutSwapBuffers();
}

// runs every 50 ms; moves the smoke squares
void increment(int t){
  int i,j;
  while(rand()%(smokeAmt+1)){  // add square
    arrayOfSquares.push_back(wsize/2+rand()%(2*varAmt+1)-varAmt);  // X
    arrayOfSquares.push_back(wsize/4+rand()%(2*varAmt+1)-varAmt);  // Y
    arrayOfSquares.push_back(rand()%5-2);  // X-vel
    arrayOfSquares.push_back(rand()%3);  // Y-vel
    arrayOfSquares.push_back(rand()%360);  // rotation
    arrayOfSquares.push_back(rand()%11-5);  // rotation-vel
    arrayOfSquares.push_back(0);  // timer
  }

  for(i=0;i<arrayOfSquares.size();i+=7){
    // delete if out of screen or at time of 3 sec
    if(arrayOfSquares[i]<(-squaresize*4) || arrayOfSquares[i]>wsize+squaresize*4 || arrayOfSquares[i+1]>wsize+squaresize*4 || arrayOfSquares[i+6]>60){
      for(j=0;j<7;j++){
	arrayOfSquares.erase(arrayOfSquares.begin()+i);
      }
      i -= 7;
      continue;
    }

    // add velocities to current position
    arrayOfSquares[i] += arrayOfSquares[i+2];
    arrayOfSquares[i+1] += arrayOfSquares[i+3];
    arrayOfSquares[i+4] += arrayOfSquares[i+5];

    // accelerate
    if(rand()%5==0) arrayOfSquares[i+3]++;
    if(wind && rand()%(abs(wind)+1) && rand()%5==0) arrayOfSquares[i+2]+=(wind)/abs(wind);

    // add to timer
    arrayOfSquares[i+6]++;
  }

  // reset timer
  if(!paused) glutTimerFunc(50,increment,0);
  glutPostRedisplay();
}

void reset(){
  arrayOfSquares.clear();
  makeSquare();
  paused = false;
  wind = 0;
  smokeAmt = 5;
  varAmt = 0;
}

void keyboard(unsigned char key,int x,int y){
  switch(key){
  case 'q':
  case 'Q':
  case 27:
    exit(0);
    break;
  case 'r':
  case 'R':
    reset();
    break;
  case 'p':
  case 'P':
    if(paused){
      glutTimerFunc(50,increment,0);
      paused = false;
    }
    else paused=true;
    break;
  default:
    break;
  }
}

void keyboardSpecials(int key,int x,int y){
  switch(key){
  case GLUT_KEY_UP:
    if(smokeAmt<10) smokeAmt++;
    break;
  case GLUT_KEY_DOWN:
    if(smokeAmt>0) smokeAmt--;
    break;
  case GLUT_KEY_LEFT:
    if(wind>-10) wind--;
    break;
  case GLUT_KEY_RIGHT:
    if(wind<10) wind++;
    break;
  default:
    break;
  }
}

/*int main(int argc,char* argv[]){
  srand(time(NULL));

  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(wsize,wsize);
  glutInitWindowPosition((1280-wsize)/2,(800-wsize)/2);
  glutCreateWindow("Smoke");

  glClearColor(1.0,1.0,1.0,1.0);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(keyboardSpecials);

  reset();
  glutTimerFunc(50,increment,0);
  glutMainLoop();
}*/

/////////////////////////////////////////////////////////////////////

typedef struct {
  // Life
  bool alive;	// is the particle alive?
  float life;	// particle lifespan
  float fade; // decay
  // color
  float red;
  float green;
  float blue;
  // Position/direction
  float xpos;
  float ypos;
  float zpos;
  // Velocity/Direction, only goes down in y dir
  float vel;
  // Gravity
  float gravity;
}particles;

particles par_sys[MAX_PARTICLES];

void initParticles(int i) {
    par_sys[i].alive = true;
    par_sys[i].life = 1.0;
    par_sys[i].fade = float(rand()%100)/1000.0f+0.003f;

    par_sys[i].xpos = (float) (rand() % 21) - 10;
    par_sys[i].ypos = 15.0;
    par_sys[i].zpos = (float) (rand() % 21) - 10;

    par_sys[i].red = 0.5;
    par_sys[i].green = 0.5;
    par_sys[i].blue = 1.0;

    par_sys[i].vel = velocity;
    par_sys[i].gravity = -0.8;//-0.8;

}
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
   
   // Initialize particles
    for (loop = 0; loop < MAX_PARTICLES; loop++) {
        initParticles(loop);
    }
}

// For Rain
void drawRain() {
  float x, y, z;
  for (loop = 0; loop < MAX_PARTICLES; loop=loop+2) {
    if (par_sys[loop].alive == true) {
      x = par_sys[loop].xpos;
      y = par_sys[loop].ypos;
      z = par_sys[loop].zpos - 2.0;

      // Draw particles
      glColor3f(0.5, 0.5, 1.0);
      glBegin(GL_LINES);
        glVertex3f(x, y, z);
        glVertex3f(x, y+0.5, z);
      glEnd();

      // Update values
      //Move
      // Adjust slowdown for speed!
      par_sys[loop].ypos += par_sys[loop].vel / (slowdown*1000);
      par_sys[loop].vel += par_sys[loop].gravity;
      // Decay
      par_sys[loop].life -= par_sys[loop].fade;

      if (par_sys[loop].ypos <= -10) {
        par_sys[loop].life = -1.0;
      }
      //Revive
      if (par_sys[loop].life < 0.0) {
        initParticles(loop);
      }
    }
  }
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
   
   
   drawRain();
   
   int i;
	double alpha,size;
	
	//glClear(GL_COLOR_BUFFER_BIT);

  //glBindTexture(GL_TEXTURE_2D,textureObj);
  //glMatrixMode(GL_MODELVIEW_MATRIX);
	//glLoadIdentity();
	
	
   for(i=arrayOfSquares.size()-7;i>=0;i-=7){
    // place
    glLoadIdentity();
    glTranslatef(arrayOfSquares[i],arrayOfSquares[i+1],0.0);
    glRotatef(arrayOfSquares[i+4],0.0,0.0,1.0);

    // set apparent alpha based on time since creation
    alpha = 1.0 - arrayOfSquares[i+6]/60.0;
    glColor4f(alpha,alpha,alpha,alpha);

    // draw with size based on time
    size = .075 * (1+arrayOfSquares[i+6]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0,0.0);
    glVertex3f(-squaresize*size,-squaresize*size,1.0f);
    glTexCoord2f(1.0,0.0);
    glVertex3f(squaresize*size,-squaresize*size,1.0f);
    glTexCoord2f(1.0,1.0);
    glVertex3f(squaresize*size,squaresize*size,1.0f);
    glTexCoord2f(0.0,1.0);
    glVertex3f(-squaresize*size,squaresize*size,1.0f);
    glEnd();

    // reset location
    
   }
   
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
   
   glutDisplayFunc(display);
   //glutDisplayFunc(displaySmoke);       // Register callback handler for window re-paint event
   initGL();                       // Our own OpenGL initialization
   AmbientLighting();
   PointLight(1,1,1, 0, 1, 1);
   //spotlight();
   glutReshapeFunc(reshape);
   //glutReshapeFunc(reshapeSmoke);       // Register callback handler for window re-size event
   glutIdleFunc(display);
   //glutIdleFunc(displaySmoke);
   glutSpecialFunc(pressKey);
   glutIgnoreKeyRepeat(1);
   glutSpecialUpFunc(releaseKey);
   glutMouseFunc(mouseButton);
   glutMotionFunc(mouseMove);

   
   glutMainLoop();                 // Enter the infinite event-processing loop
   return 0;
}
