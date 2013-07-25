#include <assert.h>
#include <fstream>

#include <stdlib.h>
#include <GL/glut.h>

#include "LightManager.h"

using namespace std;

LightManager::LightManager(){
    light1 = false;
    light2 = true;
    light3 = false;
    light4 = false;
    material = true;
    metal = false;
    braking = false;
    headlight = false;
}

void LightManager::selectSpotlight()
{
    GLfloat light_direction[] = { 0.0, -1, -1, 1 };
    GLfloat light_positiona[] = { 0, 5, 5, 1 };
    GLfloat diffuse7[] = { 0.8, 0.9, 1, 1.0 };
    GLfloat specular7[] = { 1, 1,1, 1 };
    //GLfloat specular5[] = { 0.6, 0.8, 1.0, 1.0 };
    //GLfloat diffuse5[] = { 0.6, 0.8, 1.0, 1.0 };
    ////High Beams
    //glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, 25);
    //glLightf(GL_LIGHT7, GL_SPOT_EXPONENT, 40.0f);
    //glLightfv(GL_LIGHT7, GL_DIFFUSE, diffuse5);
    //glLightfv(GL_LIGHT7, GL_SPECULAR, specular5);
    //glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, light_direction);
    //glLightf(GL_LIGHT7, GL_LINEAR_ATTENUATION, 0.01f);

		glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, 75);
	glLightf(GL_LIGHT7, GL_SPOT_EXPONENT, 25.0f);
	glLightfv(GL_LIGHT7, GL_DIFFUSE, diffuse7);
	glLightfv(GL_LIGHT7, GL_SPECULAR, specular7);
	//glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, light_direction);
	//glLightf(GL_LIGHT7, GL_LINEAR_ATTENUATION, 5);
	glLightfv(GL_LIGHT7, GL_POSITION, light_positiona);
	glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, light_direction);


	glEnable(GL_LIGHTING);	
	glEnable(GL_LIGHT7);

    //GLfloat specular[] = { 0.5, 0.5,0.5, 1 };
    //GLfloat shininess[] = { 5.0 };
    //GLfloat light_positiona[] = { 0, 2, 5, 0.0 };
    //GLfloat diffuse3[] = { 0.3, 0.3, 0.3, 1.0 };
    //GLfloat ambient_ligth[] = {0.2,0.2,0.2,1};
    //GLfloat ambient_ligthBlack[] = {0.1,0.1,0.1,1};

    //glLightfv(GL_LIGHT7, GL_AMBIENT, ambient_ligthBlack);
    //glLightfv(GL_LIGHT7, GL_DIFFUSE, diffuse3);
    //glLightfv(GL_LIGHT7, GL_SPECULAR, specular);
    //glLightfv(GL_LIGHT7, GL_POSITION, light_positiona);
    ////glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, light_direction);
    //glLightf(GL_LIGHT7, GL_LINEAR_ATTENUATION, 0.0f);

}

void LightManager::InitializeLights()
{
        /* turn on default lighting */
    GLfloat specular[] = { 0.3, 0.33,0.3, 1 };
    GLfloat shininess[] = { 5.0 };
    GLfloat light_positiona[] = { 0, 200, -80, 1.0 };
    GLfloat diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat ambient_ligth[] = {0.2,0.2,0.2,1};
    GLfloat ambient_ligthBlack[] = {0.1,0.1,0.1,1};

    GLfloat diffuse3[] = { 0.25, 0, 0, 1.0 };
    GLfloat ambient_ligth3[] = {0.01,0,0,1};

    GLfloat diffuse4[] = { 0.5, 0.5, 0, 1.0 };
    GLfloat ambient_ligth4[] = {0.1,0.1,0,1};

    GLfloat specular1[] = { 0.7, 0.7, 0.2, 1.0 };
    GLfloat light_position1[] = { 1.4, 1.2, 0, 1.0 };
    GLfloat diffuse1[] = { 0.8, 0.8, 0.5, 1.0 };
    //GLfloat diffuse1[] = { 1, 0,0, 1.0 };
    GLfloat light_direction[] = { 10.0, 0.0, 10.0, 1 };
    //    
    //GLfloat specular2[] = { 1.0, 0.4, 0.4, 1.0 };
    //GLfloat light_position2[] = { -1.4, 1.0, 0, 1.0 };
    //GLfloat diffuse2[] = { 1.0, 0.0, 0.0, 1.0 };
    //GLfloat light_direction2[] = { 10.0, 0, 0, 1 };

    //GLfloat specular5[] = { 0.6, 0.8, 1.0, 1.0 };
    //GLfloat diffuse5[] = { 0.6, 0.8, 1.0, 1.0 };
    
    //SKY
    glClearColor (0.1, 0.1, 0.4, 0.0);
    glShadeModel (GL_SMOOTH);

    //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    //glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_ligth);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_positiona);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);
        
    glLightfv(GL_LIGHT3, GL_AMBIENT, ambient_ligth);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse3);
    glLightfv(GL_LIGHT3, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT3, GL_POSITION, light_positiona);
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light_direction);
    glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.0f);
        
    glLightfv(GL_LIGHT4, GL_AMBIENT, ambient_ligth);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, diffuse4);
    glLightfv(GL_LIGHT4, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT4, GL_POSITION, light_positiona);
    glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, light_direction);
    glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.0f);

    ////Regular headlight
    //glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 40);
    //glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20);
    //glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
    //glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
    //glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_direction);
    //glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.1f);
    //    
    ////Breaking light
    //glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 60);
    //glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 20.0f);
    //glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);
    //glLightfv(GL_LIGHT2, GL_SPECULAR, specular2);
    //glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_direction2);
    //glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 5);

    ////High Beams
    //glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 25);
    //glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 40.0f);
    //glLightfv(GL_LIGHT5, GL_DIFFUSE, diffuse5);
    //glLightfv(GL_LIGHT5, GL_SPECULAR, specular5);
    //glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, light_direction);
    //glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.01f);
    
    glEnable(GL_LIGHTING);
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT5);
    //glDisable(GL_LIGHT7);
    //glEnable(GL_COLOR_MATERIAL);
    //glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

LightManager::~LightManager() {
}
