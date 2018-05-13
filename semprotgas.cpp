#include <GL/glut.h>
#include <windows.h>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <time.h>
#include <vector>
#include <stdlib.h>

#include "imageloader.h"
#include "imageloader.cpp"
#include "vec3f.h"

using namespace std;

//const float PI = 3.1415926535f;

float randomFloat() {
	return  (float)rand() / ((float) RAND_MAX +1);
}

struct Particle {
	Vec3f pos;
	Vec3f velocity;
	Vec3f color;
	float timeAlive;
	float lifespan;
};

Vec3f rotate(Vec3f v, Vec3f axis, float degrees) {
	axis.normalize();
	float radians = degrees * PI / 180;
	float s = sin(radians);
	float c = cos(radians);
	return v * c + axis * axis.dot(v) * (1 -c) + v.cross(axis) * s;
}

Vec3f adjParticlePos(Vec3f pos) {
	return pos;
	//rotate(pos, Vec3f(1,0,0), -30);
}

bool compareParticles(Particle* particle1, Particle* particle2) {
	return adjParticlePos(particle1->pos)[2] < adjParticlePos(particle2->pos)[2];
}

const float GRAVITY = 3.0f;
const int NUM_PARTICLES = 1000;
const float STEP_TIME = 0.01f;
const float PARTICLE_SIZE = 0.02f;

class ParticleEngine {
	private:
		GLuint textureId;
		Particle particles[NUM_PARTICLES];
		float timeUntilNextStep;
		float colorTime;
		float angle;
		
		Vec3f curColor() {
			Vec3f color;
			/*if (colorTime < 0.166667f) {
				color = Vec3f(1.0f, colorTime * 6, 0.0f);
			}
			else if (colorTime < 0.333333f) {
				color = Vec3f((0.333333f - colorTime) * 6, 1.0f, 0.0f);
			}
			else if (colorTime < 0.5f) {
				color = Vec3f(0.0f, 1.0f, colorTime - 0.333333f)* 6;
			}
			else if (colorTime < 0.666667f) {
				color = Vec3f(0.0f, (0.666667f - colorTime) * 6, 1.0f);
			}
			else if (colorTime < 0.833333f) {
				color = Vec3f((colorTime - 0.666667f) * 6, 0.0f, 1.0f);
			}
			else {
				color = Vec3f(1.0f, 0.0f, (1.0f - colorTime) * 6);
			}*/
			
			
			
			return color;
		}
		
		Vec3f curVelocity() {
			return Vec3f(2 * cos(angle), 2.0f, 2 * sin(angle));
		}
		
		void createParticle(Particle* p) {
			p->pos = Vec3f(0, 0, 0);
			p->velocity = curVelocity() + Vec3f(0.5f * randomFloat() - 0.25f,
												0.5f * randomFloat() - 0.25f,
												0.5f * randomFloat() - 0.25f);
			p->color = curColor();
			p->timeAlive = 0;
			p->lifespan = randomFloat() + 1;
		}
		
		void step() {
			colorTime += STEP_TIME / 10;
			while (colorTime >= 1) {
				colorTime -= 1;
			}
			
			angle += 0.5f * STEP_TIME;
			while (angle > 2 * PI) {
				angle -= 2 * PI;
			}
			
			for(int i = 0; i < NUM_PARTICLES; i++){
				Particle* p = particles + i;
				
				p->pos += p->velocity * STEP_TIME;
				p->velocity += Vec3f(0.0f, -GRAVITY * STEP_TIME, 0.0f);
				p->timeAlive += STEP_TIME;
				if (p->timeAlive > p->lifespan) {
					createParticle(p);
				}
			}
		}
		
	public :
		ParticleEngine(GLuint textureId1) {
			textureId = textureId1;
			timeUntilNextStep = 0;
			colorTime = 0;
			angle = 0;
			for(int i = 0; i < NUM_PARTICLES; i++) {
				createParticle(particles + 1);
			}
			for(int i = 0; i < 5/STEP_TIME; i++) {
				step();
			}
		}
		
		void advance(float dt) {
			while(dt > 0) {
				if (timeUntilNextStep < dt) {
					dt -= timeUntilNextStep;
					step();
					timeUntilNextStep = STEP_TIME;
				}
				else{
					timeUntilNextStep -= dt;
					dt = 0;
				}
			}
		}
		
		void draw() {
			vector<Particle*> ps;
			for(int i = 0; i < NUM_PARTICLES; i++) {
				ps.push_back(particles + i);
			}
			
			sort(ps.begin(), ps.end(), compareParticles);
			
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			glBegin(GL_QUADS);
			for(unsigned int i = 0; i < ps.size(); i++) {
				Particle* p = ps[i];
				/*glColor4f(p->color[0], p->color[1], p->color[2],
				(1 - p->timeAlive / p->lifespan));*/
				float size = PARTICLE_SIZE / 2;
				
				Vec3f pos = adjParticlePos(p->pos);
				
				glTexCoord2f(0, 0);
				glVertex3f(pos[0] - size, pos[1] - size, pos[2]);
				glTexCoord2f(0, 1);
				glVertex3f(pos[0] - size, pos[1] + size, pos[2]);
				glTexCoord2f(1, 1);
				glVertex3f(pos[0] + size, pos[1] + size, pos[2]);
				glTexCoord2f(1, 0);
				glVertex3f(pos[0] + size, pos[1] - size, pos[2]);
			}
			glEnd();
		}
};

const int TIMER_MS = 25;

ParticleEngine* _particleEngine;
GLuint _textureId;

void cleanup() {
	delete _particleEngine;
}

void handleKeypress(unsigned char key, int x, int y){
	switch (key) {
		case 27 :
			cleanup();
			exit(0);
	}
}


char* addAlphaChannel(Image* image, Image* alphaChannel) {
	char* pixels = new char[image->width * image->height * 4];
	for(int y = 0; y < image->height; y++) {
		for(int x = 0; x < image->width; x++) {
			for(int j = 0; j < 3; j++) {
				pixels[4 * (y * image->width + x) + j] = image->pixels[3 * ( y* image->width + x) + j];
			}
			pixels[4 * (y * image->width + x) + 3] = alphaChannel->pixels[3 * (y * image->width + x)];
		}
	}
	
	return pixels;
}

GLuint loadAlphaTexture(Image* image, Image* alphaChannel) {
	char* pixels = addAlphaChannel(image, alphaChannel);
	
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGBA,
				 image->width, image->height,
				 0,
				 GL_RGBA,
				 GL_UNSIGNED_BYTE,
				 pixels);
				 
	delete pixels;
	return textureId;
}

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	Image* image = loadBMP("circle.bmp");
	Image* alphaChannel = loadBMP("circlealpha.bmp");
	_textureId = loadAlphaTexture(image, alphaChannel);
	delete image;
	delete alphaChannel;
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)2 / (double)h, 1.0, 200.0);
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -10.0f);
	glScalef(2.0f, 2.0f, 2.0f);
	
	_particleEngine->draw();
	
	glutSwapBuffers();
}

void update(int value){
	_particleEngine->advance(TIMER_MS / 1000.0f);
	glutPostRedisplay();
	glutTimerFunc(TIMER_MS, update, 0);
}

int main(int argc, char** argv) {
	srand((unsigned int) time(0));
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400,400);
	
	glutCreateWindow("semprotgas");
	initRendering();
	
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	
	_particleEngine = new ParticleEngine(_textureId);
	
	glutTimerFunc(TIMER_MS, update, 0);
	
	glutMainLoop();
	
	return 0;
}
