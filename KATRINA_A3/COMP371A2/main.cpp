//COMP371 Assignemnt 3
//By: Katrina Anderson

// CREDITS
// Textures				- www.cgtextures.com
// imageLoader.cpp		- Comp371 Tutorial from week 9

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <time.h>
#include <GL/glut.h>

#include "imageloader.h"

using namespace std;

GLUquadricObj *quadratic;

bool isWireFrameOn = false;			//Indicates if the objects are displayed as wire 		
bool isOrthographicViewOn = false;  //Indicates if the view is orthographic
bool isMoving = false;
bool isFirstPerson = false;
bool isLightOn = true;
bool isLight1On = false;
bool isLight2On = false;
bool isLight2Moving = true;
bool isLight3On = false;
bool isLight4On = false;
bool isHighBeamOn = false;
bool isRustOn = false;

const double DEG2RAD = 3.14159265 / 180;	//Conversion to radians
const double RAD2DEG = 180/3.14159265;
int feildOfView = 50;						//Initiates the field of view for perspective
GLdouble nearH = 0.0;						//Height of near plane
GLdouble nearW = 0.0;						//Width of near plane
GLdouble near = 0.10;						//Distance to near plane
GLdouble far = 100.0;						//Distance to far plane
			
GLfloat X = 0.0f;		// Translate screen to x direction 
GLfloat Y = 0.0f;		// Translate screen to y direction 
GLfloat Z = 0.0f;		// Translate screen to z direction

GLfloat bikeX = 14.0f;		
GLfloat bikeY = 0.0f;		
GLfloat bikeZ = 0.0f;

int bikeRotation = 0;
int tireRotation = 0;
int bikeDirection = -90;
float cameraDirection = 0;
int bikeTimer = 0;
int bikeAcceleration = 1;
GLfloat trackRadius = 10.0f;
GLfloat root = 2;

GLfloat rotX = 0.0f;	// Rotate screen on x axis 
GLfloat rotY = 0.0f;	// Rotate screen on y axis
GLfloat rotZ = 0.0f;	// Rotate screen on z axis

static float cx = 0.0f;   // Camera x
static float cy = 0.0f;   // Camera y
static float cz = 5.0f;   // Camera z

static float dx = 0.0f;   // Line of sight vector x
static float dy = 0.0f;   // Line of sight vector y
static float dz = -1.0f;  // Line of sight vector z

int frameCount = 1;
double timePrevious = 0;
float fps = 0;

//Array of texture ids.
GLuint _textureId[8];

//Light Stuff
GLfloat light2X;
GLfloat light2Y;
GLfloat light2Z;

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1 );
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, 
				GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	return textureId;
}

//Sets up the textures for the motorcycle
void setupTextures()
{
	gluQuadricDrawStyle(quadratic, GLU_FILL);
	gluQuadricNormals(quadratic, GLU_SMOOTH);
	gluQuadricTexture(quadratic, GL_TRUE);	
	
	Image* image = loadBMP("MetalPainted.bmp");
	_textureId[0] = loadTexture(image);
	image = loadBMP("Metal1.bmp");
	_textureId[1] = loadTexture(image);
	image = loadBMP("Metal5.bmp");
	_textureId[2] = loadTexture(image);
	image = loadBMP("Metal3.bmp");
	_textureId[3] = loadTexture(image);
	image = loadBMP("Leather.bmp");
	_textureId[4] = loadTexture(image);
	image = loadBMP("MetalPainted2.bmp");
	_textureId[5] = loadTexture(image);
	image = loadBMP("MetalRivets2.bmp");
	_textureId[6] = loadTexture(image);
	image = loadBMP("Metal2.bmp");
	_textureId[7] = loadTexture(image);
	delete image;
}

//Initiliaze the OpenGL window
void init(void)
{
	glClearColor (0.0, 0.0, 0.0, 0.0);			// Clear the color 
	glEnable(GL_DEPTH_TEST);
	glShadeModel (GL_SMOOTH);					// Set the shading model to GL_FLAT
	glEnable (GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);		// Set Line Antialiasing
	quadratic=gluNewQuadric();
	setupTextures();
}

//Shines a spot light from the motorcycle headlamp
void EmitLight1(double lightX, double lightY, double lightZ, double spotX, double spotY, double spotZ){
	
	glPushMatrix();
	if(isLightOn && isLight1On)
	{		
		//Set Ambiant
		float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
		glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb);

		//Set Diffuse
		float lightDif[] = { 1.0, 1.0, 1.0, 1.0 };
		glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDif);

		//Set Specular
		float lightSpe[] = { 1.0, 1.0, 1.0, 1.0 };
		glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpe);
						
		//Enable Light Source
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT1);

		//Set Light Position
		GLfloat light_position[] = {lightX, lightY, lightZ, 1.0};
		glLightfv(GL_LIGHT1,GL_POSITION,light_position);


		//Set Spot Light Direction
		if(isHighBeamOn)
		{
			GLfloat spotDir[] = {spotX, spotY, spotZ+1};
			glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,spotDir);
			glLightf(GL_LIGHT1,GL_SPOT_EXPONENT, 4.0);
			glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 100);

			// Sphere Indicating Light Direction
			//glPushMatrix();
			//glDisable(GL_LIGHTING);
			//glTranslatef(spotDir[0], spotDir[1], spotDir[2]);
			//glColor3f(1.0, 0.0, 1.0); 
			//glutWireSphere(0.05, 8, 8);
			//glEnable(GL_LIGHTING);
			//glPopMatrix();
		}
		else
		{
			GLfloat spotDir[] = {spotX, spotY, spotZ};
			glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,spotDir);
			glLightf(GL_LIGHT1,GL_SPOT_EXPONENT, 4.0);
			glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60);

			// Sphere Indicating Light Direction
			//glPushMatrix();
			//glDisable(GL_LIGHTING);
			//glTranslatef(spotDir[0], spotDir[1], spotDir[2]);
			//glColor3f(1.0, 0.0, 1.0); 
			//glutWireSphere(0.05, 8, 8);
			//glEnable(GL_LIGHTING);
			//glPopMatrix();
		}

		//Set Material Properties
		glDisable(GL_COLOR_MATERIAL);
		float matSpec[] = { 1.0, 1.0, 1.0, 1.0};
		if(isRustOn)
		{
			float matShine[] = { 10.0 };
			glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
			glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
		}
		else
		{
			float matShine[] = { 100.0 };
			glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
			glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
		}

	
		// Sphere Indicating Light source
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glTranslatef(light_position[0], light_position[1], light_position[2]);
		glColor3f(1.0, 1.0, 1.0); 
		glutSolidSphere(0.04, 8, 8);
		glEnable(GL_LIGHTING);
		glPopMatrix();


	}
	else
	{
		glDisable(GL_LIGHT1);
	}
	glPopMatrix();
}

//Shines a blue light ontop of the motorcycle
void EmitLight2(double lightX, double lightY, double lightZ, double spotX, double spotY, double spotZ)
{
	if(isLightOn && isLight2On)
	{		
		//Set Ambiant
		float lightAmb[] = { 1.0, 1.0, 1.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);

		//Set Diffuse
		float lightDif[] = { 0.0, 0.0, 1.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif);

		//Set Specular
		float lightSpe[] = { 0.0, 0.0, 1.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpe);
						
		//Enable Light Source
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		//Set Light Position
		GLfloat light_position[] = {lightX, lightY, lightZ, 1.0};
		glLightfv(GL_LIGHT0,GL_POSITION,light_position);

		//Set Light Direction
		GLfloat spotDir[] = {spotX, spotY, spotZ};
		glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,spotDir);
		glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,4.0);
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 95);		

		//Set Material Properties
		glEnable(GL_COLOR_MATERIAL);
		float matSpec[] = { 0.0, 0.0, 1.0, 1.0};
		if(isRustOn)
		{
			float matShine[] = { 10.0 };
			glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
			glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
		}
		else
		{
			float matShine[] = { 100.0 };
			glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
			glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
		}
	
		// Sphere Indicating Light Position
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glTranslatef(light_position[0], light_position[1], light_position[2]);
		glColor3f(0.0, 0.0, 1.0); 
		glutWireSphere(0.05, 8, 8);
		glEnable(GL_LIGHTING);
		glPopMatrix();
	}
	else
	{
		glDisable(GL_LIGHT0);
	}   
}

//Shines a red light from the camera to the motorcycle.
void EmitLight3(double lightX, double lightY, double lightZ, double spotX, double spotY, double spotZ)
{
	if(isLightOn && isLight3On && !isFirstPerson)
	{		
		//Set Ambiant
		float lightAmb[] = { 1.0, 1.0, 1.0, 1.0 };
		glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmb);

		//Set Diffuse
		float lightDif[] = { 1.0, 0.0, 0.0, 1.0 };
		glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDif);

		//Set Specular
		float lightSpe[] = { 1.0, 0.0, 0.0, 1.0 };
		glLightfv(GL_LIGHT2, GL_SPECULAR, lightSpe);
						
		//Enable Light Source
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT2);

		//Set Light Position
		GLfloat light_position[] = {lightX, lightY, lightZ, 1.0};
		glLightfv(GL_LIGHT2,GL_POSITION,light_position);

		//Set Light Direction
		GLfloat spotDir[] = {spotX, spotY, spotZ};
		glLightfv(GL_LIGHT2,GL_SPOT_DIRECTION,spotDir);
		glLightf(GL_LIGHT2,GL_SPOT_EXPONENT, 4.0);
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 95);		

		//Set Material Properties
		glEnable(GL_COLOR_MATERIAL);
		float matSpec[] = { 1.0, 0.0, 0.0, 1.0};
		if(isRustOn)
		{
			float matShine[] = { 10.0 };
			glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
			glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
		}
		else
		{
			float matShine[] = { 100.0 };
			glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
			glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
		}
	}
	else
	{
		glDisable(GL_LIGHT2);
	}   
}

//Shines a red light from the camera to the motorcycle.
void EmitLight4(double lightX, double lightY, double lightZ, double spotX, double spotY, double spotZ)
{
	if(isLightOn && isLight4On)
	{		
		//Set Ambiant
		float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
		glLightfv(GL_LIGHT3, GL_AMBIENT, lightAmb);

		//Set Diffuse
		float lightDif[] = { 0.0, 0.0, 0.0, 0.0 };
		glLightfv(GL_LIGHT3, GL_DIFFUSE, lightDif);

		//Set Specular
		float lightSpe[] = { 1.0, 1.0, 1.0, 1.0 };
		glLightfv(GL_LIGHT3, GL_SPECULAR, lightSpe);
						
		//Enable Light Source
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT3);

		//Set Light Position
		GLfloat light_position[] = {lightX, lightY, lightZ, 1.0};
		glLightfv(GL_LIGHT3,GL_POSITION,light_position);

		//Set Light Direction
		GLfloat spotDir[] = {spotX, spotY, spotZ};
		glLightfv(GL_LIGHT3,GL_SPOT_DIRECTION,spotDir);
		glLightf(GL_LIGHT3,GL_SPOT_EXPONENT, 4.0);
		glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 95);		

		//Set Material Properties
		glEnable(GL_COLOR_MATERIAL);
		float matSpec[] = { 1.0, 1.0, 1.0, 1.0};
		if(isRustOn)
		{
			float matShine[] = { 10.0 };
			glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
			glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
		}
		else
		{
			float matShine[] = { 120.0 };
			glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
			glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
		}
	}
	else
	{
		glDisable(GL_LIGHT3);
	}   
}

//Creates a directional light over a given object position
void DirectionalLight(double lightX, double lightY, double lightZ)
{
	GLfloat blackAmbientLight[] = {0.0, 0.0, 0.0}; 
	GLfloat redAmbientDiffuse[] = {1.0, 0.0, 0.0}; 
	GLfloat whiteDiffuseLight[] = {1.0, 1.0, 1.0}; 
	GLfloat whiteSpecularLight[] = {1.0, 1.0, 1.0}; 
	GLfloat yellowAmbientDiffuse[] = {1.0f, 1.0f, 0.0f, 1.0f};
	GLfloat light_position[] = {lightX, lightY, lightZ, 0.0};
		
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, whiteDiffuseLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, yellowAmbientDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

//This method is responsible for drawing the inside spokes of a motercycle 
//tire and is a helper mthod to the DrawMotercycleTire function.
void DrawMotercycleTireSpoke(GLdouble rotateX, GLdouble rotateY)
{
	glPushMatrix();
	glRotatef(90,rotateX,rotateY,0);
	glTranslatef(0,0,-.08);	
	if(isWireFrameOn)
	{		
		glColor3f(0.70,0.70,0.70);
		gluQuadricDrawStyle(quadratic, GLU_LINE);
		gluCylinder(quadratic,0.0075f,0.0075f,0.16f,32,32);
	}
	else
	{
		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureId[1]); 
		gluCylinder(quadratic,0.0075f,0.0075f,0.16f,32,32); 
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();
}

//This method is responsible for drawing a tire of a motercycle.
void DrawMotercycleTire(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
	glPushMatrix();
	glTranslatef(translateX,translateY,translateZ);
	glRotatef(-tireRotation, 0,0,1);
	DrawMotercycleTireSpoke(-1,1);
	DrawMotercycleTireSpoke(1,1);
	DrawMotercycleTireSpoke(0,1);
	DrawMotercycleTireSpoke(1,0);
	glColor3f(0.30,0.30,0.30);
	if(isWireFrameOn)
	{
		glutWireTorus(0.03, 0.1, 32, 32);
	}
	else
	{
		glutSolidTorus(0.03, 0.1, 32, 32);
	}

	glPopMatrix();
}

//This method is responsible for drawing any cross bars between the handle bar shafts.
void DrawHandleCrossBar(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
	glPushMatrix();
	if(isWireFrameOn)
	{
		gluQuadricDrawStyle(quadratic, GLU_LINE);
		glColor3f(0.90,0.91,0.98);
	}
	else
	{
		gluQuadricDrawStyle(quadratic, GLU_FILL);
		glColor3f(1,1,1);
	}
	glTranslatef(translateX, translateY, translateZ);	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId[2]);
	gluCylinder(quadratic,0.02f,0.02f,0.2f,32,32);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

//This method is responsible for drawing the inside spokes of a motercycle 
//tire and is a helper mthod to the DrawMotercycleTire function.
void DrawHandleBarShaft(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
	glPushMatrix();
	if(isWireFrameOn)
	{
		gluQuadricDrawStyle(quadratic, GLU_LINE);
		glColor3f(0.8,0.498039,0.196078);
	}
	else
	{
		gluQuadricDrawStyle(quadratic, GLU_FILL);
		glColor3f(1,1,1);
	}
	glTranslatef(translateX,translateY,translateZ);
	glRotatef(-90,1,0,0);
	glRotatef(-52.5,0,1,0);	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId[1]); 
	gluCylinder(quadratic,0.02f,0.02f,0.5f,32,32);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

//This method is responsible for drawing a handle bar for the motercycle.
void DrawHandleBars(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
	glPushMatrix();
	glTranslatef(translateX,translateY,translateZ);
	if(isWireFrameOn)
	{
		glColor3f(1,1,0);
		gluQuadricDrawStyle(quadratic, GLU_LINE);
	}
	else
	{
		glColor3f(1,1,1);
		gluQuadricDrawStyle(quadratic, GLU_FILL);
	}

	//Top cross bar
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId[2]);
	gluCylinder(quadratic,0.015f,0.015f,0.2f,32,32);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Right Handle
	glPushMatrix();
	glTranslatef(-.05,.05,0);
	glRotatef(-135,0,1,0);
	glRotatef(15,1,0,0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId[2]);
	gluCylinder(quadratic,0.02f,0.02f,0.1f,32,32);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Left Handle
	glPushMatrix();
	glTranslatef(-.05,.05,0.2);
	glRotatef(-45,0,1,0);
	glRotatef(15,1,0,0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId[2]);
	gluCylinder(quadratic,0.02f,0.02f,0.1f,32,32);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPopMatrix();
}

//This method draws the part of the chassy that holds up the engine, gas tank and seat
void DrawEngineSupport(GLfloat translateX, GLfloat translateY, GLfloat translateZ){

	glPushMatrix();
	if(isWireFrameOn)
	{
		gluQuadricDrawStyle(quadratic, GLU_LINE);
		glColor3f(0.858824 , 0.858824, 0.439216);
	}
	else
	{
		gluQuadricDrawStyle(quadratic, GLU_FILL);
		glColor3f(1, 1, 1);
	}
	glTranslatef(translateX,translateY,translateZ);
	glRotatef(90,1,0,0);
	glRotatef(-52.5,0,1,0);	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId[2]);
	gluCylinder(quadratic,0.02f,0.02f,0.2f,32,32);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

//This method draws the part of the chassy that holds up the engine, gas tank and seat
void DrawOtherEngineSupport(GLfloat translateX, GLfloat translateY, GLfloat translateZ){

	glPushMatrix();
	if(isWireFrameOn)
	{
		gluQuadricDrawStyle(quadratic, GLU_LINE);
		glColor3f(0.858824 , 0.858824, 0.439216);
	}
	else
	{
		gluQuadricDrawStyle(quadratic, GLU_FILL);
		glColor3f(1, 1, 1);
	}
	glTranslatef(translateX,translateY,translateZ);
	glRotatef(90,1,0,0);
	glRotatef(52.5,0,1,0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId[2]);
	gluCylinder(quadratic,0.02f,0.02f,0.2f,32,32);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

//This method draws the trailor hitch for the motercycle.
void DrawTrailerHitch(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
	glPushMatrix();

	glTranslatef(translateX,translateY,translateZ);
	if(isWireFrameOn)
	{
		glColor3f(0.858824,0.576471,0.439216);
		gluQuadricDrawStyle(quadratic, GLU_LINE);
	}
	else
	{
		glColor3f(1,1,1);
		gluQuadricDrawStyle(quadratic, GLU_FILL);
	}

	//Top Horizontal Bar
	glPushMatrix();
	glRotatef(90,0,1,0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId[3]);
	gluCylinder(quadratic,0.018f,0.018f,0.1f,32,32);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Middle Bar
	glPushMatrix();
	glTranslatef(0.09,-0.01,0);
	glRotatef(-90,1,0,0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId[3]);
	gluCylinder(quadratic,0.02f,0.02f,0.18f,32,32);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Bottum
	glPushMatrix();
	glTranslatef(0.09,0.15,0);
	glRotatef(90,0,1,0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId[3]);
	gluCylinder(quadratic,0.02f,0.02f,0.35f,32,32);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPopMatrix();
}

//This method is charged with drawing the trailor for the motercycle.
void DrawTrailer(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
	glPushMatrix();
	glTranslatef(translateX,translateY,translateZ);
	
	if(isWireFrameOn)
	{
		glColor3f(0.70,0.70,0.70);
		glutWireSphere(0.2, 32, 32);
		glPushMatrix();
		glTranslatef(0.09,-0.03,0);
		glRotatef(90,0,1,0);
		glRotatef(21,1,0,0);
		glutWireCone(0.18, 0.4, 32, 32);
		glPopMatrix();
	}
	else
	{
		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureId[5]);  
		gluSphere(quadratic, 0.2, 32, 32);
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		glTranslatef(0.09,-0.03,0);
		glRotatef(90,0,1,0);
		glRotatef(21,1,0,0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureId[5]);
		gluCylinder(quadratic,0.18,0.0,0.4,32,32);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	glPopMatrix();
}

//This method is responsible for drawing the motercycle seat.
void DrawSeat(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
	glPushMatrix();
	glTranslatef(translateX,translateY,translateZ);
	
	if(isWireFrameOn)
	{
		glColor3f(0.42,0.26, 0.15);
		glutWireSphere(0.1, 32, 32);
		glPushMatrix();
		glTranslatef(0.05,-0.0075,0);
		glRotatef(90,0,1,0);
		glRotatef(21,1,0,0);
		glutWireCone(0.08, 0.25, 32, 32);
		glPopMatrix();
	}
	else
	{
		glColor3f(1.0,1.0, 1.0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureId[5]);
		gluSphere(quadratic, 0.1, 32, 32);
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		glTranslatef(0.05,-0.0075,0);
		glRotatef(90,0,1,0);
		glRotatef(21,1,0,0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureId[4]);
		gluCylinder(quadratic, 0.08, 0.0, 0.25, 32, 32);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	glPopMatrix();
}

//This method is responsible for drawing the motercycle seat.
void DrawGasTank(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
	glPushMatrix();
	glTranslatef(translateX,translateY,translateZ);
	if(isWireFrameOn)
	{
		glColor3f(1.0, 0.498039, 0.0);
		glutWireSphere(0.077, 32, 32);
		glPushMatrix();
		glTranslatef(-0.05,-0.01,0);
		glRotatef(-90,0,1,0);
		glRotatef(15,1,0,0);
		glutWireCone(0.06, 0.2, 32, 32);
		glPopMatrix();
	}
	else
	{
		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureId[5]);
		gluSphere(quadratic, 0.077, 32, 32);
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		glTranslatef(-0.05,-0.01,0);
		glRotatef(-90,0,1,0);
		glRotatef(15,1,0,0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureId[5]);
		gluCylinder(quadratic,0.06,0.0,0.2,32,32);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	glPopMatrix();
}

//This method is responsible for drawing the engine.
void DrawEngine(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
	glPushMatrix();
	glTranslatef(translateX,translateY,translateZ);
	glColor3f(0.70, 0.70 , 0.70);
	if(isWireFrameOn)
	{
		glutWireCube(.15);
		glPushMatrix();
		glTranslatef(-0.15,0,0);
		glutWireCube(.15);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-0.25,0,0);
		glutWireCube(.15);
		glPopMatrix();
	}
	else
	{
		glutSolidCube(.15);

		glPushMatrix();
		glTranslatef(-0.15,0,0);
		glutSolidCube(.15);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-0.25,0,0);
		glutSolidCube(.15);
		glPopMatrix();
	}
	glPopMatrix();
}

//This method is responsible for drawing the exhaust pipe.
void DrawExhaustPipe(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
	glPushMatrix();
	if(isWireFrameOn)
	{
		glColor3f(0,0.498039,1.0);
		gluQuadricDrawStyle(quadratic, GLU_LINE);
	}
	else
	{
		glColor3f(1,1,1);
		gluQuadricDrawStyle(quadratic, GLU_FILL);
	}
	glTranslatef(translateX, translateY, translateZ);
	
	glRotatef(-90,0,1,0);


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId[2]);
	gluCylinder(quadratic,0.02f,0.02f,0.4f,32,32);
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	glTranslatef(0,0,0.3);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId[2]);
	gluCylinder(quadratic,0.03f,0.03f,0.45f,32,32);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPopMatrix();
}

//This method is responsible for drawing the front light
void DrawFrontLight(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{	
	glPushMatrix();
	glTranslatef(translateX,translateY,translateZ);
	glColor3f(1, 1, 1);
	if(isWireFrameOn)
	{
		glutWireSphere(0.04, 32, 32);
		gluQuadricDrawStyle(quadratic, GLU_LINE);
		glColor3f(0.752941, 0.752941, 0.752941);
	}
	else
	{
		glutSolidSphere(0.04, 32, 32);
		gluQuadricDrawStyle(quadratic, GLU_FILL);		
	}

	glPushMatrix();
	glTranslatef(0.01,0,0);
	glRotatef(-90,0,1,0);	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId[2]);
	gluCylinder(quadratic,0.045f,0.028f,0.08f,32,32);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();	
	glPopMatrix();
}

//This method is responsible for drawing the back light
void DrawBackLight(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
	glPushMatrix();
	glTranslatef(translateX,translateY,translateZ);
	glColor3f(1, 0, 0);
	if(isWireFrameOn)
	{
		glutWireSphere(0.03, 32, 32);
		gluQuadricDrawStyle(quadratic, GLU_LINE);
		glColor3f(0.752941, 0.752941, 0.752941);
	}
	else
	{
		glutSolidSphere(0.03, 32, 32);
		gluQuadricDrawStyle(quadratic, GLU_FILL);
		glColor3f(1, 1, 1);
	}

	glPushMatrix();
	glTranslatef(0.01,0,0);
	glRotatef(90,0,1,0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId[2]);
	gluCylinder(quadratic,0.035f,0.018f,0.07f,32,32);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();
}

//Draws an infinity sign
void DrawInfinity(float size)
{
   int i; 
   glBegin(GL_LINE_LOOP);
   for(i=0;i<360;i++)
   {
	  float rad = i*DEG2RAD;
	  GLfloat width = 3.0f;
	  glVertex2f((size*sqrt(root)*cos(rad))/((sin(rad)*sin(rad))+1), (trackRadius*sqrt(root)*cos(rad)*sin(rad))/((sin(rad)*sin(rad))+1));

   }
   glEnd();
}

//Draws two elipses in the form of a figure eight race track
void DrawRaceTrack(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
	glPushMatrix();
	glRotatef(90,1,0,0);
	glTranslatef(translateX,translateY,translateZ);
	//Track
	glPushMatrix();
	glColor3f(1.0f,1.0f,1.0f);
	DrawInfinity(trackRadius);
	glPopMatrix();

	//Grass
	glPushMatrix();
	glTranslatef(-15,-10,0);
	glColor3f(0.0f, 0.5f, 0.0f);
	glRectf(0.0f, 4.0f, 30.0f, 16.0f);
	glPopMatrix();


	glPopMatrix();
}

//Moves the motorbike around the track given an acceleration
void MoveMotorbikeForward()
{
	if(isMoving)
	{
		bikeTimer += bikeAcceleration;
		tireRotation += bikeAcceleration*6;
		if(bikeTimer >=12)
		{
			float rad = bikeRotation*DEG2RAD;
			float formerBikeX = (trackRadius*sqrt(root)*cos(rad))/((sin(rad)*sin(rad))+1);
			float formerBikeZ = (trackRadius*sqrt(root)*cos(rad)*sin(rad))/((sin(rad)*sin(rad))+1);
		
			bikeRotation++;
			rad = bikeRotation*DEG2RAD;
			bikeX = (trackRadius*sqrt(root)*cos(rad))/((sin(rad)*sin(rad))+1);
			bikeZ = (trackRadius*sqrt(root)*cos(rad)*sin(rad))/((sin(rad)*sin(rad))+1);

			float opp = formerBikeX - bikeX;
			float adj =formerBikeZ - bikeZ;
			cameraDirection = (atan2(opp,adj)*RAD2DEG);
			bikeDirection = (atan2(opp,adj)*RAD2DEG)+90;
			bikeTimer=0;
		}
		if(bikeRotation >= 360)
		{
			bikeRotation = 0;
		}
		if(tireRotation >= 360)
		{
			tireRotation = 0;
		}
		glutPostRedisplay();
	}
}

//This method is responsible for incrementing the bike acceleration
void AccelerateBike()
{
	if(bikeAcceleration < 5)
	{
		bikeAcceleration++;
	}
}

//This method is responsible for decrementing the bike acceleration
void DeccelerateBike()
{
	if(bikeAcceleration > 1)
	{
		bikeAcceleration--;
	}
}

//Draws a motercycle.
void DrawMotercycle(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
	glPushMatrix();	
	glTranslatef(translateX,translateY,translateZ);
	glRotatef(bikeDirection, 0,1,0);
	EmitLight1(0.85, 0.3, 0, 2.75, 0, 0);
	DrawHandleCrossBar(1.10, 0.10, -0.10);	 	// Cross bar for front wheel
	DrawMotercycleTire(1.10, 0.10, 0.00);		// Front wheel
	DrawHandleCrossBar(0.10, 0.10, -0.10);		// Cross bar for back wheel
	DrawMotercycleTire(0.10, 0.10, 0.00);		// Back wheel     
	DrawHandleBarShaft(1.10, 0.10, -0.10);		// Left Handle Shaft
	DrawHandleBarShaft(1.10, 0.10, 0.10);		// Right Handle Shaft
	DrawHandleBars(0.75, 0.36, -0.10);			// Handlebars
	DrawEngineSupport(0.9, 0.24, -0.10);		// Left engine support
	DrawEngineSupport(0.9, 0.24, 0.10);			// Right engine support
	DrawGasTank(0.75, 0.26, 0);					// Gas Tank	
	DrawFrontLight(0.85, 0.3, 0);				// Front Light
	DrawExhaustPipe(0.7, 0.10, 0.10);			// Exhaust Pipe
	DrawEngine(0.7, 0.13,0);					// Engine
	DrawSeat(0.22, 0.30, 0);					// Seat
	DrawBackLight(0.13, 0.30, -0.06);			// Back Light Left
	DrawBackLight(0.13, 0.30, 0.06);			// Back Light Right
	DrawEngineSupport(0.25, 0.22, -0.10);		// Left engine support
	DrawOtherEngineSupport(0.23, 0.22, -0.10); 
	DrawEngineSupport(0.25, 0.22, 0.10);		// Right engine support
	DrawOtherEngineSupport(0.23, 0.22, 0.10);
	DrawTrailerHitch(-0.15, 0.15, 0);			// Trailer Hitch
	DrawTrailer(-0.58, 0.30, 0);				// Trailer chasey
	DrawMotercycleTire(-0.6, 0.10, -0.16);		// Left trailer wheel
	DrawMotercycleTire(-0.6, 0.10, 0.16);		// Right trailer Back wheel	
	glPopMatrix();
}

//Prints text to screen using bitmap character
void text(char* string)
{
	char* p;

	for (p = string; *p; p++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
}

//Prints numbers to screen using bitmap character
void numberedText(float number)
{
	string data;
	stringstream ss;
	ss<<number;
	data = ss.str();
	char* arrayChar = (char*)data.c_str();
	char* p;
	for (p = arrayChar; *p; p++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
}

//This method calculates frames per second
void CalcFramesPerSecond()
{
	double end = clock();
	if(end-timePrevious > 0)
	{
		fps = 1/((end-timePrevious)/CLOCKS_PER_SEC);
		timePrevious = clock();
	}
}

// This function is called when the viewing perspective is changed or the 
// viewing window is resized.
void reshape (int w, int h)
{
	 glClearColor(0.0, 0.0, 0.0, 0.0);
}

// This method runs while the program is idle.
void idleFunction()
{
	MoveMotorbikeForward();
	CalcFramesPerSecond();
}

//This method is responsible for drawing objects.
void display(void)
{		
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	glViewport(0, (height/12)*11, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_LIGHTING);
	glColor3ub(255, 255, 255);
	glBegin(GL_LINES);
	glVertex2i(0,0);
	glVertex2i(width,0);
	glEnd();

	glRasterPos2i(width/3, 10);
	text("Frames per second: ");
	numberedText(fps);


	glViewport(0, 0, width, (height / 8)*7);
	glMatrixMode (GL_PROJECTION);								// Set the Matrix mode
	glLoadIdentity ();

	if(isFirstPerson)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt (bikeX-(1.5*sin(cameraDirection*DEG2RAD)), bikeY, bikeZ-(1.5*cos(cameraDirection*DEG2RAD)), bikeX+(10*sin(cameraDirection*DEG2RAD)), bikeY, bikeZ+(10*cos(cameraDirection*DEG2RAD)), 0.0, 1.0, 0.0); 
	}
	else{
		if(isOrthographicViewOn)
		{
			//Convert units for orthographic projection
			nearH = near * tan(feildOfView * DEG2RAD);
			nearW = nearH * ((GLfloat) width /(GLfloat) height);
			glOrtho(-nearW, nearW, -nearH, nearH, near, far);
		}
		else
		{
			gluPerspective(feildOfView, (GLfloat) width /(GLfloat) height , near, far);
		}

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt (cx, cy, cz, bikeX + dx, bikeY + dy, bikeZ + dz, 0.0, 1.0, 0.0);		
	}

	EmitLight3(cx, cy, cz, bikeX, bikeY, bikeZ);
	glPushMatrix();										// It is important to push the Matrix before calling glRotatef and glTranslatef
	glRotatef(rotX,1.0,0.0,0.0);						// Rotate on x
	glRotatef(rotY,0.0,1.0,0.0);						// Rotate on y
	glRotatef(rotZ,0.0,0.0,1.0);						// Rotate on z
	glTranslatef(X, Y, Z);								// Translates the screen left or right, up or down or zoom in zoom out
	

	if(isLight2Moving)
	{
		light2X = bikeX;
		light2Y = bikeY;
		light2Z = bikeZ;
	}
	
	EmitLight2(light2X, light2Y+2, light2Z, light2X, light2Y, light2Z);	
	EmitLight4(bikeX, bikeY+2, bikeZ, bikeX, bikeY, bikeZ);	
	DrawMotercycle(bikeX, bikeY, bikeZ);	
	DrawRaceTrack(0, 0.0, 0.0);
	

	glPopMatrix();
	
	glFlush();
	glutSwapBuffers();
	CalcFramesPerSecond();
}

//This method is called when a key is pressed and is responsible for initiating 
//some camera motion and changes in view
void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		isFirstPerson=true;
		isLightOn = false;
		isLight1On = false;
		isLight2On = false;
		isLight3On = false;
		break;
	case '2':
		if(isLight2Moving)
		{
			isLight2Moving = false;
		}
		else
		{
			isLight2Moving = true;
		}
		break;
	case '3':
		isFirstPerson=false;
		break;
	case 'a':
		AccelerateBike();
		break;
	case 'A':
		DeccelerateBike();
		break;

	// b,B uses the gluLookAt function to move backword allong the line connecting 
	//the motercycle and the center of projection
	case 'b':
	case 'B':
		if(!isFirstPerson)
		{
			cx -= dx*0.1f;
			cz -= dz*0.1f;
			glLoadIdentity();
			gluLookAt (cx, cy, cz, cx+ dx, cy + dy, cz + dz, 0.0, 1.0, 0.0);
		}
		break;

	// Default, clears viewing parameters and returns to the intitial view.
	case 'c':				
	case 'C':
		isWireFrameOn = false;			//Indicates if the objects are displayed as wire 		
		isOrthographicViewOn = false;  //Indicates if the view is orthographic
		isMoving = false;
		isFirstPerson = false;
		isLightOn = false;
		isLight1On = false;
		isLight2On = false;
		isLight2Moving = true;
		isLight3On = false;
		isHighBeamOn = false;
		isRustOn = false;
		feildOfView = 50;					//Initiates the field of view for perspective
		nearH = 0.0;						//Height of near plane
		nearW = 0.0;						//Width of near plane
		near = 0.10;						//Distance to near plane
		far = 100.0;						//Distance to far plane			
		X = Y = Z = 0.0f;					// Translate screen to x,y,z directions
		bikeX = 14.0f;		
		bikeY = 0.0f;		
		bikeZ = 0.0f;
		bikeRotation = 0;
		tireRotation = 0;
		bikeDirection = -90;
		cameraDirection = 0;
		bikeTimer = 0;
		bikeAcceleration = 1;
		rotX = 0.0f;	// Rotate screen on x axis 
		rotY = 0.0f;	// Rotate screen on y axis
		rotZ = 0.0f;	// Rotate screen on z axis
		cx = 0.0f;   // Camera x
		cy = 0.0f;   // Camera y
		cz = 5.0f;   // Camera z
		dx = 0.0f;   // Line of sight vector x
		dy = 0.0f;   // Line of sight vector y
		dz = -1.0f;  // Line of sight vector z
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		break;

	case 'e':
	case 'E':
		bikeAcceleration =1;
		isMoving=false;
		break;

	// f,F uses the gluLookAt function to move forward allong the line connecting 
	//the motercycle and the center of projection
	case 'f':
	case 'F':	
		if(!isFirstPerson)
		{
			cx += dx*0.1f;
			cz += dz*0.1f;
			glLoadIdentity();
			gluLookAt (cx, cy, cz, cx + dx, cy + dy, cz + dz, 0.0, 1.0, 0.0);
		}
		break;
	case 'h' :
		if(isLight1On)
		{
			isHighBeamOn = false;
		}
		break;
	case 'H' :
		if(isLight1On)
		{
			isHighBeamOn = true;			
		}
		break;
	case 'i' :    // Rotates screen on y axis 
	case 'I' : 
		if(!isFirstPerson)
		{
		rotX -= 0.5f;
		}
		break;
	case 'j' :    // Rotates screen on y axis 
	case 'J' :
		if(!isFirstPerson)
		{
			rotY -= 0.5f;
		}
		break;
	case 'k' :	// Opposite
		if(!isFirstPerson)
		{
			rotY += 0.5f;
		}
		break;
	case 'K' :
		if(!isFirstPerson)
		{
			rotX += 0.5f;
		}
		break;
	case 'l' :	// Opposite
		isLightOn = true;
		break;
	case 'L' :
		isLightOn = false;
		isLight1On = false;
		isLight2On = false;
		isLight3On = false;
		break;
	// o,O,p,P uses a boolean to indicate to the reshape method, whether it should view orthographically
	// or perspectivelly respectively;
	case 'o':
	case 'O':
		if(!isFirstPerson)
		{
			isOrthographicViewOn = true;
		}
		feildOfView = 88.0;
		break;
	case'p':
	case'P':
		if(!isFirstPerson)
		{
		isOrthographicViewOn = false;
		feildOfView = 50.0;
		}
		break;
	case'S':
	case's':
		isMoving = true;
		break;
	// w,W uses a boolean to indicate to other methods whether they should draw
	// objects with a solid or wire frame
	case 'w':
	case'W':
		if(isWireFrameOn)
		{
			isWireFrameOn = false;
		}
		else
		{
			isWireFrameOn = true;
		}
		break;
	
	// z, Z is used to zoom into and out of the world respectively.
	case 'z':				// Zoom-in
		if(!isFirstPerson)
		{
			feildOfView -= 1;
		}
		break;
	case 'Z':				// Zoom-out
		if(!isFirstPerson)
		{
			feildOfView += 1;
		}
		break;

	case 27:				// Gracefull Exit	
		exit(0);				
	}

	// Redraw the scene
	glutPostRedisplay();	
}

//This method is called when a special key is pressed and is responsible for initiating 
//camera rotation about the x and y axis using the the glRotatef() function.
void specialKey(int key, int x, int y) { 

	// Check which key is pressed
	switch(key) {
	case GLUT_KEY_LEFT :    // Rotates screen on y axis 
		if(!isFirstPerson)
		{
			rotY -= 0.5f;
		}
		break;
	case GLUT_KEY_RIGHT :	// Opposite
		if(!isFirstPerson)
		{
			rotY += 0.5f;
		}
		break;
	case GLUT_KEY_UP :		// Rotates screen on x axis 
		if(!isFirstPerson)
		{
			rotX -= 0.5f;
		}
		break;
	case GLUT_KEY_DOWN :	// Opposite
		if(!isFirstPerson)
		{
			rotX += 0.5f;
		}
		break;
	case GLUT_KEY_F1 :
		if(isLight1On)
		{
			isLight1On = false;
		}
		else
		{
			isLight1On = true;
			isLight4On = false;
		}
		break;
	case GLUT_KEY_F2 :
		if(isLight2On)
		{
			isLight2On = false;
		}
		else
		{
			isLight2On = true;
			isLight4On = false;
		}
		break;
	case GLUT_KEY_F3 :
		if(isLight3On)
		{
			isLight3On = false;
		}
		else
		{
			isLight3On = true;
			isLight4On = false;
		}
		break;
	case GLUT_KEY_F4 :
		if(isRustOn)
		{
			isRustOn = false;
		}
		else
		{
			isRustOn = true;
		}
		break;
	case GLUT_KEY_F5 :
		if(isLight4On)
		{
			isLight4On = false;
		}
		else
		{
			isLight4On = true;
			isLight3On = false;
			isLight2On = false;
			isLight1On = false;
		}
		break;
	}

	// Redraw the scene
	glutPostRedisplay();		
}

// Entry point of the program
// and location of the main loop
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);		// Setup display mode to double buffer and RGB color
	glutInitWindowSize (600,600);						// Set the screen size
	glutCreateWindow("9106251 COMP371 A3");	
	init ();
	glutReshapeFunc(reshape);
	glutIdleFunc(idleFunction);
	glutDisplayFunc(display);	
	glutKeyboardFunc(keyboard);		// set window's key callback 
	glutSpecialFunc(specialKey);	// set window's to specialKey callback
	glutMainLoop();
	return 0;
}