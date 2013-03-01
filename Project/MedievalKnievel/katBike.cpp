<<<<<<< HEAD
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <strstream>
#include <iomanip>
#include <string>

#include "katBike.h"
#include "bike.h"


KatBike::KatBike(){

    init();
    quadric = gluNewQuadric();
}

KatBike::KatBike(TextureManager* t, LightManager* l, SmokeManager* s)
{
	init();	
	tm = t;
	lm = l;
    sm = s;
	quadric = gluNewQuadric();
	
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);
}

KatBike::~KatBike()
{
}

void KatBike::init()
{
    heading = 0;
    bikePosX = 0;
    bikePosZ = 0;
    velocityZ = 0;
    velocityX = 0;
    wheelRotationDegree = 0;
    tireRad = 0.2;
    materialType = 0;
    red();
}

void KatBike::draw()
{
    if (isJousting)
    {
        joust();
    }
    else
    {
        move();
    }
    glPushMatrix();
    glEnable(GL_NORMALIZE);
    setMaterial();
    glPushMatrix();
        glTranslatef(bikePosX,0.05,bikePosZ);
        glRotatef(heading, 0,1,0);
        glScalef(2,2,2);
        glPushMatrix();
            glTranslatef(0.75, 0.46, -0.10);
            glRotatef(45, 0.0f, 1.0f, 0.0f);
            if (okToDrawSpear)
                drawSpear(0.1, 0, 1.25, 75, 75);
        glPopMatrix();
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
    glDisable(GL_NORMALIZE);
    glPopMatrix();
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
}

void KatBike::move()
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
    wheelRotationDegree += (velocityX/circ) * 360 * cos(heading);
    //bikePosX -= rotationToDistance;
    if(wheelRotationDegree > ALLROUND)
    {wheelRotationDegree -= ALLROUND;}
}

void  KatBike::DrawMotercycleTireSpoke(GLdouble rotateX, GLdouble rotateY)
{
    glPushMatrix();
        glRotatef(90,rotateX,rotateY,0);
        glTranslatef(0,0,-.08);	
        glColor3f(1,1,1);
        tm->BindTextureByIndex(9); 
        gluCylinder(quadric,0.0075f,0.0075f,0.16f,32,32); 
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

//This method is responsible for drawing a tire of a motercycle.
void  KatBike::DrawMotercycleTire(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
    glPushMatrix();
        glTranslatef(translateX,translateY,translateZ);
        glRotatef(-wheelRotationDegree, 0,0,1);
        DrawMotercycleTireSpoke(-1,1);
        DrawMotercycleTireSpoke(1,1);
        DrawMotercycleTireSpoke(0,1);
        DrawMotercycleTireSpoke(1,0);
        glColor3f(0.30,0.30,0.30);
        glutSolidTorus(0.03, 0.1, 32, 32);
    glPopMatrix();
}

//This method is responsible for drawing any cross bars between the handle bar shafts.
void  KatBike::DrawHandleCrossBar(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
    glPushMatrix();
        //gluQuadricDrawStyle(quadric, GLU_FILL);
        glColor3f(1,1,1);	
    glTranslatef(translateX, translateY, translateZ);
    gluCylinder(quadric,0.02f,0.02f,0.2f,32,32);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

//This method is responsible for drawing the inside spokes of a motercycle 
//tire and is a helper mthod to the DrawMotercycleTire function.
void  KatBike::DrawHandleBarShaft(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
    glPushMatrix();
        //gluQuadricDrawStyle(quadric, GLU_FILL);
        glColor3f(1,1,1);
        glTranslatef(translateX,translateY,translateZ);
        glRotatef(-90,1,0,0);
        glRotatef(-52.5,0,1,0);	
        gluCylinder(quadric,0.02f,0.02f,0.5f,32,32);
        //glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

//This method is responsible for drawing a handle bar for the motercycle.
void  KatBike::DrawHandleBars(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
    glPushMatrix();
        glTranslatef(translateX,translateY,translateZ);
        glPushMatrix();

            glColor3f(1,1,1);
            //gluQuadricDrawStyle(quadric, GLU_FILL);
            gluCylinder(quadric,0.015f,0.015f,0.2f,32,32);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        //Right Handle
        glPushMatrix();
            glTranslatef(-.05,.05,0);
            glRotatef(-135,0,1,0);
            glRotatef(15,1,0,0);
            gluCylinder(quadric,0.02f,0.02f,0.1f,32,32);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        //Left Handle
        glPushMatrix();
            glTranslatef(-.05,.05,0.2);
            glRotatef(-45,0,1,0);
            glRotatef(15,1,0,0);
            gluCylinder(quadric,0.02f,0.02f,0.1f,32,32);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

    glPopMatrix();
}

//This method draws the part of the chassy that holds up the engine, gas tank and seat
void  KatBike::DrawEngineSupport(GLfloat translateX, GLfloat translateY, GLfloat translateZ){

    glPushMatrix();
        //gluQuadricDrawStyle(quadric, GLU_FILL);
        glColor3f(1, 1, 1);
    
        glTranslatef(translateX,translateY,translateZ);
        glRotatef(90,1,0,0);
        glRotatef(-52.5,0,1,0);	
        tm->BindTextureByIndex(2);
        gluCylinder(quadric,0.02f,0.02f,0.2f,32,32);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

//This method draws the part of the chassy that holds up the engine, gas tank and seat
void  KatBike::DrawOtherEngineSupport(GLfloat translateX, GLfloat translateY, GLfloat translateZ){

    glPushMatrix();
        //gluQuadricDrawStyle(quadric, GLU_FILL);
        glColor3f(1, 1, 1);
    
        glTranslatef(translateX,translateY,translateZ);
        glRotatef(90,1,0,0);
        glRotatef(52.5,0,1,0);
        tm->BindTextureByIndex(2);
        gluCylinder(quadric,0.02f,0.02f,0.2f,32,32);
        glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

//This method draws the trailor hitch for the motercycle.
void  KatBike::DrawTrailerHitch(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
    glPushMatrix();

        glTranslatef(translateX,translateY,translateZ);
            glColor3f(1,1,1);
            gluQuadricDrawStyle(quadric, GLU_FILL);
    

        //Top Horizontal Bar
        glPushMatrix();
            glRotatef(90,0,1,0);
            tm->BindTextureByIndex(2);
            gluCylinder(quadric,0.018f,0.018f,0.1f,32,32);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        //Middle Bar
        glPushMatrix();
            glTranslatef(0.09,-0.01,0);
            glRotatef(-90,1,0,0);
            tm->BindTextureByIndex(2);
            gluCylinder(quadric,0.02f,0.02f,0.18f,32,32);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        //Bottum
        glPushMatrix();
            glTranslatef(0.09,0.15,0);
            glRotatef(90,0,1,0);
            tm->BindTextureByIndex(0);
            gluCylinder(quadric,0.02f,0.02f,0.35f,32,32);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

    glPopMatrix();
}

//This method is charged with drawing the trailor for the motercycle.
void  KatBike::DrawTrailer(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
    glPushMatrix();
        glTranslatef(translateX,translateY,translateZ);
        applyColor();
            glColor3f(1,1,1);
            tm->BindTextureByIndex(9); 
            gluSphere(quadric, 0.2, 32, 32);
            glDisable(GL_TEXTURE_2D);
            glPushMatrix();
            glTranslatef(0.09,-0.03,0);
            glRotatef(90,0,1,0);
            glRotatef(21,1,0,0);
            tm->BindTextureByIndex(9);
            gluCylinder(quadric,0.18,0.0,0.4,32,32);
            glDisable(GL_TEXTURE_2D);
        setMaterial();
    glPopMatrix();
    
    glPopMatrix();
}

//This method is responsible for drawing the motercycle seat.
void  KatBike::DrawSeat(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
    glPushMatrix();
    glTranslatef(translateX,translateY,translateZ);
    applyColor();
        glColor3f(1.0,1.0, 1.0);
        tm->BindTextureByIndex(9);
        gluSphere(quadric, 0.1, 32, 32);
        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
            glTranslatef(0.05,-0.0075,0);
            glRotatef(90,0,1,0);
            glRotatef(21,1,0,0);
            tm->BindTextureByIndex(9);
            gluCylinder(quadric, 0.08, 0.0, 0.25, 32, 32);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    setMaterial();
    glPopMatrix();
}

//This method is responsible for drawing the motercycle seat.
void  KatBike::DrawGasTank(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
    glPushMatrix();
    glTranslatef(translateX,translateY,translateZ);
    applyColor();
        glColor3f(1.0, 1.0, 1.0);
        tm->BindTextureByIndex(9); 
        gluSphere(quadric, 0.077, 32, 32);
        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
            glTranslatef(-0.05,-0.01,0);
            glRotatef(-90,0,1,0);
            glRotatef(15,1,0,0);
            tm->BindTextureByIndex(9); 
            gluCylinder(quadric,0.06,0.0,0.2,32,32);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    setMaterial();
    glPopMatrix();
}

//This method is responsible for drawing the engine.
void  KatBike::DrawEngine(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
    glPushMatrix();
    glTranslatef(translateX,translateY,translateZ);
    glColor3f(0.70, 0.70 , 0.70);

        glutSolidCube(.15);
        glPushMatrix();
            glTranslatef(-0.15,0,0);
            glutSolidCube(.15);
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-0.25,0,0);
            glutSolidCube(.15);
        glPopMatrix();
    
    glPopMatrix();
}

//This method is responsible for drawing the exhaust pipe.
void  KatBike::DrawExhaustPipe(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
    glPushMatrix();
        glColor3f(1,1,1);
        //gluQuadricDrawStyle(quadric, GLU_FILL);
    
        glTranslatef(translateX, translateY, translateZ);
    
        glRotatef(-90,0,1,0);
    
        tm->BindTextureByIndex(2);
        gluCylinder(quadric,0.02f,0.02f,0.4f,32,32);
        glDisable(GL_TEXTURE_2D);

        glPushMatrix();
            glTranslatef(0,0,0.3);
            tm->BindTextureByIndex(2);
            gluCylinder(quadric,0.03f,0.03f,0.45f,32,32);
            glDisable(GL_TEXTURE_2D);
            if (alpha > 0)
            {
                glPushMatrix();
                    glDisable(GL_LIGHTING);
                    glRotatef(180, 0, 1, 0);
                    glScalef(0.3, 0.23, 0.3);
                    glTranslatef(0, 0.9, -1.5);
                    sm->draw(0.07);
                    glEnable(GL_LIGHTING);
                glPopMatrix();
            }
        glPopMatrix();

    glPopMatrix();
}

//This method is responsible for drawing the front light
void  KatBike::DrawFrontLight(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{	
    glPushMatrix();
        glTranslatef(translateX,translateY,translateZ);
        glColor3f(1, 1, 1);

        glutSolidSphere(0.04, 32, 32);
        //gluQuadricDrawStyle(quadric, GLU_FILL);			

        glPushMatrix();
            glTranslatef(0.01,0,0);
            glRotatef(-90,0,1,0);	
            tm->BindTextureByIndex(9);
            gluCylinder(quadric,0.045f,0.028f,0.08f,32,32);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();	
    glPopMatrix();
}

//This method is responsible for drawing the back light
void  KatBike::DrawBackLight(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
    glPushMatrix();
        glTranslatef(translateX,translateY,translateZ);
        glColor3f(1, 0, 0);
        glutSolidSphere(0.03, 32, 32);
        //gluQuadricDrawStyle(quadric, GLU_FILL);
        glColor3f(1, 1, 1);

        glPushMatrix();
            glTranslatef(0.01,0,0);
            glRotatef(90,0,1,0);
            tm->BindTextureByIndex(9);
            gluCylinder(quadric,0.035f,0.018f,0.07f,32,32);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    glPopMatrix();
}
=======
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <strstream>
#include <iomanip>
#include <string>

#include "katBike.h"
#include "bike.h"


KatBike::KatBike(){

    init();
    quadric = gluNewQuadric();
}

KatBike::KatBike(TextureManager* t, LightManager* l, SmokeManager* s)
{
	init();	
	tm = t;
	lm = l;
    sm = s;
	quadric = gluNewQuadric();
	
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);
}

KatBike::~KatBike()
{
}

void KatBike::init()
{
    heading = 0;
    bikePosX = 0;
    bikePosZ = 0;
    velocityZ = 0;
    velocityX = 0;
    wheelRotationDegree = 0;
    tireRad = 0.2;
    materialType = 0;
    red();
}

void KatBike::draw()
{
    if (isJousting)
    {
        joust();
    }
    else
    {
        move();
    }
    glPushMatrix();
    glEnable(GL_NORMALIZE);
    setMaterial();
    glPushMatrix();
        glTranslatef(bikePosX,0.05,bikePosZ);
        glRotatef(heading, 0,1,0);
        glScalef(2,2,2);
        glPushMatrix();
            glTranslatef(0.75, 0.46, -0.10);
            glRotatef(45, 0.0f, 1.0f, 0.0f);
            if (okToDrawSpear)
                drawSpear(0.1, 0, 1.25, 75, 75);
        glPopMatrix();
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
    glDisable(GL_NORMALIZE);
    glPopMatrix();
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
}

void KatBike::move()
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
    wheelRotationDegree += (velocityX/circ) * 360 * cos(heading);
    //bikePosX -= rotationToDistance;
    if(wheelRotationDegree > ALLROUND)
    {wheelRotationDegree -= ALLROUND;}
}

void  KatBike::DrawMotercycleTireSpoke(GLdouble rotateX, GLdouble rotateY)
{
    glPushMatrix();
        glRotatef(90,rotateX,rotateY,0);
        glTranslatef(0,0,-.08);	
        glColor3f(1,1,1);
        tm->BindTextureByIndex(9); 
        gluCylinder(quadric,0.0075f,0.0075f,0.16f,32,32); 
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

//This method is responsible for drawing a tire of a motercycle.
void  KatBike::DrawMotercycleTire(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
    glPushMatrix();
        glTranslatef(translateX,translateY,translateZ);
        glRotatef(-wheelRotationDegree, 0,0,1);
        DrawMotercycleTireSpoke(-1,1);
        DrawMotercycleTireSpoke(1,1);
        DrawMotercycleTireSpoke(0,1);
        DrawMotercycleTireSpoke(1,0);
        glColor3f(0.30,0.30,0.30);
        glutSolidTorus(0.03, 0.1, 32, 32);
    glPopMatrix();
}

//This method is responsible for drawing any cross bars between the handle bar shafts.
void  KatBike::DrawHandleCrossBar(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
    glPushMatrix();
        //gluQuadricDrawStyle(quadric, GLU_FILL);
        glColor3f(1,1,1);	
    glTranslatef(translateX, translateY, translateZ);
    gluCylinder(quadric,0.02f,0.02f,0.2f,32,32);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

//This method is responsible for drawing the inside spokes of a motercycle 
//tire and is a helper mthod to the DrawMotercycleTire function.
void  KatBike::DrawHandleBarShaft(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
    glPushMatrix();
        //gluQuadricDrawStyle(quadric, GLU_FILL);
        glColor3f(1,1,1);
        glTranslatef(translateX,translateY,translateZ);
        glRotatef(-90,1,0,0);
        glRotatef(-52.5,0,1,0);	
        gluCylinder(quadric,0.02f,0.02f,0.5f,32,32);
        //glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

//This method is responsible for drawing a handle bar for the motercycle.
void  KatBike::DrawHandleBars(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
    glPushMatrix();
        glTranslatef(translateX,translateY,translateZ);
        glPushMatrix();

            glColor3f(1,1,1);
            //gluQuadricDrawStyle(quadric, GLU_FILL);
            gluCylinder(quadric,0.015f,0.015f,0.2f,32,32);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        //Right Handle
        glPushMatrix();
            glTranslatef(-.05,.05,0);
            glRotatef(-135,0,1,0);
            glRotatef(15,1,0,0);
            gluCylinder(quadric,0.02f,0.02f,0.1f,32,32);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        //Left Handle
        glPushMatrix();
            glTranslatef(-.05,.05,0.2);
            glRotatef(-45,0,1,0);
            glRotatef(15,1,0,0);
            gluCylinder(quadric,0.02f,0.02f,0.1f,32,32);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

    glPopMatrix();
}

//This method draws the part of the chassy that holds up the engine, gas tank and seat
void  KatBike::DrawEngineSupport(GLfloat translateX, GLfloat translateY, GLfloat translateZ){

    glPushMatrix();
        //gluQuadricDrawStyle(quadric, GLU_FILL);
        glColor3f(1, 1, 1);
    
        glTranslatef(translateX,translateY,translateZ);
        glRotatef(90,1,0,0);
        glRotatef(-52.5,0,1,0);	
        tm->BindTextureByIndex(2);
        gluCylinder(quadric,0.02f,0.02f,0.2f,32,32);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

//This method draws the part of the chassy that holds up the engine, gas tank and seat
void  KatBike::DrawOtherEngineSupport(GLfloat translateX, GLfloat translateY, GLfloat translateZ){

    glPushMatrix();
        //gluQuadricDrawStyle(quadric, GLU_FILL);
        glColor3f(1, 1, 1);
    
        glTranslatef(translateX,translateY,translateZ);
        glRotatef(90,1,0,0);
        glRotatef(52.5,0,1,0);
        tm->BindTextureByIndex(2);
        gluCylinder(quadric,0.02f,0.02f,0.2f,32,32);
        glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

//This method draws the trailor hitch for the motercycle.
void  KatBike::DrawTrailerHitch(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
    glPushMatrix();

        glTranslatef(translateX,translateY,translateZ);
            glColor3f(1,1,1);
            gluQuadricDrawStyle(quadric, GLU_FILL);
    

        //Top Horizontal Bar
        glPushMatrix();
            glRotatef(90,0,1,0);
            tm->BindTextureByIndex(2);
            gluCylinder(quadric,0.018f,0.018f,0.1f,32,32);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        //Middle Bar
        glPushMatrix();
            glTranslatef(0.09,-0.01,0);
            glRotatef(-90,1,0,0);
            tm->BindTextureByIndex(2);
            gluCylinder(quadric,0.02f,0.02f,0.18f,32,32);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        //Bottum
        glPushMatrix();
            glTranslatef(0.09,0.15,0);
            glRotatef(90,0,1,0);
            tm->BindTextureByIndex(0);
            gluCylinder(quadric,0.02f,0.02f,0.35f,32,32);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

    glPopMatrix();
}

//This method is charged with drawing the trailor for the motercycle.
void  KatBike::DrawTrailer(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
    glPushMatrix();
        glTranslatef(translateX,translateY,translateZ);
        applyColor();
            glColor3f(1,1,1);
            tm->BindTextureByIndex(9); 
            gluSphere(quadric, 0.2, 32, 32);
            glDisable(GL_TEXTURE_2D);
            glPushMatrix();
            glTranslatef(0.09,-0.03,0);
            glRotatef(90,0,1,0);
            glRotatef(21,1,0,0);
            tm->BindTextureByIndex(9);
            gluCylinder(quadric,0.18,0.0,0.4,32,32);
            glDisable(GL_TEXTURE_2D);
        setMaterial();
    glPopMatrix();
    
    glPopMatrix();
}

//This method is responsible for drawing the motercycle seat.
void  KatBike::DrawSeat(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
    glPushMatrix();
    glTranslatef(translateX,translateY,translateZ);
    applyColor();
        glColor3f(1.0,1.0, 1.0);
        tm->BindTextureByIndex(9);
        gluSphere(quadric, 0.1, 32, 32);
        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
            glTranslatef(0.05,-0.0075,0);
            glRotatef(90,0,1,0);
            glRotatef(21,1,0,0);
            tm->BindTextureByIndex(9);
            gluCylinder(quadric, 0.08, 0.0, 0.25, 32, 32);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    setMaterial();
    glPopMatrix();
}

//This method is responsible for drawing the motercycle seat.
void  KatBike::DrawGasTank(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
    glPushMatrix();
    glTranslatef(translateX,translateY,translateZ);
    applyColor();
        glColor3f(1.0, 1.0, 1.0);
        tm->BindTextureByIndex(9); 
        gluSphere(quadric, 0.077, 32, 32);
        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
            glTranslatef(-0.05,-0.01,0);
            glRotatef(-90,0,1,0);
            glRotatef(15,1,0,0);
            tm->BindTextureByIndex(9); 
            gluCylinder(quadric,0.06,0.0,0.2,32,32);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    setMaterial();
    glPopMatrix();
}

//This method is responsible for drawing the engine.
void  KatBike::DrawEngine(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
    glPushMatrix();
    glTranslatef(translateX,translateY,translateZ);
    glColor3f(0.70, 0.70 , 0.70);

        glutSolidCube(.15);
        glPushMatrix();
            glTranslatef(-0.15,0,0);
            glutSolidCube(.15);
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-0.25,0,0);
            glutSolidCube(.15);
        glPopMatrix();
    
    glPopMatrix();
}

//This method is responsible for drawing the exhaust pipe.
void  KatBike::DrawExhaustPipe(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
    glPushMatrix();
        glColor3f(1,1,1);
        //gluQuadricDrawStyle(quadric, GLU_FILL);
    
        glTranslatef(translateX, translateY, translateZ);
    
        glRotatef(-90,0,1,0);
    
        tm->BindTextureByIndex(2);
        gluCylinder(quadric,0.02f,0.02f,0.4f,32,32);
        glDisable(GL_TEXTURE_2D);

        glPushMatrix();
            glTranslatef(0,0,0.3);
            tm->BindTextureByIndex(2);
            gluCylinder(quadric,0.03f,0.03f,0.45f,32,32);
            glDisable(GL_TEXTURE_2D);
            if (alpha > 0)
            {
                glPushMatrix();
                    glDisable(GL_LIGHTING);
                    glRotatef(180, 0, 1, 0);
                    glScalef(0.3, 0.23, 0.3);
                    glTranslatef(0, 0.9, -1.5);
                    sm->draw(0.07);
                    glEnable(GL_LIGHTING);
                glPopMatrix();
            }
        glPopMatrix();

    glPopMatrix();
}

//This method is responsible for drawing the front light
void  KatBike::DrawFrontLight(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{	
    glPushMatrix();
        glTranslatef(translateX,translateY,translateZ);
        glColor3f(1, 1, 1);

        glutSolidSphere(0.04, 32, 32);
        //gluQuadricDrawStyle(quadric, GLU_FILL);			

        glPushMatrix();
            glTranslatef(0.01,0,0);
            glRotatef(-90,0,1,0);	
            tm->BindTextureByIndex(9);
            gluCylinder(quadric,0.045f,0.028f,0.08f,32,32);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();	
    glPopMatrix();
}

//This method is responsible for drawing the back light
void  KatBike::DrawBackLight(GLfloat translateX, GLfloat translateY, GLfloat translateZ)
{
    glPushMatrix();
        glTranslatef(translateX,translateY,translateZ);
        glColor3f(1, 0, 0);
        glutSolidSphere(0.03, 32, 32);
        //gluQuadricDrawStyle(quadric, GLU_FILL);
        glColor3f(1, 1, 1);

        glPushMatrix();
            glTranslatef(0.01,0,0);
            glRotatef(90,0,1,0);
            tm->BindTextureByIndex(9);
            gluCylinder(quadric,0.035f,0.018f,0.07f,32,32);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    glPopMatrix();
}
>>>>>>> first
