<<<<<<< HEAD
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <strstream>
#include <iomanip>
#include <string>

#include "matBike.h"
#include "bike.h"

MatBike::MatBike()
{	    
    init();
    quadric = gluNewQuadric();
    wire = false;
}
MatBike::MatBike(TextureManager* t, LightManager* l, SmokeManager* s)
{
	init();	
	tm = t;
	lm = l;
    sm = s;
	quadric = gluNewQuadric();
	wire = false;

	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);
}

MatBike::~MatBike(void)
{
}

void MatBike::init()
{
    heading = 0;
    bikePosX = 0;
    bikePosZ = 0;
    velocityZ = 0;
    velocityX = 0;
    wheelRotationDegree = 0;
    tireRad = 0.20;
    materialType = 0;
    blue();
}

void MatBike::draw()
{
    setMaterial();
    if (isJousting)
    {
        joust();
    }
    else
    {
        move();
    }
    glPushMatrix();//BIKE  
        //glScalef(1f, 0.5f, 0.5f);        
        glTranslated(bikePosX, 0.28, bikePosZ);
        glRotatef(heading, 0,1,0);
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
    glDisable(GL_COLOR_MATERIAL);
    
}

void MatBike::move()
{
    if(velocityX >= 0)
    {
        braking = true;
    }
    else
    {
        braking = false;
    }

    float circ = tireRad * 2 * PI;
    bikePosX -= velocityX * cos(heading);
    wheelRotationDegree += (velocityX/circ) * 360  * cos(heading) ;
    //bikePosX -= rotationToDistance;
    if(wheelRotationDegree > ALLROUND)
    {wheelRotationDegree -= ALLROUND;}
}

float MatBike::handleBarYPos(void)
{
    float yCoord = 0.85f;
    float handleBarAngleA = 45.0f;
    float height = 0.15f * sin(handleBarAngleA * 3.1415926 / 180 ); // sin only takes radians, not degrees
    float yPos = height + yCoord;	 
    return yPos;
}

float MatBike::handleBarXPos(void)
{    
    float xCoord = 0.85f;
    float handleBarAngleB = 90.0f - 45.0f;	// subtract handleBarAngleA;
    float bottom = 0.15f * sin(handleBarAngleB * 3.1415926 / 180); // sin only takes radians, not degrees
    float xPos = xCoord - bottom; // subtract the length of the bottom from the x coordinate desired
    return xPos;
}

void MatBike::drawSteering()
{
    float handleBarX = handleBarXPos();
    float handleBarY = handleBarYPos();

    // Bar 1 that connects handle bar to shaft
    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);
    glTranslatef(0.85, 0.85, -0.35);	// Take X and Y coord of handlebars in handleBarXPos() and handleBarYPos()
    gluCylinder(quadric, 0.02, 0.02, 0.35, 25, 25);
    glPopMatrix();

    // Bar 2 that connects handle bar to shaft
    glPushMatrix();	
    glColor3f(1.0, 1.0, 0.0);
    glTranslatef(0.85, 0.85, 0.0); // Take X and Y coord of handlebars in handleBarXPos() and handleBarYPos()
    gluCylinder(quadric, 0.02, 0.02, 0.35, 25, 25);
    glPopMatrix();

    // Sphere that connects bars to wheels
    glPushMatrix();
    glColor3f(1.0, 0.0, 1.0);
    glTranslatef(0.85, 0.85, 0.0);
    glScalef(0.1, 0.15, 0.2);

        glutSolidSphere(0.5, 15, 15);
    glPopMatrix();	

    // Handle bar 1
    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);	
    glTranslatef(handleBarX, handleBarY, -0.35); // z is based off Bar 1 cylinder height
    glRotatef(90.0f, 0.0, 1.0, 0.0);
    glRotatef(45.0f, 1.0, 0.0, 0.0); // Take handleBarAngleA

        tm->BindTextureByIndex(0);

    gluCylinder(quadric, 0.01, 0.01, 0.15f, 25, 25);

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    // Handle bar 2
    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);	
    glTranslatef(handleBarX, handleBarY, 0.35); // z is based off Bar 2 cylinder height
    glRotatef(90, 0.0, 1.0, 0.0);
    glRotatef(45.0f, 1.0, 0.0, 0.0);	// Take handleBarAngleA

        tm->BindTextureByIndex(0);	
    gluCylinder(quadric, 0.01, 0.01, 0.15f, 25, 25);
    
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
        glTranslatef(1.1f, 0.6f, 0.0f);
        glRotatef(45, 0.0f, 1.0f, 0.0f);
        if (okToDrawSpear)
            drawSpear(0.2, 0, 2.5, 75, 75);
    glPopMatrix();

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
        tm->BindTextureByIndex(0);
        glutSolidSphere(0.25, 30, 30);
        glDisable(GL_TEXTURE_2D);	
    }
    glPopMatrix();
}

void MatBike::drawSeat()
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

void MatBike::drawBody()
{
    applyColor();
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
        tm->BindTextureByIndex(0);
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
    setMaterial();
}

void MatBike::drawGasTank()
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

        tm->BindTextureByIndex(0);
    gluCylinder(quadric, 0.05, 0.05, 0.2, 25, 25);
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
    gluCylinder(quadric, 0.05, 0.05, 0.125, 25, 25);
    glPopMatrix();

}

void MatBike::drawEngine()
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

    tm->BindTextureByIndex(0);
    gluCylinder(quadric, 0.05, 0.05, 1.05, 25, 25);

    if (alpha > 0)
    {
        glPushMatrix();
            glDisable(GL_LIGHTING);
            glScalef(0.7, 0.5, 0.7);
            glTranslatef(0, 0.9, -0.05);
            sm->draw(0.05);
            glEnable(GL_LIGHTING);
        glPopMatrix();
    }

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();

    // Right Bottom Cylinder
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(-1.05f, 0.4f, 0.05f);
    glRotatef(90, 0.0, 1.0, 0.0);
    
    tm->BindTextureByIndex(0);
    gluCylinder(quadric, 0.05, 0.05, 1.05, 25, 25);	

    if (alpha > 0)
    {
        glPushMatrix();
            glDisable(GL_LIGHTING);
            glScalef(0.7, 0.5, 0.7);
            glTranslatef(0, 0.9, -0.05);
            sm->draw(0.05);
            glEnable(GL_LIGHTING);
        glPopMatrix();
    }

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void MatBike::drawBackWheel()
{
    // Right Small Cylinder attached to connecting sphere
    glPushMatrix();
    glColor3b(120,120,120);
    glTranslatef(-0.7f, 0.0f, 0.075f);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    gluCylinder(quadric, 0.02, 0.02, 0.17, 25, 25);
    glPopMatrix();

    // Left Small Cylinder attached to connecting sphere
    glPushMatrix();
    glColor3b(120,120,120);
    glTranslatef(-0.7f, 0.0f, -0.245f);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    gluCylinder(quadric, 0.02, 0.02, 0.17, 25, 25);
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
    gluCylinder(quadric, 0.02, 0.02, 0.45, 25, 25);
    glPopMatrix();
    
    // Left Large Cylinder attached to connecting sphere
    glPushMatrix();
    glColor3b(120,120,120);
    glTranslatef(-0.7f, 0.0f, -0.255f);
    glRotatef(55, 0.0f, 0.0f, 1.0f);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    gluCylinder(quadric, 0.02, 0.02, 0.45, 25, 25);
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
    glRotatef(wheelRotationDegree, 0.0f, 0.0f, 1.0f);
    glScalef(1.0, 1.0, 2.0); 
    if (wire)
    {
        glutWireTorus(0.1, 0.2, 25, 25);
    }
    else
    {
        tm->BindTextureByIndex(0);
        glutSolidTorus(0.1, 0.2, 25, 25);
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();	

    // Rim #1 for Back Wheel
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    glTranslatef(-0.7f, 0.0f, 0.075f);
    gluDisk(quadric, 0, 0.1, 25, 25);
    glPopMatrix();

    // Rim #2 for Back Wheel
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    glTranslatef(-0.7f, 0.0f, -0.075f);
    gluDisk(quadric, 0, 0.1, 25, 25);
    glPopMatrix();	
    
}

void MatBike::drawFrontWheel()
{	
    // Right Small Cylinder attached to connecting sphere
    glPushMatrix();
    glColor3b(120,120,120);
    glTranslatef(1.0, 0.0, 0.075);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    gluCylinder(quadric, 0.02, 0.02, 0.2, 25, 25);
    glPopMatrix();

    // Left Small Cylinder attached to connecting sphere
    glPushMatrix();
    glColor3b(120,120,120);
    glTranslatef(1.0, 0.0, -0.275);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    gluCylinder(quadric, 0.02, 0.02, 0.2, 25, 25);
    glPopMatrix();

    // Right Large Cylinder attached to connecting sphere
    glPushMatrix();
    glColor3b(120,120,120);
    glTranslatef(1.0, 0.0, 0.275);
    glRotatef(105, 0.0f, 0.0f, 1.0f);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    gluCylinder(quadric, 0.02, 0.04, 0.65, 25, 25);
    glPopMatrix();
    
    // Left Large Cylinder attached to connecting sphere
    glPushMatrix();
    glColor3b(120,120,120);
    glTranslatef(1.0, 0.0, -0.275);
    glRotatef(105, 0.0f, 0.0f, 1.0f);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    gluCylinder(quadric, 0.02, 0.04, 0.65, 25, 25);
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
    glRotatef(wheelRotationDegree, 0.0f, 0.0f, 1.0f);
    glScalef(1.0, 1.0, 2.0); 
    if (wire)
    {
        glutWireTorus(0.1, 0.2, 25, 25);
    }
    else
    {

        tm->BindTextureByIndex(0);
        glutSolidTorus(0.1, 0.2, 25, 25);
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();

    // Rim #1 for Front Wheel
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(1.0, 0.0, 0.075);
    gluDisk(quadric, 0, 0.1, 25, 25);
    glPopMatrix();

    // Rim #2 for Front Wheel
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(1.0, 0.0, -0.075);
    gluDisk(quadric, 0, 0.1, 25, 25);
    glPopMatrix();	

    
}

void MatBike::drawTrailer()
{    
    // Hook for Moto
    glPushMatrix();
    glColor3b(120, 120 ,120);
    glTranslatef(-1.19f, 0.3f, 0.0f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);

        glutSolidTorus(0.02, 0.05, 25, 25);
    
    glPopMatrix();

    //Cylinder to connect trailer to moto
    glPushMatrix();
    glColor3b(120, 120 ,120);
    glTranslatef(-.775f, 0.3f, 0.0f);
    glRotatef(45, 0.0f, 0.0f, 1.0f);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    gluCylinder(quadric, 0.01, 0.01, 0.125, 25, 25);
    glPopMatrix();

    //Cylinder to connect trailer to moto
    glPushMatrix();
    glColor3b(120, 120 ,120);
    glTranslatef(-1.2f, 0.3f, 0.0f);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    gluCylinder(quadric, 0.01, 0.01, 0.425, 25, 25);
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
    gluCylinder(quadric, 0.01, 0.01, 0.365, 25, 25);
    glPopMatrix();

    // Wheel 2
    glPushMatrix();
    glColor3f(0.3,0.3,0.3);
    glTranslatef(-2.0, 0.0, -0.7);
    glRotatef(wheelRotationDegree, 0.0f, 0.0f, 1.0f);
    glScalef(1.0f, 1.0f, 2.0f);
    if (wire)
    {
        glutWireTorus(0.1, 0.2, 25, 25);
    }
    else
    {
        tm->BindTextureByIndex(0);
        glutSolidTorus(0.1, 0.2, 25, 25);
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();

    // Wheel 1
    glPushMatrix();
    glColor3f(0.3,0.3,0.3);
    glTranslatef(-2.0, 0.0, 0.7);
    glRotatef(wheelRotationDegree, 0.0f, 0.0f, 1.0f);
    glScalef(1.0f, 1.0f, 2.0f);
    if (wire)
    {
        glutWireTorus(0.1, 0.2, 25, 25);
    }
    else
    {
        tm->BindTextureByIndex(0);
        glutSolidTorus(0.1, 0.2, 25, 25);
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();	

    applyColor();
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
        tm->BindTextureByIndex(0);
        glutSolidCube(0.3);
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();
    setMaterial();

    // Supporting Cylinder
    glPushMatrix();
    glColor3b(120,120,120);
    glTranslatef(-2.0f, 0.0f, -0.3f);
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    gluCylinder(quadric, 0.05, 0.05, 0.4, 25, 25);
    glPopMatrix();

    // Supporting Cylinder
    glPushMatrix();
    glColor3b(120,120,120);
    glTranslatef(-2.0f, 0.0f, 0.3f);
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    gluCylinder(quadric, 0.05, 0.05, 0.4, 25, 25);
    glPopMatrix();

    
    // Cylinder Connecting Wheel 1 and Wheel 2
    glPushMatrix();
    glColor3b(120,120,120);
    glTranslatef(-2.0f, 0.0f, -0.7f);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    gluCylinder(quadric, 0.05, 0.05, 1.4, 25, 25);
    glPopMatrix();

    // Rim #1 for Wheel 1
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(-2.0, 0.0, 0.7);
    gluDisk(quadric, 0, 0.1, 25, 25);
    glPopMatrix();

    // Rim #2 for Wheel 1
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(-2.0, 0.0, -0.7);
    gluDisk(quadric, 0, 0.1, 25, 25);
    glPopMatrix();	
=======
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <strstream>
#include <iomanip>
#include <string>

#include "matBike.h"
#include "bike.h"

MatBike::MatBike()
{	    
    init();
    quadric = gluNewQuadric();
    wire = false;
}
MatBike::MatBike(TextureManager* t, LightManager* l, SmokeManager* s)
{
	init();	
	tm = t;
	lm = l;
    sm = s;
	quadric = gluNewQuadric();
	wire = false;

	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);
}

MatBike::~MatBike(void)
{
}

void MatBike::init()
{
    heading = 0;
    bikePosX = 0;
    bikePosZ = 0;
    velocityZ = 0;
    velocityX = 0;
    wheelRotationDegree = 0;
    tireRad = 0.20;
    materialType = 0;
    blue();
}

void MatBike::draw()
{
    setMaterial();
    if (isJousting)
    {
        joust();
    }
    else
    {
        move();
    }
    glPushMatrix();//BIKE  
        //glScalef(1f, 0.5f, 0.5f);        
        glTranslated(bikePosX, 0.28, bikePosZ);
        glRotatef(heading, 0,1,0);
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
    glDisable(GL_COLOR_MATERIAL);
    
}

void MatBike::move()
{
    if(velocityX >= 0)
    {
        braking = true;
    }
    else
    {
        braking = false;
    }

    float circ = tireRad * 2 * PI;
    bikePosX -= velocityX * cos(heading);
    wheelRotationDegree += (velocityX/circ) * 360  * cos(heading) ;
    //bikePosX -= rotationToDistance;
    if(wheelRotationDegree > ALLROUND)
    {wheelRotationDegree -= ALLROUND;}
}

float MatBike::handleBarYPos(void)
{
    float yCoord = 0.85f;
    float handleBarAngleA = 45.0f;
    float height = 0.15f * sin(handleBarAngleA * 3.1415926 / 180 ); // sin only takes radians, not degrees
    float yPos = height + yCoord;	 
    return yPos;
}

float MatBike::handleBarXPos(void)
{    
    float xCoord = 0.85f;
    float handleBarAngleB = 90.0f - 45.0f;	// subtract handleBarAngleA;
    float bottom = 0.15f * sin(handleBarAngleB * 3.1415926 / 180); // sin only takes radians, not degrees
    float xPos = xCoord - bottom; // subtract the length of the bottom from the x coordinate desired
    return xPos;
}

void MatBike::drawSteering()
{
    float handleBarX = handleBarXPos();
    float handleBarY = handleBarYPos();

    // Bar 1 that connects handle bar to shaft
    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);
    glTranslatef(0.85, 0.85, -0.35);	// Take X and Y coord of handlebars in handleBarXPos() and handleBarYPos()
    gluCylinder(quadric, 0.02, 0.02, 0.35, 25, 25);
    glPopMatrix();

    // Bar 2 that connects handle bar to shaft
    glPushMatrix();	
    glColor3f(1.0, 1.0, 0.0);
    glTranslatef(0.85, 0.85, 0.0); // Take X and Y coord of handlebars in handleBarXPos() and handleBarYPos()
    gluCylinder(quadric, 0.02, 0.02, 0.35, 25, 25);
    glPopMatrix();

    // Sphere that connects bars to wheels
    glPushMatrix();
    glColor3f(1.0, 0.0, 1.0);
    glTranslatef(0.85, 0.85, 0.0);
    glScalef(0.1, 0.15, 0.2);

        glutSolidSphere(0.5, 15, 15);
    glPopMatrix();	

    // Handle bar 1
    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);	
    glTranslatef(handleBarX, handleBarY, -0.35); // z is based off Bar 1 cylinder height
    glRotatef(90.0f, 0.0, 1.0, 0.0);
    glRotatef(45.0f, 1.0, 0.0, 0.0); // Take handleBarAngleA

        tm->BindTextureByIndex(0);

    gluCylinder(quadric, 0.01, 0.01, 0.15f, 25, 25);

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    // Handle bar 2
    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);	
    glTranslatef(handleBarX, handleBarY, 0.35); // z is based off Bar 2 cylinder height
    glRotatef(90, 0.0, 1.0, 0.0);
    glRotatef(45.0f, 1.0, 0.0, 0.0);	// Take handleBarAngleA

        tm->BindTextureByIndex(0);	
    gluCylinder(quadric, 0.01, 0.01, 0.15f, 25, 25);
    
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
        glTranslatef(1.1f, 0.6f, 0.0f);
        glRotatef(45, 0.0f, 1.0f, 0.0f);
        if (okToDrawSpear)
            drawSpear(0.2, 0, 2.5, 75, 75);
    glPopMatrix();

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
        tm->BindTextureByIndex(0);
        glutSolidSphere(0.25, 30, 30);
        glDisable(GL_TEXTURE_2D);	
    }
    glPopMatrix();
}

void MatBike::drawSeat()
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

void MatBike::drawBody()
{
    applyColor();
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
        tm->BindTextureByIndex(0);
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
    setMaterial();
}

void MatBike::drawGasTank()
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

        tm->BindTextureByIndex(0);
    gluCylinder(quadric, 0.05, 0.05, 0.2, 25, 25);
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
    gluCylinder(quadric, 0.05, 0.05, 0.125, 25, 25);
    glPopMatrix();

}

void MatBike::drawEngine()
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

    tm->BindTextureByIndex(0);
    gluCylinder(quadric, 0.05, 0.05, 1.05, 25, 25);

    if (alpha > 0)
    {
        glPushMatrix();
            glDisable(GL_LIGHTING);
            glScalef(0.7, 0.5, 0.7);
            glTranslatef(0, 0.9, -0.05);
            sm->draw(0.05);
            glEnable(GL_LIGHTING);
        glPopMatrix();
    }

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();

    // Right Bottom Cylinder
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(-1.05f, 0.4f, 0.05f);
    glRotatef(90, 0.0, 1.0, 0.0);
    
    tm->BindTextureByIndex(0);
    gluCylinder(quadric, 0.05, 0.05, 1.05, 25, 25);	

    if (alpha > 0)
    {
        glPushMatrix();
            glDisable(GL_LIGHTING);
            glScalef(0.7, 0.5, 0.7);
            glTranslatef(0, 0.9, -0.05);
            sm->draw(0.05);
            glEnable(GL_LIGHTING);
        glPopMatrix();
    }

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void MatBike::drawBackWheel()
{
    // Right Small Cylinder attached to connecting sphere
    glPushMatrix();
    glColor3b(120,120,120);
    glTranslatef(-0.7f, 0.0f, 0.075f);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    gluCylinder(quadric, 0.02, 0.02, 0.17, 25, 25);
    glPopMatrix();

    // Left Small Cylinder attached to connecting sphere
    glPushMatrix();
    glColor3b(120,120,120);
    glTranslatef(-0.7f, 0.0f, -0.245f);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    gluCylinder(quadric, 0.02, 0.02, 0.17, 25, 25);
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
    gluCylinder(quadric, 0.02, 0.02, 0.45, 25, 25);
    glPopMatrix();
    
    // Left Large Cylinder attached to connecting sphere
    glPushMatrix();
    glColor3b(120,120,120);
    glTranslatef(-0.7f, 0.0f, -0.255f);
    glRotatef(55, 0.0f, 0.0f, 1.0f);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    gluCylinder(quadric, 0.02, 0.02, 0.45, 25, 25);
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
    glRotatef(wheelRotationDegree, 0.0f, 0.0f, 1.0f);
    glScalef(1.0, 1.0, 2.0); 
    if (wire)
    {
        glutWireTorus(0.1, 0.2, 25, 25);
    }
    else
    {
        tm->BindTextureByIndex(0);
        glutSolidTorus(0.1, 0.2, 25, 25);
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();	

    // Rim #1 for Back Wheel
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    glTranslatef(-0.7f, 0.0f, 0.075f);
    gluDisk(quadric, 0, 0.1, 25, 25);
    glPopMatrix();

    // Rim #2 for Back Wheel
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    glTranslatef(-0.7f, 0.0f, -0.075f);
    gluDisk(quadric, 0, 0.1, 25, 25);
    glPopMatrix();	
    
}

void MatBike::drawFrontWheel()
{	
    // Right Small Cylinder attached to connecting sphere
    glPushMatrix();
    glColor3b(120,120,120);
    glTranslatef(1.0, 0.0, 0.075);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    gluCylinder(quadric, 0.02, 0.02, 0.2, 25, 25);
    glPopMatrix();

    // Left Small Cylinder attached to connecting sphere
    glPushMatrix();
    glColor3b(120,120,120);
    glTranslatef(1.0, 0.0, -0.275);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    gluCylinder(quadric, 0.02, 0.02, 0.2, 25, 25);
    glPopMatrix();

    // Right Large Cylinder attached to connecting sphere
    glPushMatrix();
    glColor3b(120,120,120);
    glTranslatef(1.0, 0.0, 0.275);
    glRotatef(105, 0.0f, 0.0f, 1.0f);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    gluCylinder(quadric, 0.02, 0.04, 0.65, 25, 25);
    glPopMatrix();
    
    // Left Large Cylinder attached to connecting sphere
    glPushMatrix();
    glColor3b(120,120,120);
    glTranslatef(1.0, 0.0, -0.275);
    glRotatef(105, 0.0f, 0.0f, 1.0f);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    gluCylinder(quadric, 0.02, 0.04, 0.65, 25, 25);
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
    glRotatef(wheelRotationDegree, 0.0f, 0.0f, 1.0f);
    glScalef(1.0, 1.0, 2.0); 
    if (wire)
    {
        glutWireTorus(0.1, 0.2, 25, 25);
    }
    else
    {

        tm->BindTextureByIndex(0);
        glutSolidTorus(0.1, 0.2, 25, 25);
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();

    // Rim #1 for Front Wheel
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(1.0, 0.0, 0.075);
    gluDisk(quadric, 0, 0.1, 25, 25);
    glPopMatrix();

    // Rim #2 for Front Wheel
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(1.0, 0.0, -0.075);
    gluDisk(quadric, 0, 0.1, 25, 25);
    glPopMatrix();	

    
}

void MatBike::drawTrailer()
{    
    // Hook for Moto
    glPushMatrix();
    glColor3b(120, 120 ,120);
    glTranslatef(-1.19f, 0.3f, 0.0f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);

        glutSolidTorus(0.02, 0.05, 25, 25);
    
    glPopMatrix();

    //Cylinder to connect trailer to moto
    glPushMatrix();
    glColor3b(120, 120 ,120);
    glTranslatef(-.775f, 0.3f, 0.0f);
    glRotatef(45, 0.0f, 0.0f, 1.0f);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    gluCylinder(quadric, 0.01, 0.01, 0.125, 25, 25);
    glPopMatrix();

    //Cylinder to connect trailer to moto
    glPushMatrix();
    glColor3b(120, 120 ,120);
    glTranslatef(-1.2f, 0.3f, 0.0f);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    gluCylinder(quadric, 0.01, 0.01, 0.425, 25, 25);
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
    gluCylinder(quadric, 0.01, 0.01, 0.365, 25, 25);
    glPopMatrix();

    // Wheel 2
    glPushMatrix();
    glColor3f(0.3,0.3,0.3);
    glTranslatef(-2.0, 0.0, -0.7);
    glRotatef(wheelRotationDegree, 0.0f, 0.0f, 1.0f);
    glScalef(1.0f, 1.0f, 2.0f);
    if (wire)
    {
        glutWireTorus(0.1, 0.2, 25, 25);
    }
    else
    {
        tm->BindTextureByIndex(0);
        glutSolidTorus(0.1, 0.2, 25, 25);
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();

    // Wheel 1
    glPushMatrix();
    glColor3f(0.3,0.3,0.3);
    glTranslatef(-2.0, 0.0, 0.7);
    glRotatef(wheelRotationDegree, 0.0f, 0.0f, 1.0f);
    glScalef(1.0f, 1.0f, 2.0f);
    if (wire)
    {
        glutWireTorus(0.1, 0.2, 25, 25);
    }
    else
    {
        tm->BindTextureByIndex(0);
        glutSolidTorus(0.1, 0.2, 25, 25);
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();	

    applyColor();
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
        tm->BindTextureByIndex(0);
        glutSolidCube(0.3);
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();
    setMaterial();

    // Supporting Cylinder
    glPushMatrix();
    glColor3b(120,120,120);
    glTranslatef(-2.0f, 0.0f, -0.3f);
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    gluCylinder(quadric, 0.05, 0.05, 0.4, 25, 25);
    glPopMatrix();

    // Supporting Cylinder
    glPushMatrix();
    glColor3b(120,120,120);
    glTranslatef(-2.0f, 0.0f, 0.3f);
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    gluCylinder(quadric, 0.05, 0.05, 0.4, 25, 25);
    glPopMatrix();

    
    // Cylinder Connecting Wheel 1 and Wheel 2
    glPushMatrix();
    glColor3b(120,120,120);
    glTranslatef(-2.0f, 0.0f, -0.7f);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    gluCylinder(quadric, 0.05, 0.05, 1.4, 25, 25);
    glPopMatrix();

    // Rim #1 for Wheel 1
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(-2.0, 0.0, 0.7);
    gluDisk(quadric, 0, 0.1, 25, 25);
    glPopMatrix();

    // Rim #2 for Wheel 1
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(-2.0, 0.0, -0.7);
    gluDisk(quadric, 0, 0.1, 25, 25);
    glPopMatrix();	
>>>>>>> first
}