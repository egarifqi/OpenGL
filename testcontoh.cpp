#include <iostream>
#include <GL/glut.h>
 
using namespace std;
 
void display();
void LookAt();
void DrawCube();
 
void DrawSphere(const float scale, const float alpha);
 
void AmbientLighting();
void PointLight(const float x, const float y, const float z,  const float amb, const float diff, const float spec);
 
int WindowHeight = 1000;
int WindowWidth = 1000;
 
int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(WindowWidth, WindowHeight);
  glutInitWindowPosition(0, 0);
 
  glutCreateWindow("Lighting Example");
 
  glutDisplayFunc(display);
 
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
 
  AmbientLighting();
  PointLight(1,1,1, 0, 1, 1);
  //PointLight(10,10,10);
  //PointLight(0,0,0, 0, 1, 1);
 
  LookAt();
 
  glutMainLoop();
 
  return 0;
}
 
void AmbientLighting()
{
  glEnable(GL_LIGHTING);
 
  double amb = .3;
  GLfloat global_ambient[] = {amb,amb,amb, 0.1};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
}
 
void PointLight(const float x, const float y, const float z, const float amb, const float diff, const float spec)
{
  glEnable(GL_LIGHTING);
 
  /*
  GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  */
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
 
void DrawSphere(const float radius, const float alpha)
{
  glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;
  glEnable ( GL_COLOR_MATERIAL ) ;
  glColor4f(1.0f, 0.0f, 0.0f, alpha);
 
  //equivalent to doing this:
  //float diffuse[4] =  {1.0f, 1.0f, 0.0f, alpha};
      //glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
 
 
  GLUquadricObj *quadratic;
  quadratic = gluNewQuadric();
  gluQuadricNormals(quadratic, GLU_SMOOTH);
  gluSphere(quadratic, radius, 32, 32);
}
 
void LookAt()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70, 1, 1, 100);
 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
 
  gluLookAt(2, 2, 10, 2, 0, 0, 0, 1, 0);
}
 
void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
 
  DrawSphere(1, 1);
  DrawSphere(1.1, .5);
  DrawSphere(1.2, .2);
 
  glutSwapBuffers();
}
