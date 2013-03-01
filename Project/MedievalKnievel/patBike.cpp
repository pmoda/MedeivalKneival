<<<<<<< HEAD
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <strstream>
#include <iomanip>
#include <string>

#include "patBike.h"
#include "bike.h"

GLint list[3];

PatBike::PatBike()
{
    init();
    quadric = gluNewQuadric();
}

PatBike::PatBike(TextureManager* t, LightManager* l, SmokeManager* s)
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

PatBike::~PatBike()
{
}

void PatBike::init()
{
    heading = 0;
    bikePosX = 0;
    bikePosZ = 0;
    velocityZ = 0;
    velocityX = 0;
    wheelRotationDegree = 0;
    tireRad = 0.25;
    materialType = 0;    
    turnCounter = 5;
    green();
}

void PatBike::draw()
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

        glTranslated(bikePosX, 0.28, bikePosZ);
        glRotatef(heading, 0, 1, 0);
        
        glScalef(0.5,0.5,0.5);
        tm->BindTextureByIndex(2);   
        drawEngine(0.55, 0.45, 0.50, 0.4, 0.75);
        glDisable(GL_TEXTURE_2D);

        glPushMatrix();// Connect Front2Back
            glColor3f(0.6, 0.6, 0.65);
            glTranslatef(0.55, 0, 0);
            glRotatef(90, 0, 1, 0);
            glRotatef(-70, 1, 0, 0);
            drawCylinder(0.075, 0.075, 1.25, 75, 75);
        glPopMatrix();

        glPushMatrix();// Gas Tank

            glColor3f(0.4, 0.2, 0.2);
            glTranslatef(-0.6, 0.35, 0.0);
            glScalef(0.62,0.42, 0.42);
         //glEnable(GL_TEXTURE_2D); 
         //   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
         //   glBindTexture(GL_TEXTURE_2D, texture[5]); 
            
            tm->BindTextureByIndex(5);   
                glutSolidCube(1.0);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
                
        drawSeat();

        glPushMatrix();
            drawTrailerHitch();
        glPopMatrix();
        
        //Draw the lower back frame
        drawBottomBackFrame();

        glPushMatrix();
            glTranslatef(-1.4, 0, 0.0);
            drawTire(11);
        glPopMatrix();				

            //Start front frame
        glPushMatrix();
            glTranslatef(1.7, 0, 0.0);
            glRotatef(15, 0, 0, 1);
            //if (heading < -225.0f)
            //{
            //    glRotatef(-45.0f,0,1,0);
            //}
            //else if (heading < -179.0f)
            //{
            //    glRotatef(heading + 180.0f,0,1,0);
            //}
            //else if (heading < -45.0f)
            //{
            //    glRotatef(-45.0f,0,1,0);
            //}
            //else
            //{
            //    glRotatef(heading,0,1,0);
            //}

            glPushMatrix();	
                drawTire(11);
            glPopMatrix();
            drawFrontFrame();
            glTranslatef(-0.01, 1.2, 0.0);
            glPushMatrix();
                drawHead();					
            glPopMatrix();
        glPopMatrix();	
    

        glPushMatrix();
            glTranslatef(1.0f, 1.6f, 0.0f);
            glRotatef(45, 0.0f, 1.0f, 0.0f);
            if (okToDrawSpear)
                drawSpear(0.4, 0, 5.00, 75, 75);
        glPopMatrix();
    glPopMatrix();
    glDisable(GL_COLOR_MATERIAL);
    
}

void PatBike::move()
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
	bikePosX -= velocityX * cos(heading * PI/180);
    bikePosZ += velocityX * sin(heading * PI/180);
	wheelRotationDegree += (velocityX/circ) * 360  * cos(heading) ;
	//bikePosX -= rotationToDistance;
	if(wheelRotationDegree > ALLROUND)
	{wheelRotationDegree -= ALLROUND;}
}

void PatBike::drawEngine(double topBase, double bottomBase, double backHeight, double width, double frontHeight)
{
    glPushMatrix();
        
    glTranslatef(0.12,0.05,0);
    glColor3d(0.7, 0.8, 0.9);
        glBegin(GL_POLYGON);
             glTexCoord2f(0.0, 0.0);glVertex3d(-bottomBase, 0, -width);
             glTexCoord2f(1.0, 0.0);glVertex3d(-bottomBase, 0, width);
             glTexCoord2f(1.0, 1.0);glVertex3d(bottomBase, 0, width);
             glTexCoord2f(0.0, 1.0);glVertex3d(bottomBase, 0, -width);
        glEnd();
        glBegin(GL_POLYGON);
            glTexCoord2f(0.0, 0.0);glVertex3d(-topBase, backHeight, -width);
            glTexCoord2f(1.0, 0.0);glVertex3d(-topBase, backHeight, width);
            glTexCoord2f(1.0, 1.0);glVertex3d(topBase, frontHeight, width);
            glTexCoord2f(0.0, 1.0);glVertex3d(topBase, frontHeight, -width);
        glEnd();
        glBegin(GL_POLYGON);
            glTexCoord2f(0.0, 0.0);glVertex3d(-topBase, backHeight, width);
            glTexCoord2f(8.0, 0.0);glVertex3d(-topBase, backHeight, -width);
            glTexCoord2f(8.0, 8.0);glVertex3d(-bottomBase, 0, -width);	
            glTexCoord2f(0.0, 8.0);glVertex3d(-bottomBase, 0, width);
        glEnd();
        glBegin(GL_POLYGON);		
            glTexCoord2f(0.0, 0.0);glVertex3d(topBase, frontHeight, width);
            glTexCoord2f(1.0, 0.0);glVertex3d(topBase, frontHeight, -width);
            glTexCoord2f(1.0, 1.0);glVertex3d(bottomBase, 0, -width);
            glTexCoord2f(0.0, 1.0);glVertex3d(bottomBase, 0, width);
        glEnd();
        
        tm->BindTextureByIndex(3); 
       // glBindTexture(GL_TEXTURE_2D, texture[3]);    
        glBegin(GL_POLYGON);		
            glTexCoord2f(1.0, 1.0);glVertex3d(topBase, frontHeight, width);
            glTexCoord2f(0.0, 1.0);glVertex3d(-topBase, backHeight, width);
            glTexCoord2f(0.0, 0.0);glVertex3d(-bottomBase, 0, width);
            glTexCoord2f(1.0, 0.0);glVertex3d(bottomBase, 0, width);
        glEnd();
        glBegin(GL_POLYGON);		
            glTexCoord2f(1.0, 1.0);glVertex3d(topBase, frontHeight, -width);
            glTexCoord2f(0.0, 1.0);glVertex3d(-topBase, backHeight, -width);
            glTexCoord2f(0.0, 0.0);glVertex3d(-bottomBase, 0, -width);
            glTexCoord2f(1.0, 0.0);glVertex3d(bottomBase, 0,-width);
        glEnd();
        
    glPopMatrix();
}

void PatBike::drawCylinder(GLdouble base, GLdouble top, GLdouble height, GLdouble slices, GLdouble stacks)
{
    //cylinder with nice disks to cap off
    slices = slices/3;
    stacks = stacks/3;
    gluCylinder(quadric, base, top, height, slices, stacks); 
    glRotatef(180, 1,0,0); 
    gluDisk(quadric, 0.0f, base, slices, 1);
    glRotatef(180, 1,0,0); 
    glTranslatef(0.0f, 0.0f, height); 
    gluDisk(quadric, 0.0f, top, slices, 1);
    glTranslatef(0.0f, 0.0f, -height);
}

void PatBike::drawSeat()
{
    glPushMatrix();// SEAT
    glEnable(GL_NORMALIZE);
        glColor3f(0.201, 0.251, 0.280);
        glTranslatef(-0.8, 0.66, 0.0);			
        glPushMatrix();//Back of seat
            glTranslatef(-0.45, 0.32, 0.0);
            glScalef(0.22,0.22, 0.42);
            glutSolidCube(1.0);
            glPushMatrix();
                glTranslatef(-1,0,0);
                glScalef(2.22,2.22, 2.22);
                glRotatef(180, 0, 1, 0);
                glPushMatrix(); //light cast
                    glColor3f(1,0.3,0.5);
                    glRotatef(-90.0, 0.0, 1.0, 0.0);
                    glutSolidCone(0.25, 0.5, 50, 50);
                glPopMatrix();
                glPushMatrix(); //light bulb
                
            //glEnable(GL_BLEND);
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            //glEnable(GL_ALPHA_TEST);
                    glColor3f(1.0,0.8,0.9);
                    glScalef(0.1, 0.2, 0.2);

                    if(braking)
                    {
                        GLfloat emission[] = { 1, 0, 0, alpha };					
                        glMaterialfv(GL_FRONT, GL_EMISSION, emission);
                        glutSolidSphere(1.0, 50, 50);
                        GLfloat light_position2[] = { 0, 0, 0, 1.0 };
                        GLfloat light_direction2[] = { 10.0, 0, 0, 1 };		
                        //glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
                        //glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_direction2);

                        GLfloat emission1[] = { 0, 0, 0, alpha };					
                        glMaterialfv(GL_FRONT, GL_EMISSION, emission1);
                        //glEnable(GL_LIGHT2);
                    }
                    else
                    {
                        glutSolidSphere(1.0, 50, 50);
                    }
                    
            //glDisable(GL_ALPHA_TEST);
            //glDisable(GL_BLEND);
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
        glScalef(0.82,0.40, 0.4);
        glRotatef(180, 1, 0, 0);		
        glRotatef(7, 0, 0, 1);
        glColor3f(0.2,0.1,0.1);


        glutSolidTeapot(0.9);//seat

        glDisable(GL_NORMALIZE);
    glPopMatrix();
}

void PatBike::drawTrailerHitch()
{
    glPushMatrix();
        glTranslatef(-3.2, 0.45, 0);
        glRotatef(-5,0,0,1);		
        glPushMatrix();//WHeels
            glTranslatef(-1.25, -0.58, -0.7);
            glPushMatrix();
                drawTire(12);
            glPopMatrix();
            glTranslatef(0,0,1.4);
            glPushMatrix();
                drawTire(12);
            glPopMatrix();
            glTranslatef(0,0,-1.6);						
            glColor3f(0.31,0.1,1.0);//Rear Axl
            drawCylinder(0.075, 0.075, 1.8, 75, 75);
            glRotatef(180,0,1,0);
            glutSolidCone(0.075, 0.1, 25, 25);
            glRotatef(-180,0,1,0);			
            glTranslatef(0,0,1.8);
            glutSolidCone(0.075, 0.1, 25, 25);
        glPopMatrix();
        glScalef(0.9,0.7,0.7);
        //glRotatef(180, 1, 0, 0);
        glColor3f(0.39,0.1,0.1);
        applyColor();
            glutSolidTeapot(1.0); //front of hitch
            glTranslatef(-0.9, 0.25, 0);
            glScalef(1.9,0.7,1);		
            glScalef(1,1.5,1);	
            gluSphere( quadric, 1,70,70);
        setMaterial();
    glPopMatrix();
}

void PatBike::drawBottomBackFrame()
{
	glPushMatrix();
		glColor3f(1,1,0);
		glTranslatef(-1.9, 0.0, -0.30);
		glRotatef (90.0 , 0.0, 1.0, 0.0);
		gluQuadricDrawStyle(quadric, GLU_FILL);
		gluQuadricOrientation(quadric, GLU_INSIDE);
		
        if (alpha > 0)
        {
            glPushMatrix();
                glDisable(GL_LIGHTING);
                glTranslatef(0, 0.9, 0);
                sm->draw(0.05);
                glEnable(GL_LIGHTING);
            glPopMatrix();
        }

        glColor3f(0.7, 0.7, 0.7);
        float high_shininess[] = {500.0f * alpha};
        glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
        drawCylinder(0.1, 0.1, 2.5, 75, 75); //left exhaust

        //glColor3f(1,0,0);
        glRotatef (-90.0 , 0.0, 1.0, 0.0);
        glTranslatef(0.0, 0.0, 0.6);
        glRotatef (90.0 , 0.0, 1.0, 0.0);
        //glRotatef (90.0 , 0.0, 1.0, 0.0);

        if (alpha > 0)
        {
            glPushMatrix();
                glDisable(GL_LIGHTING);
                glTranslatef(0, 0.9, 0);
                sm->draw(0.05);
                glEnable(GL_LIGHTING);
            glPopMatrix();
        }

        glColor3f(0.7, 0.7, 0.7);
        glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
        drawCylinder(0.1, 0.1, 2.5, 75, 75);//right exhaust

        glRotatef (90.0 , 0.0, 1.0, 0.0);
        glColor3f(1,0,1);
        glTranslatef(-0.50, 0, 0);
        drawCylinder(0.075, 0.075, 0.6, 75, 75);
        glTranslatef(-2.0, 0, 0);
        drawCylinder(0.075, 0.075, 0.6, 75, 75);
    glPopMatrix();
}

void PatBike::drawTire(int spokes)
{
        // Rotation about X from UP-DOWN Arrow key
        //glRotatef(beta, 1, 0, 0);
        // Rotation about Y from idle function.
    glRotatef(wheelRotationDegree, 0, 0, 1);

    glColor3f(0, 0, 0);
    glutSolidTorus(0.1, 0.5, 25, 25);//WHEEL radius 0.5!
    
    //gluCylinder(quadric, 0.1, 0.1, 0.2, 25, 25);
    
    glutSolidTorus(0.03, 0.08, 25, 25);
    
    int currentSpokeA = 0;
    int spokeA = 360/spokes;
    
    for(int i = 0; i < spokes ; i++){
        glPushMatrix();
            glRotatef(currentSpokeA, 0.0, 0.0, 1.0);
            glRotatef(-90, 1.0, 0.0, 0.0);
            glScalef(0.1, 0.1, 0.4);
            //glColor3f(1, 1, 1);
            glTranslatef(0.0, 0.0, 0.25);
            drawSpoke();

        glPopMatrix();
            currentSpokeA += spokeA;
            if (currentSpokeA > 360 )
                break;
    }
}

void PatBike::drawSpoke()
{				
    if(velocityX < -10 || velocityX > 10)
    {
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    //glEnable(GL_ALPHA_TEST);
    }
        glColor3f(0.7, 0.7, 0.7);
        float high_shininess[] = {500.0f * alpha};
        glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
        //draw a spoke
        gluCylinder(quadric, 0.3, 0.3, 0.75, 10, 10);
    //glDisable(GL_ALPHA_TEST);
    //glDisable(GL_BLEND);
}

void PatBike::drawFrontFrame()
{
    glColor3f(0.7, 0.7, 0.7);
    float high_shininess[] = {500.0f * alpha};
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    glPushMatrix();
        glTranslatef(0.0, 0.0, -0.3);
        drawCylinder(0.07, 0.07, 0.6, 75, 75);//bearing in front
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.0, 0.0, 0.30);		 
        glRotatef (20.0 , 0.0, 0.0, 1.0);
        glRotatef (-90.0 , 1.0, 0.0, 0.0);	
        drawCylinder(0.09, 0.09, 1.25, 75, 75);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.0, 0.0, -0.30);		 
        glRotatef (20.0 , 0.0, 0.0, 1.0);			 
        glRotatef (-90.0 , 1.0, 0.0, 0.0);
        drawCylinder(0.09, 0.09, 1.25, 75, 75);
    glPopMatrix();
    glPushMatrix();
        glColor3d(1,0,0);
        glTranslatef(-0.1, 0.70, 0);
        glRotatef(7, 0, 0, 1);
        glScalef(0.55,0.2,0.25);		
        //tm->BindTextureByIndex(4, quadric);
        applyColor();
                gluSphere( quadric, 1,70,70);
        setMaterial();

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_CULL_FACE);
    //glutSolidSphere(1,20,20);
    glPopMatrix();
}

void PatBike::drawHead()
{
    glRotatef(-20, 0, 0, 1);
    glPushMatrix(); //light cast
        glRotatef(-90.0, 0.0, 1.0, 0.0);
        glutSolidCone(0.25, 0.5, 50, 50);
    glPopMatrix();
    glPushMatrix(); //light bulb
        glColor3f(1.0,1.0,1.0);
        glScalef(0.1, 0.2, 0.2);
        
            //glEnable(GL_BLEND);
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            //glEnable(GL_ALPHA_TEST);
        if(lm->light2)
        {
            GLfloat emission[] = { 1, 1, 0.6, alpha };					
            glMaterialfv(GL_FRONT, GL_EMISSION, emission);
            glutSolidSphere(1.0, 50, 50);
            GLfloat light_position1[] = { 0, 0, 0, 1.0 };
            GLfloat light_direction[] = { 10, -1, 0, 1 };	
            //glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
            //glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_direction);

            GLfloat emission1[] = { 0, 0, 0, alpha };					
            glMaterialfv(GL_FRONT, GL_EMISSION, emission1);
            //glEnable(GL_LIGHT1);
        }		
        else
        {
            GLfloat emission[] = { 0.5, 0.5, 1, alpha };					
            glMaterialfv(GL_FRONT, GL_EMISSION, emission);
            glutSolidSphere(1.0, 50, 50);
            GLfloat light_position1[] = { 0, 0, 0, 1.0 };
            GLfloat light_direction[] = {20, -1, 0, 1 };	
            //glLightfv(GL_LIGHT5, GL_POSITION, light_position1);
            //glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, light_direction);

            GLfloat emission1[] = { 0, 0, 0, alpha };					
            glMaterialfv(GL_FRONT, GL_EMISSION, emission1);
            //glEnable(GL_LIGHT5);
        }
            //glDisable(GL_ALPHA_TEST);
            //glDisable(GL_BLEND);
        //else
        //{
        //	glutSolidSphere(1.0, 50, 50);
        //}
    glPopMatrix();
    glColor3f(1,0,0.5);
    glTranslatef(-0.3, 0.0, 0.0);
    glPushMatrix(); // light support panel			
        applyColor();	
            glScalef(0.1, 0.2, 0.35);
            glutSolidSphere(1.5, 50, 50);
        setMaterial();
    glPopMatrix();

    glTranslatef(-0.15, 0.15, -0.75);
    glPushMatrix(); // Handle Bar main

        glColor3f(0.2, 0.2, 0.2);
    //glColor3f(0,0,0.5);
        glRotatef(-90.0, 0.0, 0.0, 1.0);
        drawCylinder(0.12, 0.12, 1.5, 75, 75);

        glTranslatef(0.0, 0.0, 0.07);
        //glColor3f(0,0.5,0.1);
        glPushMatrix();// left handle bar
            glRotatef(-90.0, 0.0, 1.0, 0.0);
            glRotatef(20.0, 1.0, 0.0, 0.0);
            drawCylinder(0.08, 0.08, 0.5, 75, 75);
            glTranslatef(0.0, 0.0, 0.45);	
            glRotatef(-90, 0, 1, 0);
            //glRotatef(-40, 1, 0, 0);
            glRotatef(40, 1, 0, 0);
            drawCylinder(0.07, 0.07, 0.5, 75, 75);
        glPopMatrix();

        glTranslatef(0.0, 0.0, 1.36);
        glPushMatrix();// right handle bar
            glRotatef(-90.0, 0.0, 1.0, 0.0);
            glRotatef(20.0, 1.0, 0.0, 0.0);
            drawCylinder(0.08, 0.08, 0.5, 75, 75);
            glTranslatef(0.0, 0.0, 0.45);	
            glRotatef(90, 0, 1, 0);
            //glRotatef(-40, 1, 0, 0);
            glRotatef(40, 1, 0, 0);
            drawCylinder(0.07, 0.07, 0.5, 75, 75);
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

#include "patBike.h"
#include "bike.h"

GLint list[3];

PatBike::PatBike()
{
    init();
    quadric = gluNewQuadric();
}

PatBike::PatBike(TextureManager* t, LightManager* l, SmokeManager* s)
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

PatBike::~PatBike()
{
}

void PatBike::init()
{
    heading = 0;
    bikePosX = 0;
    bikePosZ = 0;
    velocityZ = 0;
    velocityX = 0;
    wheelRotationDegree = 0;
    tireRad = 0.25;
    materialType = 0;    
    turnCounter = 5;
    green();
}

void PatBike::draw()
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

        glTranslated(bikePosX, 0.28, bikePosZ);
        glRotatef(heading, 0, 1, 0);
        
        glScalef(0.5,0.5,0.5);
        tm->BindTextureByIndex(2);   
        drawEngine(0.55, 0.45, 0.50, 0.4, 0.75);
        glDisable(GL_TEXTURE_2D);

        glPushMatrix();// Connect Front2Back
            glColor3f(0.6, 0.6, 0.65);
            glTranslatef(0.55, 0, 0);
            glRotatef(90, 0, 1, 0);
            glRotatef(-70, 1, 0, 0);
            drawCylinder(0.075, 0.075, 1.25, 75, 75);
        glPopMatrix();

        glPushMatrix();// Gas Tank

            glColor3f(0.4, 0.2, 0.2);
            glTranslatef(-0.6, 0.35, 0.0);
            glScalef(0.62,0.42, 0.42);
         //glEnable(GL_TEXTURE_2D); 
         //   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
         //   glBindTexture(GL_TEXTURE_2D, texture[5]); 
            
            tm->BindTextureByIndex(5);   
                glutSolidCube(1.0);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
                
        drawSeat();

        glPushMatrix();
            drawTrailerHitch();
        glPopMatrix();
        
        //Draw the lower back frame
        drawBottomBackFrame();

        glPushMatrix();
            glTranslatef(-1.4, 0, 0.0);
            drawTire(11);
        glPopMatrix();				

            //Start front frame
        glPushMatrix();
            glTranslatef(1.7, 0, 0.0);
            glRotatef(15, 0, 0, 1);
            //if (heading < -225.0f)
            //{
            //    glRotatef(-45.0f,0,1,0);
            //}
            //else if (heading < -179.0f)
            //{
            //    glRotatef(heading + 180.0f,0,1,0);
            //}
            //else if (heading < -45.0f)
            //{
            //    glRotatef(-45.0f,0,1,0);
            //}
            //else
            //{
            //    glRotatef(heading,0,1,0);
            //}

            glPushMatrix();	
                drawTire(11);
            glPopMatrix();
            drawFrontFrame();
            glTranslatef(-0.01, 1.2, 0.0);
            glPushMatrix();
                drawHead();					
            glPopMatrix();
        glPopMatrix();	
    

        glPushMatrix();
            glTranslatef(1.0f, 1.6f, 0.0f);
            glRotatef(45, 0.0f, 1.0f, 0.0f);
            if (okToDrawSpear)
                drawSpear(0.4, 0, 5.00, 75, 75);
        glPopMatrix();
    glPopMatrix();
    glDisable(GL_COLOR_MATERIAL);
    
}

void PatBike::move()
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
	bikePosX -= velocityX * cos(heading * PI/180);
    bikePosZ += velocityX * sin(heading * PI/180);
	wheelRotationDegree += (velocityX/circ) * 360  * cos(heading) ;
	//bikePosX -= rotationToDistance;
	if(wheelRotationDegree > ALLROUND)
	{wheelRotationDegree -= ALLROUND;}
}

void PatBike::drawEngine(double topBase, double bottomBase, double backHeight, double width, double frontHeight)
{
    glPushMatrix();
        
    glTranslatef(0.12,0.05,0);
    glColor3d(0.7, 0.8, 0.9);
        glBegin(GL_POLYGON);
             glTexCoord2f(0.0, 0.0);glVertex3d(-bottomBase, 0, -width);
             glTexCoord2f(1.0, 0.0);glVertex3d(-bottomBase, 0, width);
             glTexCoord2f(1.0, 1.0);glVertex3d(bottomBase, 0, width);
             glTexCoord2f(0.0, 1.0);glVertex3d(bottomBase, 0, -width);
        glEnd();
        glBegin(GL_POLYGON);
            glTexCoord2f(0.0, 0.0);glVertex3d(-topBase, backHeight, -width);
            glTexCoord2f(1.0, 0.0);glVertex3d(-topBase, backHeight, width);
            glTexCoord2f(1.0, 1.0);glVertex3d(topBase, frontHeight, width);
            glTexCoord2f(0.0, 1.0);glVertex3d(topBase, frontHeight, -width);
        glEnd();
        glBegin(GL_POLYGON);
            glTexCoord2f(0.0, 0.0);glVertex3d(-topBase, backHeight, width);
            glTexCoord2f(8.0, 0.0);glVertex3d(-topBase, backHeight, -width);
            glTexCoord2f(8.0, 8.0);glVertex3d(-bottomBase, 0, -width);	
            glTexCoord2f(0.0, 8.0);glVertex3d(-bottomBase, 0, width);
        glEnd();
        glBegin(GL_POLYGON);		
            glTexCoord2f(0.0, 0.0);glVertex3d(topBase, frontHeight, width);
            glTexCoord2f(1.0, 0.0);glVertex3d(topBase, frontHeight, -width);
            glTexCoord2f(1.0, 1.0);glVertex3d(bottomBase, 0, -width);
            glTexCoord2f(0.0, 1.0);glVertex3d(bottomBase, 0, width);
        glEnd();
        
        tm->BindTextureByIndex(3); 
       // glBindTexture(GL_TEXTURE_2D, texture[3]);    
        glBegin(GL_POLYGON);		
            glTexCoord2f(1.0, 1.0);glVertex3d(topBase, frontHeight, width);
            glTexCoord2f(0.0, 1.0);glVertex3d(-topBase, backHeight, width);
            glTexCoord2f(0.0, 0.0);glVertex3d(-bottomBase, 0, width);
            glTexCoord2f(1.0, 0.0);glVertex3d(bottomBase, 0, width);
        glEnd();
        glBegin(GL_POLYGON);		
            glTexCoord2f(1.0, 1.0);glVertex3d(topBase, frontHeight, -width);
            glTexCoord2f(0.0, 1.0);glVertex3d(-topBase, backHeight, -width);
            glTexCoord2f(0.0, 0.0);glVertex3d(-bottomBase, 0, -width);
            glTexCoord2f(1.0, 0.0);glVertex3d(bottomBase, 0,-width);
        glEnd();
        
    glPopMatrix();
}

void PatBike::drawCylinder(GLdouble base, GLdouble top, GLdouble height, GLdouble slices, GLdouble stacks)
{
    //cylinder with nice disks to cap off
    slices = slices/3;
    stacks = stacks/3;
    gluCylinder(quadric, base, top, height, slices, stacks); 
    glRotatef(180, 1,0,0); 
    gluDisk(quadric, 0.0f, base, slices, 1);
    glRotatef(180, 1,0,0); 
    glTranslatef(0.0f, 0.0f, height); 
    gluDisk(quadric, 0.0f, top, slices, 1);
    glTranslatef(0.0f, 0.0f, -height);
}

void PatBike::drawSeat()
{
    glPushMatrix();// SEAT
    glEnable(GL_NORMALIZE);
        glColor3f(0.201, 0.251, 0.280);
        glTranslatef(-0.8, 0.66, 0.0);			
        glPushMatrix();//Back of seat
            glTranslatef(-0.45, 0.32, 0.0);
            glScalef(0.22,0.22, 0.42);
            glutSolidCube(1.0);
            glPushMatrix();
                glTranslatef(-1,0,0);
                glScalef(2.22,2.22, 2.22);
                glRotatef(180, 0, 1, 0);
                glPushMatrix(); //light cast
                    glColor3f(1,0.3,0.5);
                    glRotatef(-90.0, 0.0, 1.0, 0.0);
                    glutSolidCone(0.25, 0.5, 50, 50);
                glPopMatrix();
                glPushMatrix(); //light bulb
                
            //glEnable(GL_BLEND);
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            //glEnable(GL_ALPHA_TEST);
                    glColor3f(1.0,0.8,0.9);
                    glScalef(0.1, 0.2, 0.2);

                    if(braking)
                    {
                        GLfloat emission[] = { 1, 0, 0, alpha };					
                        glMaterialfv(GL_FRONT, GL_EMISSION, emission);
                        glutSolidSphere(1.0, 50, 50);
                        GLfloat light_position2[] = { 0, 0, 0, 1.0 };
                        GLfloat light_direction2[] = { 10.0, 0, 0, 1 };		
                        //glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
                        //glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_direction2);

                        GLfloat emission1[] = { 0, 0, 0, alpha };					
                        glMaterialfv(GL_FRONT, GL_EMISSION, emission1);
                        //glEnable(GL_LIGHT2);
                    }
                    else
                    {
                        glutSolidSphere(1.0, 50, 50);
                    }
                    
            //glDisable(GL_ALPHA_TEST);
            //glDisable(GL_BLEND);
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
        glScalef(0.82,0.40, 0.4);
        glRotatef(180, 1, 0, 0);		
        glRotatef(7, 0, 0, 1);
        glColor3f(0.2,0.1,0.1);


        glutSolidTeapot(0.9);//seat

        glDisable(GL_NORMALIZE);
    glPopMatrix();
}

void PatBike::drawTrailerHitch()
{
    glPushMatrix();
        glTranslatef(-3.2, 0.45, 0);
        glRotatef(-5,0,0,1);		
        glPushMatrix();//WHeels
            glTranslatef(-1.25, -0.58, -0.7);
            glPushMatrix();
                drawTire(12);
            glPopMatrix();
            glTranslatef(0,0,1.4);
            glPushMatrix();
                drawTire(12);
            glPopMatrix();
            glTranslatef(0,0,-1.6);						
            glColor3f(0.31,0.1,1.0);//Rear Axl
            drawCylinder(0.075, 0.075, 1.8, 75, 75);
            glRotatef(180,0,1,0);
            glutSolidCone(0.075, 0.1, 25, 25);
            glRotatef(-180,0,1,0);			
            glTranslatef(0,0,1.8);
            glutSolidCone(0.075, 0.1, 25, 25);
        glPopMatrix();
        glScalef(0.9,0.7,0.7);
        //glRotatef(180, 1, 0, 0);
        glColor3f(0.39,0.1,0.1);
        applyColor();
            glutSolidTeapot(1.0); //front of hitch
            glTranslatef(-0.9, 0.25, 0);
            glScalef(1.9,0.7,1);		
            glScalef(1,1.5,1);	
            gluSphere( quadric, 1,70,70);
        setMaterial();
    glPopMatrix();
}

void PatBike::drawBottomBackFrame()
{
	glPushMatrix();
		glColor3f(1,1,0);
		glTranslatef(-1.9, 0.0, -0.30);
		glRotatef (90.0 , 0.0, 1.0, 0.0);
		gluQuadricDrawStyle(quadric, GLU_FILL);
		gluQuadricOrientation(quadric, GLU_INSIDE);
		
        if (alpha > 0)
        {
            glPushMatrix();
                glDisable(GL_LIGHTING);
                glTranslatef(0, 0.9, 0);
                sm->draw(0.05);
                glEnable(GL_LIGHTING);
            glPopMatrix();
        }

        glColor3f(0.7, 0.7, 0.7);
        float high_shininess[] = {500.0f * alpha};
        glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
        drawCylinder(0.1, 0.1, 2.5, 75, 75); //left exhaust

        //glColor3f(1,0,0);
        glRotatef (-90.0 , 0.0, 1.0, 0.0);
        glTranslatef(0.0, 0.0, 0.6);
        glRotatef (90.0 , 0.0, 1.0, 0.0);
        //glRotatef (90.0 , 0.0, 1.0, 0.0);

        if (alpha > 0)
        {
            glPushMatrix();
                glDisable(GL_LIGHTING);
                glTranslatef(0, 0.9, 0);
                sm->draw(0.05);
                glEnable(GL_LIGHTING);
            glPopMatrix();
        }

        glColor3f(0.7, 0.7, 0.7);
        glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
        drawCylinder(0.1, 0.1, 2.5, 75, 75);//right exhaust

        glRotatef (90.0 , 0.0, 1.0, 0.0);
        glColor3f(1,0,1);
        glTranslatef(-0.50, 0, 0);
        drawCylinder(0.075, 0.075, 0.6, 75, 75);
        glTranslatef(-2.0, 0, 0);
        drawCylinder(0.075, 0.075, 0.6, 75, 75);
    glPopMatrix();
}

void PatBike::drawTire(int spokes)
{
        // Rotation about X from UP-DOWN Arrow key
        //glRotatef(beta, 1, 0, 0);
        // Rotation about Y from idle function.
    glRotatef(wheelRotationDegree, 0, 0, 1);

    glColor3f(0, 0, 0);
    glutSolidTorus(0.1, 0.5, 25, 25);//WHEEL radius 0.5!
    
    //gluCylinder(quadric, 0.1, 0.1, 0.2, 25, 25);
    
    glutSolidTorus(0.03, 0.08, 25, 25);
    
    int currentSpokeA = 0;
    int spokeA = 360/spokes;
    
    for(int i = 0; i < spokes ; i++){
        glPushMatrix();
            glRotatef(currentSpokeA, 0.0, 0.0, 1.0);
            glRotatef(-90, 1.0, 0.0, 0.0);
            glScalef(0.1, 0.1, 0.4);
            //glColor3f(1, 1, 1);
            glTranslatef(0.0, 0.0, 0.25);
            drawSpoke();

        glPopMatrix();
            currentSpokeA += spokeA;
            if (currentSpokeA > 360 )
                break;
    }
}

void PatBike::drawSpoke()
{				
    if(velocityX < -10 || velocityX > 10)
    {
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    //glEnable(GL_ALPHA_TEST);
    }
        glColor3f(0.7, 0.7, 0.7);
        float high_shininess[] = {500.0f * alpha};
        glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
        //draw a spoke
        gluCylinder(quadric, 0.3, 0.3, 0.75, 10, 10);
    //glDisable(GL_ALPHA_TEST);
    //glDisable(GL_BLEND);
}

void PatBike::drawFrontFrame()
{
    glColor3f(0.7, 0.7, 0.7);
    float high_shininess[] = {500.0f * alpha};
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    glPushMatrix();
        glTranslatef(0.0, 0.0, -0.3);
        drawCylinder(0.07, 0.07, 0.6, 75, 75);//bearing in front
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.0, 0.0, 0.30);		 
        glRotatef (20.0 , 0.0, 0.0, 1.0);
        glRotatef (-90.0 , 1.0, 0.0, 0.0);	
        drawCylinder(0.09, 0.09, 1.25, 75, 75);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.0, 0.0, -0.30);		 
        glRotatef (20.0 , 0.0, 0.0, 1.0);			 
        glRotatef (-90.0 , 1.0, 0.0, 0.0);
        drawCylinder(0.09, 0.09, 1.25, 75, 75);
    glPopMatrix();
    glPushMatrix();
        glColor3d(1,0,0);
        glTranslatef(-0.1, 0.70, 0);
        glRotatef(7, 0, 0, 1);
        glScalef(0.55,0.2,0.25);		
        //tm->BindTextureByIndex(4, quadric);
        applyColor();
                gluSphere( quadric, 1,70,70);
        setMaterial();

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_CULL_FACE);
    //glutSolidSphere(1,20,20);
    glPopMatrix();
}

void PatBike::drawHead()
{
    glRotatef(-20, 0, 0, 1);
    glPushMatrix(); //light cast
        glRotatef(-90.0, 0.0, 1.0, 0.0);
        glutSolidCone(0.25, 0.5, 50, 50);
    glPopMatrix();
    glPushMatrix(); //light bulb
        glColor3f(1.0,1.0,1.0);
        glScalef(0.1, 0.2, 0.2);
        
            //glEnable(GL_BLEND);
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            //glEnable(GL_ALPHA_TEST);
        if(lm->light2)
        {
            GLfloat emission[] = { 1, 1, 0.6, alpha };					
            glMaterialfv(GL_FRONT, GL_EMISSION, emission);
            glutSolidSphere(1.0, 50, 50);
            GLfloat light_position1[] = { 0, 0, 0, 1.0 };
            GLfloat light_direction[] = { 10, -1, 0, 1 };	
            //glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
            //glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_direction);

            GLfloat emission1[] = { 0, 0, 0, alpha };					
            glMaterialfv(GL_FRONT, GL_EMISSION, emission1);
            //glEnable(GL_LIGHT1);
        }		
        else
        {
            GLfloat emission[] = { 0.5, 0.5, 1, alpha };					
            glMaterialfv(GL_FRONT, GL_EMISSION, emission);
            glutSolidSphere(1.0, 50, 50);
            GLfloat light_position1[] = { 0, 0, 0, 1.0 };
            GLfloat light_direction[] = {20, -1, 0, 1 };	
            //glLightfv(GL_LIGHT5, GL_POSITION, light_position1);
            //glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, light_direction);

            GLfloat emission1[] = { 0, 0, 0, alpha };					
            glMaterialfv(GL_FRONT, GL_EMISSION, emission1);
            //glEnable(GL_LIGHT5);
        }
            //glDisable(GL_ALPHA_TEST);
            //glDisable(GL_BLEND);
        //else
        //{
        //	glutSolidSphere(1.0, 50, 50);
        //}
    glPopMatrix();
    glColor3f(1,0,0.5);
    glTranslatef(-0.3, 0.0, 0.0);
    glPushMatrix(); // light support panel			
        applyColor();	
            glScalef(0.1, 0.2, 0.35);
            glutSolidSphere(1.5, 50, 50);
        setMaterial();
    glPopMatrix();

    glTranslatef(-0.15, 0.15, -0.75);
    glPushMatrix(); // Handle Bar main

        glColor3f(0.2, 0.2, 0.2);
    //glColor3f(0,0,0.5);
        glRotatef(-90.0, 0.0, 0.0, 1.0);
        drawCylinder(0.12, 0.12, 1.5, 75, 75);

        glTranslatef(0.0, 0.0, 0.07);
        //glColor3f(0,0.5,0.1);
        glPushMatrix();// left handle bar
            glRotatef(-90.0, 0.0, 1.0, 0.0);
            glRotatef(20.0, 1.0, 0.0, 0.0);
            drawCylinder(0.08, 0.08, 0.5, 75, 75);
            glTranslatef(0.0, 0.0, 0.45);	
            glRotatef(-90, 0, 1, 0);
            //glRotatef(-40, 1, 0, 0);
            glRotatef(40, 1, 0, 0);
            drawCylinder(0.07, 0.07, 0.5, 75, 75);
        glPopMatrix();

        glTranslatef(0.0, 0.0, 1.36);
        glPushMatrix();// right handle bar
            glRotatef(-90.0, 0.0, 1.0, 0.0);
            glRotatef(20.0, 1.0, 0.0, 0.0);
            drawCylinder(0.08, 0.08, 0.5, 75, 75);
            glTranslatef(0.0, 0.0, 0.45);	
            glRotatef(90, 0, 1, 0);
            //glRotatef(-40, 1, 0, 0);
            glRotatef(40, 1, 0, 0);
            drawCylinder(0.07, 0.07, 0.5, 75, 75);
        glPopMatrix();
    glPopMatrix();
}
>>>>>>> first
