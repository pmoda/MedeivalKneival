#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include <strstream>
#include <iomanip>
#include "imageloader.h"
#include "vec3f.h"
#include <vector>

#define PI 3.14159265

GLfloat X = 0.0f;		// Translate screen to x direction (left or right)
GLfloat Y = 0.0f;		// Translate screen to y direction (up or down)
GLfloat Z = 0.0f;		// Translate screen to z direction (zoom in or out)
GLdouble rotX = 0.0f;	// Rotate screen on x axis 
GLdouble rotY = 0.0f;	// Rotate screen on y axis
GLdouble rotZ = 0.0f;	// Rotate screen on z axis

static float cx = 0.0f;   // Camera x
static float cy = 0.0f;   // Camera y
static float cz = 20.0f;   // Camera z

static float dx = 0.0f;   // Line of sight vector  x
static float dy = 0.0f;   // Line of sight vector y
static float dz = -1.0f;   // Line of sight vector z

static float angle = 0.0f;
static int wheelRotation = 0.0f;
static int wheelSpeed = 1.0f;
static float turnCounter = 5;

static float motoX = 0.0f;
static float motoY = -0.5f;
static float motoZ = 30.0f;

static float camDist = 50.0f;

// handle bars
static float handleBarLength = 0.15f;
GLUquadric *quadobj = gluNewQuadric();

static double fovy = 90;
static bool wire = false;
bool ortho = false;
bool moveable = true;
float stoppedSpeed = 0.0f;
bool thirdPersonView = true;
bool rotatedBy180 = false;
float motoRot = 0.0f;
float viewAngle = 0.0f;

//	FPS message
time_t start, end;
double FPS, elapsed, framecount;
int fpsViewport = 30;
char msg[30];

// Light fixtures;
bool spotLightOn = true;
bool directionalLightOn = false;
bool highBeamOn = true;
bool movingLightOn = true;
bool freezeMovingLight = true;
bool eyeOfSauron = false;
float movingLightAngle = 0.0f;

//void glDisplayLines(void);

GLuint _textureId;
GLuint _textureId2;
GLuint _textureId3;
GLuint _textureId4;

GLuint loadTexture(Image* image) 
{
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		image->width, image->height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		image->pixels);
	return textureId;
}

void text(char* str){
    char* p;
    for (p = str; *p; p++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *p);
}

void drawGrass()
{
	glColor3f(0.0f,0.5f,0.0f);
	glPushMatrix();
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTranslatef(0, -2.6f, 0.0f);
	glScalef(100, 1.0f, 200);
	glutSolidCube(1.0);
	
	//for(int i=-50;i<50;i=i++)
	//{
	//	glBegin(GL_TRIANGLE_STRIP);
	//		glNormal3f(0,1,0);
	//		for(int j=-100;j<100;j++)
	//		{
	//			glVertex3f(i,-2,j);
	//			glVertex3f(i+1,-2,j);
	//			glVertex3f(i,-2,j+1);
	//			glVertex3f(i+1,-2,j+1);
	//		}
	//	glEnd();
	//}
	glPopMatrix();
}

void drawTrack(int left, int right, float radius)
{	
	float curveAngle;
	float xCurve;	
	float yCurve = -2; // Constant
	float zCurve;	

	//	Track Width
	float outerRadius = radius;
	float innerRadius = radius/2;
	float radiusDiff = outerRadius - innerRadius;
	int pieces = 64;
	int arc = 180;	
	
	
	if (!wire)
	{
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);		
	}
	else
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
	}	
	
	glPushMatrix();
		glColor3f(1.0,1.0,1.0);
	
		//	Left Curve
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, left);
			glRotatef(180, 0, 1, 0);
			float counter = 0;
			float inner = innerRadius;
			float outer = 0.0f;
			while (counter < radiusDiff)
			{				
				inner = innerRadius + counter;
				outer = inner + 1.0f;
				glBegin(GL_TRIANGLE_STRIP);
				for (int i=0; i<=pieces; i++) 
				{					
					int j = i + 1;
					float curveAngle1 = (i*arc*PI/180)/pieces;
					float curveAngle2 = (j*arc*PI/180)/pieces;

										
					glNormal3f(0,1,0);					
					xCurve = inner*cos(curveAngle1);
					zCurve = inner*sin(curveAngle1);		
					glVertex3f(xCurve, yCurve,zCurve);
					
					xCurve = outer*cos(curveAngle1);
					zCurve = outer*sin(curveAngle1);		
					glVertex3f(xCurve, yCurve,zCurve);					
					
					xCurve = inner*cos(curveAngle2);
					zCurve = inner*sin(curveAngle2);		
					glVertex3f(xCurve, yCurve,zCurve);

					xCurve = outer*cos(curveAngle2);
					zCurve = outer*sin(curveAngle2);		
					glVertex3f(xCurve, yCurve,zCurve);					
				}
				glEnd();
				counter++;
			}

			//glBegin(GL_TRIANGLE_STRIP);
			//for (int i=0; i<=pieces; i++) 
			//{		
			//	glNormal3f(0,1,0);
			//	curveAngle = (i*arc*PI/180)/pieces;
			//	xCurve = outerRadius*cos(curveAngle);
			//	zCurve = outerRadius*sin(curveAngle);		
			//	glVertex3f(xCurve, yCurve,zCurve);

			//	xCurve = innerRadius*cos(curveAngle);
			//	zCurve = innerRadius*sin(curveAngle);		
			//	glVertex3f(xCurve, yCurve,zCurve);
			//}
			//glEnd();
		glPopMatrix();
	
		//	Bottom Track
		for(int i=left;i<left + innerRadius;i++){
			glBegin(GL_TRIANGLE_STRIP);
			glNormal3f(0,1,0);
			for(int j=left;j<right;j++)
			{			
				glVertex3f(i,yCurve,j);
				glVertex3f(i+1,yCurve,j);
				glVertex3f(i,yCurve,j+1);
				glVertex3f(i+1,yCurve,j+1);		
			}
			glEnd();			
		}	

		// Top track
		for(int i=right-innerRadius;i<right;i++){
			glBegin(GL_TRIANGLE_STRIP);
			glNormal3f(0,1,0);
				for(int j=left;j<right;j++)
				{		
					glVertex3f(i,yCurve,j);
					glVertex3f(i+1,yCurve,j);
					glVertex3f(i,yCurve,j+1);
					glVertex3f(i+1,yCurve,j+1);			
				}
			glEnd();
			
		}

		// Right Curve
		glPushMatrix();		
			glTranslatef(0.0f, 0.0f, right);	
			counter = 0;
			inner = innerRadius;
			outer = 0.0f;
			while (counter < radiusDiff)
			{				
				inner = innerRadius + counter;
				outer = inner + 1.0f;
				glBegin(GL_TRIANGLE_STRIP);
				for (int i=0; i<=pieces; i++) 
				{					
					int j = i + 1;
					float curveAngle1 = (i*arc*PI/180)/pieces;
					float curveAngle2 = (j*arc*PI/180)/pieces;

										
					glNormal3f(0,1,0);					
					xCurve = inner*cos(curveAngle1);
					zCurve = inner*sin(curveAngle1);		
					glVertex3f(xCurve, yCurve,zCurve);
					
					xCurve = outer*cos(curveAngle1);
					zCurve = outer*sin(curveAngle1);		
					glVertex3f(xCurve, yCurve,zCurve);					
					
					xCurve = inner*cos(curveAngle2);
					zCurve = inner*sin(curveAngle2);		
					glVertex3f(xCurve, yCurve,zCurve);

					xCurve = outer*cos(curveAngle2);
					zCurve = outer*sin(curveAngle2);		
					glVertex3f(xCurve, yCurve,zCurve);					
				}
				glEnd();
				counter++;
				/*glNormal3f(0,1,0);
				curveAngle = (i*arc*PI/180)/pieces;
				xCurve = outerRadius*cos(curveAngle);
				zCurve = outerRadius*sin(curveAngle);		
				glVertex3f(xCurve, yCurve,zCurve);

				xCurve = innerRadius*cos(curveAngle);
				zCurve = innerRadius*sin(curveAngle);		
				glVertex3f(xCurve, yCurve,zCurve);

				curveAngle = ((i+1)*arc*PI/180)/pieces;				
				xCurve = innerRadius*cos(curveAngle);
				zCurve = innerRadius*sin(curveAngle);		
				glVertex3f(xCurve, yCurve,zCurve);*/

			}
			
		glPopMatrix();
	glPopMatrix();
}

float handleBarYPos(void)
{
	float yCoord = 0.85f;
	float handleBarAngleA = 45.0f;
	float height = handleBarLength * sin(handleBarAngleA * 3.1415926 / 180 ); // sin only takes radians, not degrees
	float yPos = height + yCoord;	 
	return yPos;
}

float handleBarXPos(void)
{
	float xCoord = 0.85f;
	float handleBarAngleB = 90.0f - 45.0f;	// subtract handleBarAngleA;
	float bottom = handleBarLength * sin(handleBarAngleB * 3.1415926 / 180); // sin only takes radians, not degrees
	float xPos = xCoord - bottom; // subtract the length of the bottom from the x coordinate desired
	return xPos;
}

void drawSteering()
{
	float handleBarX = handleBarXPos();
	float handleBarY = handleBarYPos();

	// Bar 1 that connects handle bar to shaft
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);
	glTranslatef(0.85, 0.85, -0.35);	// Take X and Y coord of handlebars in handleBarXPos() and handleBarYPos()
	gluCylinder(quadobj, 0.02, 0.02, 0.35, 25, 25);
	glPopMatrix();

	// Bar 2 that connects handle bar to shaft
	glPushMatrix();	
	glColor3f(1.0, 1.0, 0.0);
	glTranslatef(0.85, 0.85, 0.0); // Take X and Y coord of handlebars in handleBarXPos() and handleBarYPos()
	gluCylinder(quadobj, 0.02, 0.02, 0.35, 25, 25);
	glPopMatrix();

	// Sphere that connects bars to wheels
	glPushMatrix();
	glColor3f(1.0, 0.0, 1.0);
	glTranslatef(0.85, 0.85, 0.0);
	glScalef(0.1, 0.15, 0.2);
	if(wire)
	{
		glutWireSphere(0.5, 15, 15);
	}
	else
	{
		glutSolidSphere(0.5, 15, 15);
	}
	glPopMatrix();	

	// Handle bar 1
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);	
	glTranslatef(handleBarX, handleBarY, -0.35); // z is based off Bar 1 cylinder height
	glRotatef(90.0f, 0.0, 1.0, 0.0);
	glRotatef(45.0f, 1.0, 0.0, 0.0); // Take handleBarAngleA

	glEnable(GL_TEXTURE_2D);		
	glBindTexture(GL_TEXTURE_2D, _textureId2);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glEnable(GL_TEXTURE_GEN_T);

	gluCylinder(quadobj, 0.01, 0.01, handleBarLength, 25, 25);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
	// Handle bar 2
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);	
	glTranslatef(handleBarX, handleBarY, 0.35); // z is based off Bar 2 cylinder height
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(45.0f, 1.0, 0.0, 0.0);	// Take handleBarAngleA
	
	glEnable(GL_TEXTURE_2D);		
	glBindTexture(GL_TEXTURE_2D, _textureId2);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glEnable(GL_TEXTURE_GEN_T);
	
	gluCylinder(quadobj, 0.01, 0.01, handleBarLength, 25, 25);
	
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// Connecting Sphere from large cylinder to steering shaft
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(0.83f, 0.625f, 0.27f);
	if (wire)
	{
		glutWireSphere(0.075, 25, 25); 
	}
	else
	{
		glutSolidSphere(0.075, 25, 25); 
	}
	glPopMatrix();

	// Connecting Sphere from large cylinder to steering shaft
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(0.83f, 0.625f, -0.27f);
	if (wire)
	{
		glutWireSphere(0.075, 25, 25); 
	}
	else
	{
		glutSolidSphere(0.075, 25, 25); 
	}
	glPopMatrix();

	// Light
	// cylinder to hold the circle light in


	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(1.1, 0.6, 0.0);
	glScalef(0.05, 0.15, 0.2);
	if (wire)
	{
		glutWireCube(1.0);
	}
	else
	{
		glutSolidCube(1.0);
	}
	glPopMatrix();

	
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(0.85f, 0.6f, 0.0f);
	glScalef(1.0f, 1.0f, 1.2f);
	if (wire)
	{
		glutWireSphere(0.25, 30, 30);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);		
		glBindTexture(GL_TEXTURE_2D, _textureId2);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
		glEnable(GL_TEXTURE_GEN_T);
		glutSolidSphere(0.25, 30, 30);
		glDisable(GL_TEXTURE_2D);	
	}
	glPopMatrix();
}

void drawSeat()
{
	glPushMatrix();
	glColor3b(90, 90, 90);
	glTranslatef(0.0f, 0.7f, 0.0);
	glRotatef(120, 0.0f, 0.0f, 1.0f);
	glRotatef(90, 0.0f, 1.0f, 0.0f);
	if (wire)
	{
		glutWireTorus(0.075, 0.075, 25, 25);
	}
	else
	{
		glutSolidTorus(0.075, 0.075, 25, 25);
	}
	glPopMatrix();	

	glPushMatrix();
	glColor3b(90, 90, 90);
	glTranslatef(-0.2f, 0.7f, 0.0);
	glRotatef(60, 0.0f, 0.0f, 1.0f);
	glRotatef(90, 0.0f, 1.0f, 0.0f);
	if (wire)
	{
		glutWireTorus(0.075, 0.075, 25, 25);
	}
	else
	{
		glutSolidTorus(0.075, 0.075, 25, 25);
	}
	glPopMatrix();	

}

void drawBody()
{
	// Main Body of Moto
	glPushMatrix();	
	glColor3f(1.0f,0.0f,0.0f);
	glTranslatef(0.0f, 0.5f, 0.0f);
	glScalef(1.0f, 0.25f, 0.3f);
	if (wire)
	{
		glutWireSphere(1.0, 30, 30);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);		
		glBindTexture(GL_TEXTURE_2D, _textureId2);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
		glEnable(GL_TEXTURE_GEN_T);
		glutSolidSphere(1.0, 30, 30);
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();

	//Backlight
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(-1.0f, 0.55f, 0.0f);
	glScalef(0.05, 0.08, 0.2);
	if (wire)
	{
		glutWireCube(1.0);
	}
	else
	{
		glutSolidCube(1.0);
	}
	glPopMatrix();

}

void drawGasTank()
{
	glPushMatrix();
	glColor3b(80, 80, 80);
	glTranslatef(0.55f, 0.65f, 0.0f);
	/*glScalef(0*/
	if(wire)
	{
		glutWireCube(0.25);
	}
	else
	{
		glutSolidCube(0.25);
	}
	glPopMatrix();


	// Connect Engine to Gas Tank
	glPushMatrix();
	glColor3b(90, 90, 90);
	glTranslatef(0.3f, .4f, 0.0f);
	glRotatef(90, 0.0, 1.0, 0.0);
	glEnable(GL_TEXTURE_2D);		
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glEnable(GL_TEXTURE_GEN_T);
	gluCylinder(quadobj, 0.05, 0.05, 0.2, 25, 25);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// Sphere to connect the Cylinders
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.5f, 0.4f, 0.0f);
	if (wire)
	{
		glutWireSphere(0.05, 25, 25); 
	}
	else
	{
		glutSolidSphere(0.05, 25, 25); 
	}
	glPopMatrix();

	// Connecting Gas tank to engine
	glPushMatrix();
	glColor3b(90, 90, 90);
	glTranslatef(0.5f, .4f, 0.0f);
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	gluCylinder(quadobj, 0.05, 0.05, 0.125, 25, 25);
	glPopMatrix();

}

void drawEngine()
{	
	// Tank
	glPushMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.15f, 0.35f, 0.0f);	
	if(wire)
	{
		glutWireCube(0.3);
	}
	else
	{
		glutSolidCube(0.3);
	}
	glPopMatrix();

	// Left Bottom Cylinder
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(-1.05f, 0.4f, -0.05f);
	glRotatef(90, 0.0, 1.0, 0.0);

	glEnable(GL_TEXTURE_2D);		
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glEnable(GL_TEXTURE_GEN_T);

	gluCylinder(quadobj, 0.05, 0.05, 1.05, 25, 25);

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	// Right Bottom Cylinder
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(-1.05f, 0.4f, 0.05f);
	glRotatef(90, 0.0, 1.0, 0.0);

	glEnable(GL_TEXTURE_2D);		
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glEnable(GL_TEXTURE_GEN_T);

	gluCylinder(quadobj, 0.05, 0.05, 1.05, 25, 25);	
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();




	//// Connecting Sphere 1
	//glPushMatrix();
	//glColor3f(1.0f, 1.0f, 1.0f);
	//glTranslatef(0.0f, 0.4f, 0.05f);
	//if (wire)
	//{
	//	glutWireSphere(0.05, 25, 25); 
	//}
	//else
	//{
	//	glutSolidSphere(0.05, 25, 25); 
	//}
	//glPopMatrix();

	//// Connecting Sphere 2
	//glPushMatrix();
	//glColor3f(1.0f, 1.0f, 1.0f);
	//glTranslatef(0.0f, 0.4f, -0.05f);
	//if (wire)
	//{
	//	glutWireSphere(0.05, 25, 25); 
	//}
	//else
	//{
	//	glutSolidSphere(0.05, 25, 25); 
	//}
	//glPopMatrix();

	//// Left Angled Cylinder 
	//glPushMatrix();
	//glColor3f(1.0f, 1.0f, 1.0f);
	//glTranslatef(0.0f, 0.4f, -0.05f);
	////glRotatef(25, 0.0, 0.0, 1.0);
	//glRotatef(90, 0.0, 1.0, 0.0);
	//gluCylinder(quadobj, 0.05, 0.05, 0.4, 25, 25);
	//glPopMatrix();
	//
	//// Right Angled Cylinder
	//glPushMatrix();
	//glColor3f(1.0f, 1.0f, 1.0f);
	//glTranslatef(0.0f, 0.4f, 0.05f);
	////glRotatef(25, 0.0, 0.0, 1.0);
	//glRotatef(90, 0.0, 1.0, 0.0);
	//gluCylinder(quadobj, 0.05, 0.05, 0.4, 25, 25);
	//glPopMatrix();


}

void drawBackWheel()
{
	// Right Small Cylinder attached to connecting sphere
	glPushMatrix();
	glColor3b(120,120,120);
	glTranslatef(-0.7f, 0.0f, 0.075f);
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	gluCylinder(quadobj, 0.02, 0.02, 0.17, 25, 25);
	glPopMatrix();

	// Left Small Cylinder attached to connecting sphere
	glPushMatrix();
	glColor3b(120,120,120);
	glTranslatef(-0.7f, 0.0f, -0.245f);
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	gluCylinder(quadobj, 0.02, 0.02, 0.17, 25, 25);
	glPopMatrix();

	// Sphere 1 to connect Cylinder 1 - Wheel 1 to steering shaft
	glPushMatrix();
	glColor3b(120,120,120);
	glTranslatef(-0.7f, 0.0f, 0.255f);
	if (wire)
	{
		glutWireSphere(0.02, 25, 25);
	}
	else
	{
		glutSolidSphere(0.02, 25, 25);
	}
	glPopMatrix();

	// Sphere 2 to connect Cylinder 2 - Wheel 1 to steering shaft
	glPushMatrix();
	glColor3b(120,120,120);
	glTranslatef(-0.7f, 0.0f, -0.255f);
	if (wire)
	{
		glutWireSphere(0.02, 25, 25);
	}
	else
	{
		glutSolidSphere(0.02, 25, 25);
	}
	glPopMatrix();

	// Right Large Cylinder attached to connecting sphere
	glPushMatrix();
	glColor3b(120,120,120);
	glTranslatef(-0.7f, 0.0f, 0.255f);
	glRotatef(55, 0.0f, 0.0f, 1.0f);
	glRotatef(90, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadobj, 0.02, 0.02, 0.45, 25, 25);
	glPopMatrix();
	
	// Left Large Cylinder attached to connecting sphere
	glPushMatrix();
	glColor3b(120,120,120);
	glTranslatef(-0.7f, 0.0f, -0.255f);
	glRotatef(55, 0.0f, 0.0f, 1.0f);
	glRotatef(90, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadobj, 0.02, 0.02, 0.45, 25, 25);
	glPopMatrix();

	// Sphere to connect right cylinder to body
	glPushMatrix();
	glColor3b(120,120,120);
	glTranslatef(-0.41f, 0.4f, 0.255f);
	if (wire)
	{
		glutWireSphere(0.07, 25, 25);
	}
	else
	{
		glutSolidSphere(0.07, 25, 25);
	}
	glPopMatrix();

	// Sphere to connect left cylinder to body
	glPushMatrix();
	glColor3b(120,120,120);
	glTranslatef(-0.41f, 0.4f, -0.255f);
	if (wire)
	{
		glutWireSphere(0.07, 25, 25);
	}
	else
	{
		glutSolidSphere(0.07, 25, 25);
	}
	glPopMatrix();

	// Back Wheel
	glPushMatrix();
	glColor3f(0.3, 0.3, 0.3);
	glTranslatef(-0.7, 0.0, 0.0);
	glRotatef(wheelRotation, 0.0f, 0.0f, 1.0f);
	glScalef(1.0, 1.0, 2.0); 
	if (wire)
	{
		glutWireTorus(0.1, 0.2, 25, 25);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureId4);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
		glEnable(GL_TEXTURE_GEN_T);
		glutSolidTorus(0.1, 0.2, 25, 25);
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();	

	// Rim #1 for Back Wheel
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(-0.7f, 0.0f, 0.075f);
	gluDisk(quadobj, 0, 0.1, 25, 25);
	glPopMatrix();

	// Rim #2 for Back Wheel
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(-0.7f, 0.0f, -0.075f);
	gluDisk(quadobj, 0, 0.1, 25, 25);
	glPopMatrix();	
	
}

void drawFrontWheel()
{	
	// Right Small Cylinder attached to connecting sphere
	glPushMatrix();
	glColor3b(120,120,120);
	glTranslatef(1.0, 0.0, 0.075);
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	gluCylinder(quadobj, 0.02, 0.02, 0.2, 25, 25);
	glPopMatrix();

	// Left Small Cylinder attached to connecting sphere
	glPushMatrix();
	glColor3b(120,120,120);
	glTranslatef(1.0, 0.0, -0.275);
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	gluCylinder(quadobj, 0.02, 0.02, 0.2, 25, 25);
	glPopMatrix();

	// Right Large Cylinder attached to connecting sphere
	glPushMatrix();
	glColor3b(120,120,120);
	glTranslatef(1.0, 0.0, 0.275);
	glRotatef(105, 0.0f, 0.0f, 1.0f);
	glRotatef(90, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadobj, 0.02, 0.04, 0.65, 25, 25);
	glPopMatrix();
	
	// Left Large Cylinder attached to connecting sphere
	glPushMatrix();
	glColor3b(120,120,120);
	glTranslatef(1.0, 0.0, -0.275);
	glRotatef(105, 0.0f, 0.0f, 1.0f);
	glRotatef(90, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadobj, 0.02, 0.04, 0.65, 25, 25);
	glPopMatrix();

	// Sphere 1 to connect Cylinder 1 - Wheel 1 to steering shaft
	glPushMatrix();
	glColor3b(120,120,120);
	glTranslatef(1.0, 0.0, 0.275);
	if (wire)
	{
		glutWireSphere(0.02, 25, 25);
	}
	else
	{
		glutSolidSphere(0.02, 25, 25);
	}
	glPopMatrix();

	// Sphere 2 to connect Cylinder 2 - Wheel 1 to steering shaft
	glPushMatrix();
	glColor3b(120,120,120);
	glTranslatef(1.0, 0.0, -0.275);
	if (wire)
	{
		glutWireSphere(0.02, 25, 25);
	}
	else
	{
		glutSolidSphere(0.02, 25, 25);
	}
	glPopMatrix();


	// Front Wheel 
	glPushMatrix();

	glColor3f(0.3, 0.3, 0.3);
	glTranslatef(1.0, 0.0, 0.0);
	glRotatef(wheelRotation, 0.0f, 0.0f, 1.0f);
	glScalef(1.0, 1.0, 2.0); 
	if (wire)
	{
		glutWireTorus(0.1, 0.2, 25, 25);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureId4);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
		glEnable(GL_TEXTURE_GEN_T);
		glutSolidTorus(0.1, 0.2, 25, 25);
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();

	// Rim #1 for Front Wheel
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(1.0, 0.0, 0.075);
	gluDisk(quadobj, 0, 0.1, 25, 25);
	glPopMatrix();

	// Rim #2 for Front Wheel
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(1.0, 0.0, -0.075);
	gluDisk(quadobj, 0, 0.1, 25, 25);
	glPopMatrix();	

	
}

void drawTrailer()
{
	// Hook for Moto
	glPushMatrix();
	glColor3b(120, 120 ,120);
	glTranslatef(-1.19f, 0.3f, 0.0f);
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	if (wire)
	{
		glutWireTorus(0.02, 0.05, 25, 25);
	}
	else
	{
		glutSolidTorus(0.02, 0.05, 25, 25);
	}
	glPopMatrix();

	//Cylinder to connect trailer to moto
	glPushMatrix();
	glColor3b(120, 120 ,120);
	glTranslatef(-.775f, 0.3f, 0.0f);
	glRotatef(45, 0.0f, 0.0f, 1.0f);
	glRotatef(90, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadobj, 0.01, 0.01, 0.125, 25, 25);
	glPopMatrix();

	//Cylinder to connect trailer to moto
	glPushMatrix();
	glColor3b(120, 120 ,120);
	glTranslatef(-1.2f, 0.3f, 0.0f);
	glRotatef(90, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadobj, 0.01, 0.01, 0.425, 25, 25);
	glPopMatrix();


	// Hook for Moto
	glPushMatrix();
	glColor3b(120, 120 ,120);
	glTranslatef(-1.225f, 0.3f, 0.0f);
	if (wire)
	{
		glutWireTorus(0.02, 0.05, 25, 25);
	}
	else
	{
		glutSolidTorus(0.02, 0.05, 25, 25);
	}
	glPopMatrix();

	//Cylinder to connect trailer to moto
	glPushMatrix();
	glColor3b(120, 120 ,120);
	glTranslatef(-1.625f, 0.3f, 0.0f);
	glRotatef(90, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadobj, 0.01, 0.01, 0.365, 25, 25);
	glPopMatrix();

	// Wheel 2
	glPushMatrix();
	glColor3f(0.3,0.3,0.3);
	glTranslatef(-2.0, 0.0, -0.7);
	glRotatef(wheelRotation, 0.0f, 0.0f, 1.0f);
	glScalef(1.0f, 1.0f, 2.0f);
	if (wire)
	{
		glutWireTorus(0.1, 0.2, 25, 25);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureId4);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
		glEnable(GL_TEXTURE_GEN_T);
		glutSolidTorus(0.1, 0.2, 25, 25);
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();

	// Wheel 1
	glPushMatrix();
	glColor3f(0.3,0.3,0.3);
	glTranslatef(-2.0, 0.0, 0.7);
	glRotatef(wheelRotation, 0.0f, 0.0f, 1.0f);
	glScalef(1.0f, 1.0f, 2.0f);
	if (wire)
	{
		glutWireTorus(0.1, 0.2, 25, 25);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureId4);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
		glEnable(GL_TEXTURE_GEN_T);
		glutSolidTorus(0.1, 0.2, 25, 25);
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();	

	// Main body of trailer
	glPushMatrix();
	glColor3b(120,120,120);
	glTranslatef(-2.0f, 0.3f, -0.0f);
	glScalef(2.5f, 1.0f, 2.5f);
	if (wire)
	{
		glutWireCube(0.3);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureId3);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
		glEnable(GL_TEXTURE_GEN_T);
		//glTexImage2D(GL_TEXTURE_2D, 4, GL_RGB, 200, 150, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
		glutSolidCube(0.3);
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();

	// Supporting Cylinder
	glPushMatrix();
	glColor3b(120,120,120);
	glTranslatef(-2.0f, 0.0f, -0.3f);
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	gluCylinder(quadobj, 0.05, 0.05, 0.4, 25, 25);
	glPopMatrix();

	// Supporting Cylinder
	glPushMatrix();
	glColor3b(120,120,120);
	glTranslatef(-2.0f, 0.0f, 0.3f);
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	gluCylinder(quadobj, 0.05, 0.05, 0.4, 25, 25);
	glPopMatrix();

	
	// Cylinder Connecting Wheel 1 and Wheel 2
	glPushMatrix();
	glColor3b(120,120,120);
	glTranslatef(-2.0f, 0.0f, -0.7f);
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	gluCylinder(quadobj, 0.05, 0.05, 1.4, 25, 25);
	glPopMatrix();

	// Rim #1 for Wheel 1
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(-2.0, 0.0, 0.7);
	gluDisk(quadobj, 0, 0.1, 25, 25);
	glPopMatrix();

	// Rim #2 for Wheel 1
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(-2.0, 0.0, -0.7);
	gluDisk(quadobj, 0, 0.1, 25, 25);
	glPopMatrix();	
}

void drawMotorcycle()
{
	glPushMatrix();
	glScalef(5.0f, 5.0f, 5.0f);
	drawSteering();
	drawBackWheel();
	drawFrontWheel();
	
	drawSeat();
	drawGasTank();
	drawEngine();

	glPushMatrix();	
	drawBody();
	glPopMatrix();
	drawTrailer();
	glPopMatrix();
}

void drawLines()
{
	// Draw the positive side of the lines x,y,z
	glBegin(GL_LINES);
		glColor3f (0.0, 1.0, 0.0);			// Green for x axis
		glVertex3f(0,0,0);
		glVertex3f(10,0,0);
		glColor3f(1.0,0.0,0.0);				// Red for y axis
		glVertex3f(0,0,0);
		glVertex3f(0,10,0);
		glColor3f(0.0,0.0,1.0);				// Blue for z axis
		glVertex3f(0,0,0);	
		glVertex3f(0,0,10);
	glEnd();
	
	// Dotted lines for the negative sides of x,y,z
	glEnable(GL_LINE_STIPPLE);				// Enable line stipple to use a dotted pattern for the lines
	glLineStipple(1, 0x0101);				// Dotted stipple pattern for the lines
	glBegin(GL_LINES);				
		glColor3f (0.0, 1.0, 0.0);			// Green for x axis
		glVertex3f(-10,0,0);
		glVertex3f(0,0,0);
		glColor3f(1.0,0.0,0.0);				// Red for y axis
		glVertex3f(0,0,0);
		glVertex3f(0,-10,0);
		glColor3f(0.0,0.0,1.0);				// Blue for z axis
		glVertex3f(0,0,0);
		glVertex3f(0,0,-10);
	glEnd();

	glDisable(GL_LINE_STIPPLE);				// Disable the line stipple
}

void idle()
{
	wheelRotation = (wheelRotation - wheelSpeed)%360;
}

// This method helps refresh the screen by putting a timer for when the wait method will be called, which essentially just redisplays the screen
void wait(int i){
	glutPostRedisplay();
	glutTimerFunc(30,wait,0);
}


// This function is called whenever the window size is changed
void reshape (int w, int h)
{	
	//glViewport (0, 0, (GLsizei) w, (GLsizei) h);				// Set the viewport
	glMatrixMode (GL_PROJECTION);								// Set the Matrix mode
	glLoadIdentity ();
	if (ortho)
	{
		glOrtho(GLfloat(-w/2), GLfloat(w/2), GLfloat(-h/2), GLfloat(h/2), 0.1, 3000);
		
	}
	else
	{
		gluPerspective(fovy, (GLfloat) w /(GLfloat) h , 0.10, 3000.0);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (thirdPersonView)
	{
		gluLookAt (cx, cy, cz, motoX, motoY, motoZ, 0.0, 1.0, 0.0); 
	}
	else
	{
		gluLookAt (cx, cy + 10.0f, cz, motoX + 30.0f*cos(motoRot * PI/180), motoY, motoZ - 30.0f*sin(motoRot * PI/180), 0.0, 1.0, 0.0);
	}
}

//	Moves the motorcycle around the track
void move(int left, int right, float radius)
{	
	GLfloat lightOne[]  = {1.0f,1.0f,1.0f,0.0f}; // white;
	if (!highBeamOn)
	{
		// softer white
		lightOne[0] = 0.3f;
		lightOne[1] = 0.3f;
		lightOne[2] = 0.3f;		
	}

	GLfloat lightTwo[] = {0.0f,0.0f,1.0f,0.0f}; // blue
	GLfloat lightThree[] = {1.0f,0.0f,0.0f,0.0f}; // crimsom red

	float curveAngle = 0;
	GLfloat spotLight[4];
	//	Track Width
	float outerRadius = radius;
	float innerRadius = radius/2;
	float motoRadius = (outerRadius + innerRadius) / 2;
	
	int pieces = 64;

	
	if ( motoX <= left && rotatedBy180)
	{
		curveAngle = (turnCounter/pieces)*PI + PI/2;
		motoRot = (turnCounter *180 + pieces*180)/ pieces;

		motoX = 30 * cos(curveAngle) - 40;
		motoZ = -30* sin(curveAngle);		
		
		if (motoRot >= 360)
		{
			rotatedBy180 = false;
		}	
		turnCounter+=(0.1f*wheelSpeed);
		if (motoRot >= 90)
		{
			spotLight[0] = 10* cos(curveAngle) + 10;
		}
		else
		{
			spotLight[0] = -10 + 10* cos(curveAngle) ;
			
		}		
		spotLight[1] = 0.0;
		spotLight[2] = 10;
		spotLight[3] = 1;
		if (highBeamOn)
		{
			spotLight[0] += -5.0;
		}
	}
	else if (motoX >= right && !rotatedBy180)
	{
		curveAngle = (turnCounter/pieces)*PI + 3*PI/2;
		motoRot = (turnCounter *180 )/ pieces;

		motoX = 10 * cos(curveAngle) + 40;
		motoZ = -10 * sin(curveAngle);				
		
		if (motoRot >= 180)
		{
			rotatedBy180 = true;
		}	
		turnCounter+=(0.1f*wheelSpeed);

		if (motoRot >= 90)
		{
			spotLight[0] = 10* cos(curveAngle) - 10;
		}
		else
		{
			spotLight[0] = 10 - 10* cos(curveAngle) ;
		}		
		spotLight[1] = 1;
		spotLight[2] = -10;
		spotLight[3] = 1;
		if (highBeamOn)
		{
			spotLight[0] += 5.0;
		}
	}
	else if (rotatedBy180)
	{

		spotLight[0] = -10.0;
		spotLight[1] = 0.0;
		spotLight[2] = 0;
		spotLight[3] = 1;
		curveAngle = 180;
		turnCounter = 5;
		motoX = motoX - (0.2f*wheelSpeed);		
		if (highBeamOn)
		{
			spotLight[0] += -5.0;
		}
	}
	else
	{
		spotLight[0] = 10.0;
		spotLight[1] = 0.0;
		spotLight[2] = 0;
		spotLight[3] = 1;
		turnCounter = 5;
		motoX = motoX + (0.2f*wheelSpeed);
		if (highBeamOn)
		{
			spotLight[0] += 5.0;
		}
	}

	if (thirdPersonView)
	{
		cx = motoX + camDist*cos(PI*rotX/180)*sin(PI*rotY/180);
		cy = camDist*sin(PI*rotX/180);
		cz = motoZ + camDist*cos(PI*rotX/180)*cos(PI*rotY/180);	
	}
	else
	{
		cx = motoX; 
		cy = motoY;
		cz = motoZ;		
	}

	viewAngle = curveAngle;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (thirdPersonView)
	{
		gluLookAt (cx + dx, cy, cz + dz, motoX, motoY, motoZ, 0.0, 1.0, 0.0); 
	}
	else
	{
		gluLookAt (cx, cy + 10.0f, cz, motoX + 75.0f*cos(motoRot * PI/180), motoY, motoZ - 75.0f*sin(motoRot * PI/180), 0.0, 1.0, 0.0);
	}
	//reshape((GLfloat) glutGet(GLUT_WINDOW_WIDTH), (GLfloat) glutGet(GLUT_WINDOW_HEIGHT));
	glPushMatrix();
	glTranslatef(motoX, motoY, motoZ);
	

	if (spotLightOn)
	{		
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightOne);
		glLightfv(GL_LIGHT0, GL_POSITION, spotLight);		
	}
	else
	{
		glDisable(GL_LIGHT0);
	}

	if (movingLightOn)
	{
		glDisable(GL_MODELVIEW);
		if (movingLightAngle > 360)
		{
			movingLightAngle = 0.0f;
		}
		if (freezeMovingLight)
		{
			movingLightAngle += 0.1f;
		}
		GLfloat movingLightPosition[] = {6*cos(movingLightAngle), 2, 6*sin(movingLightAngle), 1.0};
		glLightfv(GL_LIGHT3, GL_SPOT_CUTOFF, lightTwo);		
		glLightfv(GL_LIGHT3, GL_DIFFUSE, lightTwo);
		glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 50);
		glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 500);
		glLightfv(GL_LIGHT3, GL_POSITION, movingLightPosition);
		glEnable(GL_MODELVIEW);
	}
	else
	{
		glDisable(GL_LIGHT3);
	}

	if (eyeOfSauron)
	{
		glPushMatrix();		
		GLfloat evilLightPosition[] = {cx, cy + 10.0f, cz,1};
		glLightfv (GL_LIGHT4, GL_SPOT_CUTOFF, lightThree);		
		glLightfv(GL_LIGHT4, GL_DIFFUSE, lightThree);
		glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 30);
		glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 500);
		glLightfv(GL_LIGHT4, GL_POSITION, evilLightPosition);
		glPopMatrix();
	}
	else
	{
		glDisable(GL_LIGHT4);
	}

	glRotatef(motoRot, 0, 1, 0);
	drawMotorcycle();
	glPopMatrix();
}

// Initiliaze the OpenGL window
void init(void)
{
	glClearColor (0.0, 0.0, 0.0, 0.0);			// Clear the color 	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);


	glShadeModel (GL_FLAT);						// Set the shading model to GL_FLAT
	glEnable (GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);		// Set Line Antialiasing		   
    

	glEnable(GL_BLEND); //Enable alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set the blend function

	Image* image = loadBMP("metal.bmp"); // has some rust
	_textureId = loadTexture(image);
	Image* image2 = loadBMP("metal2.bmp"); // for the pipes
	_textureId2 = loadTexture(image2);
	Image* image3 = loadBMP("leather.bmp");
	_textureId3 = loadTexture(image3);
	Image* image4 = loadBMP("tire.bmp");
	_textureId4 = loadTexture(image4);
	delete image;
	delete image2;
	delete image3;
	delete image4;
}


void display(void)
{

	start = clock();		
	int left = -40;
	int right = 40;
	float radius = 40;
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	GLfloat directionalLight[] = {0.5, 0.5, 0.5, 0};
	
	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();
	glOrtho(GLfloat(-width/2), GLfloat(width/2), GLfloat(-height/2), GLfloat(height/2), 0.1, 3000);

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Clear the Color Buffer 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glViewport(0,height-fpsViewport,width,fpsViewport);		
	
	glPushMatrix();
		glColor3f(1,1,1);
		glRotatef(100,0,1,0);
		glRasterPos2i(width/2, fpsViewport/2);
		sprintf_s(msg,30,"frames per second: %.2f",FPS);
		text(msg);
	glPopMatrix();
	
	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();
	if (ortho)
	{
		glOrtho(cx, (GLdouble) width,  (GLdouble) height, 0.0, 0.10, 3000.0);
	}
	else
	{
		gluPerspective(fovy, (GLfloat) width /(GLfloat) height, 0.10, 3000.0);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0,0,width,height-fpsViewport);		
	glEnable(GL_LIGHTING);

	if (directionalLightOn)
	{		
		glLightfv(GL_LIGHT2, GL_AMBIENT, directionalLight);		
	}
	else
	{
		glDisable(GL_LIGHT2);
	}
	glPushMatrix();
		move(left, right, radius);
		glPushMatrix();
			glRotatef(90, 0, 1, 0);
			drawTrack(left, right, radius);
			drawGrass();
		glPopMatrix();		
	glPopMatrix();		

	idle();

	
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	

	//	Calculating FPS
	framecount++;
	end=clock();

	elapsed = elapsed + (double)(end-start)/CLOCKS_PER_SEC;	
	if(elapsed >=1)
	{
		FPS=framecount;
		framecount=0;
		elapsed=0;
	}

	glFlush();
	glutSwapBuffers();
}


// This function is used for the navigation keys
void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
		case '1': // First Person View
			thirdPersonView = false;
			break;
		case '2':
			freezeMovingLight = !freezeMovingLight;
			break;
		case '3': // Third Person View
			thirdPersonView = true;
			break;
		case 'h': // low beam
			highBeamOn = false;
			break;
		case 'H': // high beam
			highBeamOn = true;
			break;
		case 's':
		case 'S':
			if (moveable)
			{
				moveable = false;
				stoppedSpeed = wheelSpeed;
				wheelSpeed = 0;
			}
			break;
		case 'e':
		case 'E':
			if(!moveable)
			{
				moveable = true;
				wheelSpeed = stoppedSpeed;
			}
			break;
		case 'a':
			if (moveable)
				wheelSpeed++; // negative for forward motion, position to reverse the wheels			
			break;
		case'A':		
			if (moveable && wheelSpeed > 5)
				wheelSpeed--; // negative for forward motion, position to reverse the wheels			
		break;
		case 'w':
		case 'W':
			wire = !wire;
			break;
		case 'z':
			fovy -= 5;
			break;	
		case 'Z':
			fovy += 5;			
			break;
		case 'o':
		case 'O':
			ortho = true;			
			break;
		case 'p':
		case 'P':
			ortho = false;			
			break;
		case 'i':
		case 'I':
			if (rotX < 85)
				rotX += 5.0f;
			break;
		case 'k':
		case 'K':
			if (rotX > -85)
				rotX -= 5.0f;
			break;
		case 'j':			
		case 'J':
			rotY -= 5.0f;
			break;	
		case 'l':			
		case 'L':
			rotY += 5.0f;
			break;	
		case 'b':
			dx += 10.1f;
			dz += 10.1f;
			break;
		case 'f':
			dx -= 10.1f;
			dz -= 10.1f;
			break;		
		case 'C':				// Default, resets the translations vies from starting view
		case 'c':	
			wheelSpeed = 0;
		X = Y = 0.0f;
		Z = 0.0f;
		fovy = 90;
		rotX = 0.0f;
		rotY = 0.0f;
		rotZ = 0.0f;
		cx = 0.0f;
        cy = 0.0f;
        cz = 5.0f;
		dx = 0.0f;
		dy = 0.0f;
		dz = -1.0f;
        reshape((GLfloat) glutGet(GLUT_WINDOW_WIDTH), (GLfloat) glutGet(GLUT_WINDOW_HEIGHT));
		break;
		case 27:
			exit(0);			
	}
	glutPostRedisplay();	// Redraw the scene
}

// called on special key pressed
void specialKey(int key, int x, int y) { 
  
	// The keys below are using the gluLookAt() function for navigation
	// Check which key is pressed
    switch(key) {
		case GLUT_KEY_LEFT :    // Rotate on x axis
			rotY -= 5.0f;
          break;
        case GLUT_KEY_RIGHT :	// Rotate on x axis (opposite)
			rotY += 5.0f;
          break;
        case GLUT_KEY_UP :		// Rotate on y axis 
			if (rotX < 85)
				rotX += 5.0f;
          break;
        case GLUT_KEY_DOWN :	// Rotate on y axis (opposite)						
			if (rotX > -85)
				rotX -= 5.0f;
          break;	
  		case GLUT_KEY_F1:
			spotLightOn = !spotLightOn;
			break;		
		case GLUT_KEY_F2:
			movingLightOn = !movingLightOn;
			break;
		case GLUT_KEY_F3:
			eyeOfSauron = !eyeOfSauron;
			break;
		case GLUT_KEY_F4:
			directionalLightOn = !directionalLightOn;
			break;
    }
	glutPostRedisplay();		// Redraw the scene
}
// Main entry point of the program
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);		// Setup display mode to double buffer and RGB color
	glutInitWindowSize (600,600);						// Set the screen size
	
	glutCreateWindow("Motorcyle");	
	init ();

	glutReshapeFunc(reshape);		
	glutDisplayFunc(display);	
	glutKeyboardFunc(keyboard);		// set window's key callback 
	glutSpecialFunc(specialKey);	// set window's to specialKey callback

	
	wait(0);

	glutMainLoop();	
}