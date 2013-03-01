<<<<<<< HEAD
#include <stdlib.h>

#include <GL/glut.h>
#include <iostream>
#include <strstream>
#include <iomanip>
#include <math.h>
#include "Scenery.h"

#define PI 3.14159265

void Scenery::drawCylinder(GLdouble base, GLdouble top, GLdouble height, GLdouble slices, GLdouble stacks, int scene)
{	
	//cylinder with nice disks to cap off
	slices = slices;
	stacks = stacks;
	gluCylinder(quadric, base, top, height, slices, stacks); 
	glRotatef(180, 1,0,0); 
	gluDisk(quadric, 0.0f, base, slices, 1);
	glRotatef(180, 1,0,0); 
	glTranslatef(0.0f, 0.0f, height); 
	gluDisk(quadric, 0.0f, top, slices, 1); 
	glTranslatef(0.0f, 0.0f, -height);
}

void Scenery::drawCube()
{
	glBegin(GL_QUADS);
		// front
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		// back
		glVertex3f(0.0f, 0.0f, -1.0f);
		glVertex3f(1.0f, 0.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(0.0f, 1.0f, -1.0f);
		// right
		glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		// left
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, -1.0f);
		glVertex3f(0.0f, 1.0f, -1.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		// top
		glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(0.0f, 1.0f, -1.0f);
		// bottom
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, -1.0f);
		glVertex3f(0.0f, 0.0f, -1.0f);
	glEnd();
}

void Scenery::drawColumn()
{
	glPushMatrix();	
		glColor3f(0.90, 0.85, 0.65);
		glTranslatef(0, -0.5, 1.2);
		glPushMatrix();	//Bottom Part
			glPushMatrix();	
				glTranslatef(0, 0, -0.25);
				glRotatef(-45, 0, 0, 1);
				//drawCube();
				drawCylinder(0.5, 0.5, 0.5, 4, 4, 0);
			glPopMatrix();
		glPopMatrix();
		glTranslatef(0, 0.25, 0);		
		glPushMatrix();	
			glRotatef(-90, 1, 0, 0);
			gluCylinder(quadric, 0.25, 0.25, 5, 8, 8);
		glPopMatrix();
		glTranslatef(0,-1,0);
		glPushMatrix();	//Top Part
			glRotatef(90, 0, 1, 0);
			glPushMatrix();				
				glRotatef(-45, 0, 0, 1);
				glTranslatef(0,0,-0.5);
				drawCylinder(0.5, 0.5, 4.3, 4, 4, 0);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

void Scenery::drawSkyrim()
{
	glPushMatrix();	//Top Part
		glTranslatef(0,5.5,1.2);
		glPushMatrix();		
			glRotatef(90, 0, 1, 0);		
			glRotatef(-45, 0, 0, 1);
			drawCylinder(0.6, 0.6, 4.3, 4, 4, 0);
		glPopMatrix();		
		glTranslatef(0,-0.71,0);
		glPushMatrix();		
			glTranslatef(0, 0, -0.25);		
			glRotatef(-45, 0, 0, 1);
			//glTranslatef(1, 0, 0);	
			drawCylinder(0.5, 0.5, 0.5, 4, 4, 0);
			glTranslatef(2, -1, 0);				
			drawCylinder(0.3, 0.3, 0.5, 4, 4, 0);
			glTranslatef(1, 1, 0);	
			drawCylinder(0.3, 0.3, 0.5, 4, 4, 0);
			glTranslatef(1, 1, 0);	
			drawCylinder(0.3, 0.3, 0.5, 4, 4, 0);
		glPopMatrix();
	glPopMatrix();
}

void Scenery::drawArch(float x1, float x2)
{
	tm->BindTextureByIndex(5);
	float circleRadius = 1.0;
	bool first = true;
	bool last = false;
	glPushMatrix();

		// right sides
		glPushMatrix();

			glBegin(GL_TRIANGLE_STRIP);
	   // glColor3f(1, 0, 0);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				//if(first)
				// {
				first = false;
				glTexCoord2f(1.0, i%2);glVertex3f(0, i, 0);
				glTexCoord2f(0.0, i%2);glVertex3f(0, i, circleRadius);
/*                }
				else if(i == x1 + x2)
				{  */              
				  //  glTexCoord2f(1.0, 1.0);glVertex3f(0, i, 0);
				  //  glTexCoord2f(0.0, 1.0);glVertex3f(0, i, circleRadius);
				//}
				//else
				//{
				 //   glVertex3f(0, i, 0);
				   // glVertex3f(0, i, circleRadius);
			   // }
			}
			glEnd();

			glTranslatef(2 * circleRadius + 1, 0, 0);

			glBegin(GL_TRIANGLE_STRIP);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				 glTexCoord2f(1.0, i%2);glVertex3f(0, i, 0);
				glTexCoord2f(0.0, i%2);glVertex3f(0, i, circleRadius);
			}
			glEnd();

		glPopMatrix();

		glTranslatef(1.0f, 0.0f, 0.0f);

		// left sides
		glPushMatrix();
			glBegin(GL_TRIANGLE_STRIP);
	   // glColor3f(0, 1, 0);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				 glTexCoord2f(0.0, i%2);glVertex3f(0, i, circleRadius);
				 glTexCoord2f(1.0, i%2);glVertex3f(0, i, 0);
			}
			glEnd();

			glTranslatef(2 * circleRadius + 1, 0, 0);

			glBegin(GL_TRIANGLE_STRIP);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				 glTexCoord2f(0.0, i%2);glVertex3f(0, i, circleRadius);
				 glTexCoord2f(1.0, i%2);glVertex3f(0, i, 0);
			}

			glEnd();

		glPopMatrix();

	glPopMatrix();

	glPushMatrix();

		// front sides
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, 1.0f);
			glBegin(GL_TRIANGLE_STRIP);

		 //   glColor3f(0, 0, 1);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				 glTexCoord2f(0.0, i%2);glVertex3f(0, i, 0);
				 glTexCoord2f(1, i%2);glVertex3f(circleRadius, i, 0);
			}
			glEnd();

			glTranslatef(2 * circleRadius + 1, 0, 0);

			glBegin(GL_TRIANGLE_STRIP);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				 glTexCoord2f(0.0, i%2);glVertex3f(0, i, 0);
				 glTexCoord2f(1, i%2);glVertex3f(circleRadius, i, 0);
			}
			glEnd();

		glPopMatrix();

		// back sides
		glPushMatrix();
			glBegin(GL_TRIANGLE_STRIP);

	   //     glColor3f(1, 0, 1);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				 glTexCoord2f(0.0, i%2);glVertex3f(circleRadius, i, 0);
				glTexCoord2f(1, i%2);glVertex3f(0, i, 0);
			}
			glEnd();

			glTranslatef(2 * circleRadius + 1, 0, 0);

			glBegin(GL_TRIANGLE_STRIP);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				 glTexCoord2f(0.0, i%2);glVertex3f(circleRadius, i, 0);
				 glTexCoord2f(1, i%2);glVertex3f(0, i, 0);
			}

			glEnd();

		glPopMatrix();

	glPopMatrix();

	//makin the middle
	glPushMatrix();

		glBegin(GL_TRIANGLE_STRIP);
	   // glColor3f(1, 1, 1);
		for(int i = 0 ; i <= circleRadius * 4; i+=LOD)
		{		
			 glTexCoord2f(0.0, i%2);glVertex3f(i, x1 + x2, 0);
			 glTexCoord2f(1, i%2);glVertex3f(i, x1 + x2, circleRadius);
		}
		glEnd();
		
		float step = 0.5;
		float start = PI;
		float end = 0;
		float angle = start;
		
		glBegin(GL_TRIANGLE_STRIP);
		
	  //  glColor3f(1, 0.5, 0.5);
			while(angle >= end)
			{
				 glTexCoord2f(0.0, ((int)(angle*10) % 10)/5);glVertex3f(cos(angle) + 2, sin(angle) + x1,1);
				 glTexCoord2f(1, ((int)(angle*10) % 10)/5);glVertex3f(cos(angle) + 2, sin(angle) + x1, 0);
				angle -= step;
			}
			
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);

			while(angle <= start)
			{
				 glTexCoord2f(0.0, ((int)(angle*10) % 10)/5);glVertex3f(cos(angle) + 2, x1+x2, 0);
				 glTexCoord2f(1, ((int)(angle*10) % 10)/5);glVertex3f(cos(angle) + 2, sin(angle) + x1,0);
				angle += step;
			}
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);
			while(angle >= end)
			{
				 glTexCoord2f(0.0, ((int)(angle*10) % 10)/5);glVertex3f(cos(angle) + 2, x1+x2, 1);
				 glTexCoord2f(1, ((int)(angle*10) % 10)/5);glVertex3f(cos(angle) + 2, sin(angle) + x1,1);
				angle -= step;
			}
		glEnd();

	glPopMatrix();    
}

void Scenery::drawColosseumRing(float amountOfArches, int x1, int x2, bool half, int layer)
{
	//float step = 2 * atan(2/ringRadius);
	float step = 2 * PI / amountOfArches;
	float ringRadius = 2 / tan(step / 2);
	float end = 2 * PI;

	if (half)
	{        
		end = PI - (layer * step);
	}

	float start = 0;
	if(half)
	{
		start = layer * step;
	}    

	for (float sphereRad = start; sphereRad < end; sphereRad += step)
	{
		glPushMatrix();
			glTranslatef(ringRadius*cos(sphereRad), 0, ringRadius*sin(sphereRad));
			glPushMatrix();
				glRotatef(90, 0.0, 1.0, 0.0);
				glRotatef(-(sphereRad - step / 2)*(360/(2*PI)),0,1,0);
				drawArch(x1, x2);
				drawColumn();
				if(x1 == 1)
					drawSkyrim();
			glPopMatrix();
		glPopMatrix();
	}
}

void Scenery::drawColosseum(int x1, int x2)
{
	int layers = 4;
	float startCount = 50;
	glPushMatrix();
		glScalef(1.3, 1.0, 1.0);
		for (int i = 0; i < layers; ++i)
		{
			if (i == layers - 1)
			{
				drawColosseumRing(startCount, 1, x1 + x2 - 1, true, i);
				glTranslatef(0, x1 + x2, 0);
			}
			else
			{
				drawColosseumRing(startCount, x1, x2, true, i);
				glTranslatef(0, x1 + x2, 0);
			}
		}
	glPopMatrix();	
	layers = 3;
	glPushMatrix();
		glScalef(1.3, 1.0, 1.0);
		glScaled(0.8,0.8,0.8); 
		for (int i = 0; i < layers; ++i)
		{
			if (i == layers - 1)
			{
				drawColosseumRing(startCount, 0, x1 + x2, false, i);
				glTranslatef(0, x1 + x2, 0);
			}
			else
			{
				drawColosseumRing(startCount, x1, x2, false, i);
				glTranslatef(0, x1 + x2, 0);
			}
		}
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
}

void Scenery::drawWoodPlank(float height, float base)
{
	int x1 = height;
	int x2 = 0;
	tm->BindTextureByIndex(8);
	//float base = base;
	bool first = true;
	bool last = false;
	glPushMatrix();

		// right sides
		glPushMatrix();
			glBegin(GL_TRIANGLE_STRIP);
	   // glColor3f(1, 0, 0);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				first = false;
				glTexCoord2f(1.0, i%2);glVertex3f(0, i, 0);
				glTexCoord2f(0.0, i%2);glVertex3f(0, i, base);
			}
			glEnd();
		glPopMatrix();

		glTranslatef(base, 0.0f, 0.0f);

		// left sides
		glPushMatrix();
			glBegin(GL_TRIANGLE_STRIP);
	   // glColor3f(0, 1, 0);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				 glTexCoord2f(0.0, i%2);glVertex3f(0, i, base);
				 glTexCoord2f(1.0, i%2);glVertex3f(0, i, 0);
			}
			glEnd();
		glPopMatrix();
	glPopMatrix();

	glPushMatrix();
		// front sides
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, base);
			glBegin(GL_TRIANGLE_STRIP);

		 //   glColor3f(0, 0, 1);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				 glTexCoord2f(0.0, i%2);glVertex3f(0, i, 0);
				 glTexCoord2f(1, i%2);glVertex3f(base, i, 0);
			}
			glEnd();
		glPopMatrix();

		// back sides
		glPushMatrix();
			glBegin(GL_TRIANGLE_STRIP);

	   //     glColor3f(1, 0, 1);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				 glTexCoord2f(0.0, i%2);glVertex3f(base, i, 0);
				glTexCoord2f(1, i%2);glVertex3f(0, i, 0);
			}
			glEnd();
		glPopMatrix();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void Scenery::drawFence()
{
	glPushMatrix();
	glScaled(1,0.9,1);
		glTranslatef(-20,0.9,-0.1);
		glRotatef(-90,0,0,1);
		glPushMatrix();
			glTranslatef(0.4,0,0);
			glScaled(2,1,0.5);
			drawWoodPlank(40, 0.1);
			glTranslatef(0,0,0.3);
			drawWoodPlank(40, 0.1);
		glPopMatrix();	
		//top of fence
		drawWoodPlank(40, 0.2);
		glRotatef(90,0,0,1);
		glTranslatef(0,-1,0.05);
		for(int i = -20; i <= 20; i++)
		{			
			drawWoodPlank(1, 0.1);
			glTranslatef(1,0,0);
		}
	glPopMatrix();
	
}

void Scenery::InnerRoad()
{
	//Draw road
	glPushMatrix();
		glScaled(1.5,1.1,1.1);
		glColor3f(0.2, 0.2, 0.2);
		glRotatef(-90, 1, 0, 0);
		gluDisk(quadric, 0, 23, 22,22);
		
		tm->BindTextureByIndex(7);
		glColor3f(1, 1, 1);
		glTranslated(0,0,0.02);
		glScaled(0.75,0.21,0.21);
		//glColor3f(0.2, 0.2, 0.2);
		//gluDisk(quadric, 0, 21, 55,55);
	glPopMatrix();	
}

void Scenery::OuterRoad()
{	
	tm->BindTextureByIndex(4);
	glPushMatrix();	
	//glTranslated(0,0,-0.1);
		glScaled(1.5,1.1,1.1);
		glColor3f(0.2, 0.2, 0.2);
		glRotatef(-90, 1, 0, 0);        
		glTranslated(0,0,0.04);
		gluDisk(quadric, 23, 30, 12,12);
	glPopMatrix();
}

void Scenery::Grass()
{
	glPushMatrix();			   
		tm->BindTextureByIndex(1);
			glBegin(GL_POLYGON);
				glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, -0.01, 100.0);
				glTexCoord2f(20.0, 0.0); glVertex3f(100.0, -0.01, 100.0);
				glTexCoord2f(20.0, 20.0); glVertex3f(100.0, -0.01, -100.0);
				glTexCoord2f(0.0, 20.0); glVertex3f(-100.0, -0.01, -100.0);
			glEnd();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();	

}

void Scenery::Moon()
{
	tm->BindTextureByIndex(11);
	glPushMatrix();
		glTranslatef(55,70,-55);
		glColor3f(1,1,0.7);
			GLfloat emission[] = { 0.6, 0.6, 0.4, 1.0 };				
			float mat_ambient[] = {0.3f, 0.3f, 0.2f, 1.0f};
			float mat_ambient_color[] = {0.2f, 0.2f, 0.2f, 1.0f};
			float mat_diffuse[] = {0.25f, 0.5f, 0.60f, 1.0f};
			float mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
			float low_shininess[] = {15.0f};

			glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
			//glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
			glMaterialfv(GL_FRONT, GL_EMISSION, emission);
			gluSphere(quadric, 12.0, 50, 50);
			
			GLfloat emission1[] = { 0, 0, 0, 0.0 };					
			glMaterialfv(GL_FRONT, GL_EMISSION, emission1);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void Scenery::DrawStar()
{
	glPushMatrix();
	glRotated(90, 0,0,1);
		glColor3f(1,1,0.7);
		GLfloat emission[] = { 1, 1, 1.0, 1.0 };				
		float mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
		float mat_ambient_color[] = {0.8f, 0.8f, 0.2f, 1.0f};
		float mat_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
		float mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
		float low_shininess[] = {25.0f};

		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		//glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);

		//int starCount  = 10;
		for(int i = 0; i < sizeof(stars)/sizeof(stars[0]); i++)
		{		
			if(stars[i].initial && clock() % 180 > 0){
				stars[i].starPitch = rand() % 180;
				stars[i].starYaw = rand()  % 180;
				stars[i].size = rand()  % 300;
				stars[i].initial = false;
			}
			glPushMatrix();
				glRotatef(-stars[i].starPitch, 1, 0, 0);
				glRotatef(stars[i].starYaw, 0, 1, 0);
				//glRotatef(stars[i].starYaw - 90, 0,0,1);
				glTranslatef(/*-(i % 2) * stars[i].starYaw*/0 ,0,100);
				glutSolidSphere(stars[i].size / 1000, 5,5);
			glPopMatrix();
		}
		GLfloat emission1[] = { 0, 0, 0, 0.0 };					
		glMaterialfv(GL_FRONT, GL_EMISSION, emission1);
	glPopMatrix();
}

Scenery::Scenery(TextureManager* texman, GLUquadric* quad)
{	
	tm = texman;
	quadric = quad;
	LOD = 1;
	init = false;
}

void Scenery::drawColosseumShadow()
{
	glPushMatrix();
        //Enable alpha stuff
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glAlphaFunc(GL_GREATER,0.2);
        glEnable(GL_ALPHA_TEST);
        float mat_ambient[] = {0, 0, 0, 0.3};
        float mat_diffuse[] = {0, 0, 0, 0.3};
        float mat_ambient2[] = {1, 1, 1, 1};
        float mat_diffuse2[] = {1, 1, 1, 1};

        //Set material properties
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

        
		glScaled(1.5,1.1,1.1);
		glColor3f(0.7, 0.7, 0.7);

        //Draw ombra
        glPushMatrix();
        glTranslatef(-1,0,5);
		glRotatef(-90, 1, 0, 0);       
		glTranslated(0,0,0.01);
		gluDisk(quadric, 23, 29, 32, 32);
        glPopMatrix();

        //Draw penombra
        glPushMatrix();
        glTranslatef(-2,0,5);
		glRotatef(-90, 1, 0, 0);       
		glTranslated(0,0,0.03);
		gluDisk(quadric, 21, 30, 32, 32);
        glPopMatrix();

        //reset material properties
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient2);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);

        //Disable alpha stuff
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);
	glPopMatrix();

}

void Scenery::draw()
{	 //DRAW SCENERY
	
	glEnable(GL_NORMALIZE);
	glPushMatrix();	
	
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);		
		
		//Draw Moon -1
		Moon();

		//Draw the stars - 3
		DrawStar();

		//Draw grass - 1
		Grass();
		drawFence();

		//Draw road	1
		tm->BindTextureByIndex(6);
        
		InnerRoad();        
        drawColosseumShadow();
        OuterRoad();
        
		glDisable(GL_TEXTURE_2D);

		//Draw the colosseum - 30
		drawColosseum(3, 3);
		glTranslatef(0,6,0);
		//drawColumn();
		//glScaled(0.8,0.8,0.8);        
		//drawColosseum(4, 3);
	glPopMatrix();
	
	//Hills -1
	glPushMatrix();
		glTranslatef(50,-5,-40);
		glColor3f(1,1,1);
		glPushMatrix();
			tm->BindTextureByIndex(1);
			//glColor3f(0.0, 1.0, 0.0);
			gluSphere( quadric, 12,30,30);

			glScaled(2,1,2);
			glTranslatef(-50,0,-5);
			gluSphere( quadric, 12,30,30);

			glScaled(4,1,2);
			glTranslatef(5,-2,35);
			gluSphere( quadric, 12,30,30);

			glDisable(GL_TEXTURE_2D);
			glDisable(GL_CULL_FACE);
		glPopMatrix();
		glTranslatef(50,4,0);
		glColor3f(1,1,1);			

	glPopMatrix();
	//END OF SCENERY
	
	glDisable(GL_NORMALIZE);
}
=======
#include <stdlib.h>

#include <GL/glut.h>
#include <iostream>
#include <strstream>
#include <iomanip>
#include <math.h>
#include "Scenery.h"

#define PI 3.14159265

void Scenery::drawCylinder(GLdouble base, GLdouble top, GLdouble height, GLdouble slices, GLdouble stacks, int scene)
{	
	//cylinder with nice disks to cap off
	slices = slices;
	stacks = stacks;
	gluCylinder(quadric, base, top, height, slices, stacks); 
	glRotatef(180, 1,0,0); 
	gluDisk(quadric, 0.0f, base, slices, 1);
	glRotatef(180, 1,0,0); 
	glTranslatef(0.0f, 0.0f, height); 
	gluDisk(quadric, 0.0f, top, slices, 1); 
	glTranslatef(0.0f, 0.0f, -height);
}

void Scenery::drawCube()
{
	glBegin(GL_QUADS);
		// front
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		// back
		glVertex3f(0.0f, 0.0f, -1.0f);
		glVertex3f(1.0f, 0.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(0.0f, 1.0f, -1.0f);
		// right
		glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		// left
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, -1.0f);
		glVertex3f(0.0f, 1.0f, -1.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		// top
		glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(0.0f, 1.0f, -1.0f);
		// bottom
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, -1.0f);
		glVertex3f(0.0f, 0.0f, -1.0f);
	glEnd();
}

void Scenery::drawColumn()
{
	glPushMatrix();	
		glColor3f(0.90, 0.85, 0.65);
		glTranslatef(0, -0.5, 1.2);
		glPushMatrix();	//Bottom Part
			glPushMatrix();	
				glTranslatef(0, 0, -0.25);
				glRotatef(-45, 0, 0, 1);
				//drawCube();
				drawCylinder(0.5, 0.5, 0.5, 4, 4, 0);
			glPopMatrix();
		glPopMatrix();
		glTranslatef(0, 0.25, 0);		
		glPushMatrix();	
			glRotatef(-90, 1, 0, 0);
			gluCylinder(quadric, 0.25, 0.25, 5, 8, 8);
		glPopMatrix();
		glTranslatef(0,-1,0);
		glPushMatrix();	//Top Part
			glRotatef(90, 0, 1, 0);
			glPushMatrix();				
				glRotatef(-45, 0, 0, 1);
				glTranslatef(0,0,-0.5);
				drawCylinder(0.5, 0.5, 4.3, 4, 4, 0);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

void Scenery::drawSkyrim()
{
	glPushMatrix();	//Top Part
		glTranslatef(0,5.5,1.2);
		glPushMatrix();		
			glRotatef(90, 0, 1, 0);		
			glRotatef(-45, 0, 0, 1);
			drawCylinder(0.6, 0.6, 4.3, 4, 4, 0);
		glPopMatrix();		
		glTranslatef(0,-0.71,0);
		glPushMatrix();		
			glTranslatef(0, 0, -0.25);		
			glRotatef(-45, 0, 0, 1);
			//glTranslatef(1, 0, 0);	
			drawCylinder(0.5, 0.5, 0.5, 4, 4, 0);
			glTranslatef(2, -1, 0);				
			drawCylinder(0.3, 0.3, 0.5, 4, 4, 0);
			glTranslatef(1, 1, 0);	
			drawCylinder(0.3, 0.3, 0.5, 4, 4, 0);
			glTranslatef(1, 1, 0);	
			drawCylinder(0.3, 0.3, 0.5, 4, 4, 0);
		glPopMatrix();
	glPopMatrix();
}

void Scenery::drawArch(float x1, float x2)
{
	tm->BindTextureByIndex(5);
	float circleRadius = 1.0;
	bool first = true;
	bool last = false;
	glPushMatrix();

		// right sides
		glPushMatrix();

			glBegin(GL_TRIANGLE_STRIP);
	   // glColor3f(1, 0, 0);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				//if(first)
				// {
				first = false;
				glTexCoord2f(1.0, i%2);glVertex3f(0, i, 0);
				glTexCoord2f(0.0, i%2);glVertex3f(0, i, circleRadius);
/*                }
				else if(i == x1 + x2)
				{  */              
				  //  glTexCoord2f(1.0, 1.0);glVertex3f(0, i, 0);
				  //  glTexCoord2f(0.0, 1.0);glVertex3f(0, i, circleRadius);
				//}
				//else
				//{
				 //   glVertex3f(0, i, 0);
				   // glVertex3f(0, i, circleRadius);
			   // }
			}
			glEnd();

			glTranslatef(2 * circleRadius + 1, 0, 0);

			glBegin(GL_TRIANGLE_STRIP);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				 glTexCoord2f(1.0, i%2);glVertex3f(0, i, 0);
				glTexCoord2f(0.0, i%2);glVertex3f(0, i, circleRadius);
			}
			glEnd();

		glPopMatrix();

		glTranslatef(1.0f, 0.0f, 0.0f);

		// left sides
		glPushMatrix();
			glBegin(GL_TRIANGLE_STRIP);
	   // glColor3f(0, 1, 0);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				 glTexCoord2f(0.0, i%2);glVertex3f(0, i, circleRadius);
				 glTexCoord2f(1.0, i%2);glVertex3f(0, i, 0);
			}
			glEnd();

			glTranslatef(2 * circleRadius + 1, 0, 0);

			glBegin(GL_TRIANGLE_STRIP);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				 glTexCoord2f(0.0, i%2);glVertex3f(0, i, circleRadius);
				 glTexCoord2f(1.0, i%2);glVertex3f(0, i, 0);
			}

			glEnd();

		glPopMatrix();

	glPopMatrix();

	glPushMatrix();

		// front sides
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, 1.0f);
			glBegin(GL_TRIANGLE_STRIP);

		 //   glColor3f(0, 0, 1);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				 glTexCoord2f(0.0, i%2);glVertex3f(0, i, 0);
				 glTexCoord2f(1, i%2);glVertex3f(circleRadius, i, 0);
			}
			glEnd();

			glTranslatef(2 * circleRadius + 1, 0, 0);

			glBegin(GL_TRIANGLE_STRIP);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				 glTexCoord2f(0.0, i%2);glVertex3f(0, i, 0);
				 glTexCoord2f(1, i%2);glVertex3f(circleRadius, i, 0);
			}
			glEnd();

		glPopMatrix();

		// back sides
		glPushMatrix();
			glBegin(GL_TRIANGLE_STRIP);

	   //     glColor3f(1, 0, 1);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				 glTexCoord2f(0.0, i%2);glVertex3f(circleRadius, i, 0);
				glTexCoord2f(1, i%2);glVertex3f(0, i, 0);
			}
			glEnd();

			glTranslatef(2 * circleRadius + 1, 0, 0);

			glBegin(GL_TRIANGLE_STRIP);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				 glTexCoord2f(0.0, i%2);glVertex3f(circleRadius, i, 0);
				 glTexCoord2f(1, i%2);glVertex3f(0, i, 0);
			}

			glEnd();

		glPopMatrix();

	glPopMatrix();

	//makin the middle
	glPushMatrix();

		glBegin(GL_TRIANGLE_STRIP);
	   // glColor3f(1, 1, 1);
		for(int i = 0 ; i <= circleRadius * 4; i+=LOD)
		{		
			 glTexCoord2f(0.0, i%2);glVertex3f(i, x1 + x2, 0);
			 glTexCoord2f(1, i%2);glVertex3f(i, x1 + x2, circleRadius);
		}
		glEnd();
		
		float step = 0.5;
		float start = PI;
		float end = 0;
		float angle = start;
		
		glBegin(GL_TRIANGLE_STRIP);
		
	  //  glColor3f(1, 0.5, 0.5);
			while(angle >= end)
			{
				 glTexCoord2f(0.0, ((int)(angle*10) % 10)/5);glVertex3f(cos(angle) + 2, sin(angle) + x1,1);
				 glTexCoord2f(1, ((int)(angle*10) % 10)/5);glVertex3f(cos(angle) + 2, sin(angle) + x1, 0);
				angle -= step;
			}
			
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);

			while(angle <= start)
			{
				 glTexCoord2f(0.0, ((int)(angle*10) % 10)/5);glVertex3f(cos(angle) + 2, x1+x2, 0);
				 glTexCoord2f(1, ((int)(angle*10) % 10)/5);glVertex3f(cos(angle) + 2, sin(angle) + x1,0);
				angle += step;
			}
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);
			while(angle >= end)
			{
				 glTexCoord2f(0.0, ((int)(angle*10) % 10)/5);glVertex3f(cos(angle) + 2, x1+x2, 1);
				 glTexCoord2f(1, ((int)(angle*10) % 10)/5);glVertex3f(cos(angle) + 2, sin(angle) + x1,1);
				angle -= step;
			}
		glEnd();

	glPopMatrix();    
}

void Scenery::drawColosseumRing(float amountOfArches, int x1, int x2, bool half, int layer)
{
	//float step = 2 * atan(2/ringRadius);
	float step = 2 * PI / amountOfArches;
	float ringRadius = 2 / tan(step / 2);
	float end = 2 * PI;

	if (half)
	{        
		end = PI - (layer * step);
	}

	float start = 0;
	if(half)
	{
		start = layer * step;
	}    

	for (float sphereRad = start; sphereRad < end; sphereRad += step)
	{
		glPushMatrix();
			glTranslatef(ringRadius*cos(sphereRad), 0, ringRadius*sin(sphereRad));
			glPushMatrix();
				glRotatef(90, 0.0, 1.0, 0.0);
				glRotatef(-(sphereRad - step / 2)*(360/(2*PI)),0,1,0);
				drawArch(x1, x2);
				drawColumn();
				if(x1 == 1)
					drawSkyrim();
			glPopMatrix();
		glPopMatrix();
	}
}

void Scenery::drawColosseum(int x1, int x2)
{
	int layers = 4;
	float startCount = 50;
	glPushMatrix();
		glScalef(1.3, 1.0, 1.0);
		for (int i = 0; i < layers; ++i)
		{
			if (i == layers - 1)
			{
				drawColosseumRing(startCount, 1, x1 + x2 - 1, true, i);
				glTranslatef(0, x1 + x2, 0);
			}
			else
			{
				drawColosseumRing(startCount, x1, x2, true, i);
				glTranslatef(0, x1 + x2, 0);
			}
		}
	glPopMatrix();	
	layers = 3;
	glPushMatrix();
		glScalef(1.3, 1.0, 1.0);
		glScaled(0.8,0.8,0.8); 
		for (int i = 0; i < layers; ++i)
		{
			if (i == layers - 1)
			{
				drawColosseumRing(startCount, 0, x1 + x2, false, i);
				glTranslatef(0, x1 + x2, 0);
			}
			else
			{
				drawColosseumRing(startCount, x1, x2, false, i);
				glTranslatef(0, x1 + x2, 0);
			}
		}
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
}

void Scenery::drawWoodPlank(float height, float base)
{
	int x1 = height;
	int x2 = 0;
	tm->BindTextureByIndex(8);
	//float base = base;
	bool first = true;
	bool last = false;
	glPushMatrix();

		// right sides
		glPushMatrix();
			glBegin(GL_TRIANGLE_STRIP);
	   // glColor3f(1, 0, 0);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				first = false;
				glTexCoord2f(1.0, i%2);glVertex3f(0, i, 0);
				glTexCoord2f(0.0, i%2);glVertex3f(0, i, base);
			}
			glEnd();
		glPopMatrix();

		glTranslatef(base, 0.0f, 0.0f);

		// left sides
		glPushMatrix();
			glBegin(GL_TRIANGLE_STRIP);
	   // glColor3f(0, 1, 0);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				 glTexCoord2f(0.0, i%2);glVertex3f(0, i, base);
				 glTexCoord2f(1.0, i%2);glVertex3f(0, i, 0);
			}
			glEnd();
		glPopMatrix();
	glPopMatrix();

	glPushMatrix();
		// front sides
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, base);
			glBegin(GL_TRIANGLE_STRIP);

		 //   glColor3f(0, 0, 1);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				 glTexCoord2f(0.0, i%2);glVertex3f(0, i, 0);
				 glTexCoord2f(1, i%2);glVertex3f(base, i, 0);
			}
			glEnd();
		glPopMatrix();

		// back sides
		glPushMatrix();
			glBegin(GL_TRIANGLE_STRIP);

	   //     glColor3f(1, 0, 1);
			for(int i = 0 ; i <= x1 + x2; i+=LOD)
			{		
				 glTexCoord2f(0.0, i%2);glVertex3f(base, i, 0);
				glTexCoord2f(1, i%2);glVertex3f(0, i, 0);
			}
			glEnd();
		glPopMatrix();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void Scenery::drawFence()
{
	glPushMatrix();
	glScaled(1,0.9,1);
		glTranslatef(-20,0.9,-0.1);
		glRotatef(-90,0,0,1);
		glPushMatrix();
			glTranslatef(0.4,0,0);
			glScaled(2,1,0.5);
			drawWoodPlank(40, 0.1);
			glTranslatef(0,0,0.3);
			drawWoodPlank(40, 0.1);
		glPopMatrix();	
		//top of fence
		drawWoodPlank(40, 0.2);
		glRotatef(90,0,0,1);
		glTranslatef(0,-1,0.05);
		for(int i = -20; i <= 20; i++)
		{			
			drawWoodPlank(1, 0.1);
			glTranslatef(1,0,0);
		}
	glPopMatrix();
	
}

void Scenery::InnerRoad()
{
	//Draw road
	glPushMatrix();
		glScaled(1.5,1.1,1.1);
		glColor3f(0.2, 0.2, 0.2);
		glRotatef(-90, 1, 0, 0);
		gluDisk(quadric, 0, 23, 22,22);
		
		tm->BindTextureByIndex(7);
		glColor3f(1, 1, 1);
		glTranslated(0,0,0.02);
		glScaled(0.75,0.21,0.21);
		//glColor3f(0.2, 0.2, 0.2);
		//gluDisk(quadric, 0, 21, 55,55);
	glPopMatrix();	
}

void Scenery::OuterRoad()
{	
	tm->BindTextureByIndex(4);
	glPushMatrix();	
	//glTranslated(0,0,-0.1);
		glScaled(1.5,1.1,1.1);
		glColor3f(0.2, 0.2, 0.2);
		glRotatef(-90, 1, 0, 0);        
		glTranslated(0,0,0.04);
		gluDisk(quadric, 23, 30, 12,12);
	glPopMatrix();
}

void Scenery::Grass()
{
	glPushMatrix();			   
		tm->BindTextureByIndex(1);
			glBegin(GL_POLYGON);
				glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, -0.01, 100.0);
				glTexCoord2f(20.0, 0.0); glVertex3f(100.0, -0.01, 100.0);
				glTexCoord2f(20.0, 20.0); glVertex3f(100.0, -0.01, -100.0);
				glTexCoord2f(0.0, 20.0); glVertex3f(-100.0, -0.01, -100.0);
			glEnd();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();	

}

void Scenery::Moon()
{
	tm->BindTextureByIndex(11);
	glPushMatrix();
		glTranslatef(55,70,-55);
		glColor3f(1,1,0.7);
			GLfloat emission[] = { 0.6, 0.6, 0.4, 1.0 };				
			float mat_ambient[] = {0.3f, 0.3f, 0.2f, 1.0f};
			float mat_ambient_color[] = {0.2f, 0.2f, 0.2f, 1.0f};
			float mat_diffuse[] = {0.25f, 0.5f, 0.60f, 1.0f};
			float mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
			float low_shininess[] = {15.0f};

			glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
			//glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
			glMaterialfv(GL_FRONT, GL_EMISSION, emission);
			gluSphere(quadric, 12.0, 50, 50);
			
			GLfloat emission1[] = { 0, 0, 0, 0.0 };					
			glMaterialfv(GL_FRONT, GL_EMISSION, emission1);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void Scenery::DrawStar()
{
	glPushMatrix();
	glRotated(90, 0,0,1);
		glColor3f(1,1,0.7);
		GLfloat emission[] = { 1, 1, 1.0, 1.0 };				
		float mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
		float mat_ambient_color[] = {0.8f, 0.8f, 0.2f, 1.0f};
		float mat_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
		float mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
		float low_shininess[] = {25.0f};

		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		//glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);

		//int starCount  = 10;
		for(int i = 0; i < sizeof(stars)/sizeof(stars[0]); i++)
		{		
			if(stars[i].initial && clock() % 180 > 0){
				stars[i].starPitch = rand() % 180;
				stars[i].starYaw = rand()  % 180;
				stars[i].size = rand()  % 300;
				stars[i].initial = false;
			}
			glPushMatrix();
				glRotatef(-stars[i].starPitch, 1, 0, 0);
				glRotatef(stars[i].starYaw, 0, 1, 0);
				//glRotatef(stars[i].starYaw - 90, 0,0,1);
				glTranslatef(/*-(i % 2) * stars[i].starYaw*/0 ,0,100);
				glutSolidSphere(stars[i].size / 1000, 5,5);
			glPopMatrix();
		}
		GLfloat emission1[] = { 0, 0, 0, 0.0 };					
		glMaterialfv(GL_FRONT, GL_EMISSION, emission1);
	glPopMatrix();
}

Scenery::Scenery(TextureManager* texman, GLUquadric* quad)
{	
	tm = texman;
	quadric = quad;
	LOD = 1;
	init = false;
}

void Scenery::drawColosseumShadow()
{
	glPushMatrix();
        //Enable alpha stuff
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glAlphaFunc(GL_GREATER,0.2);
        glEnable(GL_ALPHA_TEST);
        float mat_ambient[] = {0, 0, 0, 0.3};
        float mat_diffuse[] = {0, 0, 0, 0.3};
        float mat_ambient2[] = {1, 1, 1, 1};
        float mat_diffuse2[] = {1, 1, 1, 1};

        //Set material properties
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

        
		glScaled(1.5,1.1,1.1);
		glColor3f(0.7, 0.7, 0.7);

        //Draw ombra
        glPushMatrix();
        glTranslatef(-1,0,5);
		glRotatef(-90, 1, 0, 0);       
		glTranslated(0,0,0.01);
		gluDisk(quadric, 23, 29, 32, 32);
        glPopMatrix();

        //Draw penombra
        glPushMatrix();
        glTranslatef(-2,0,5);
		glRotatef(-90, 1, 0, 0);       
		glTranslated(0,0,0.03);
		gluDisk(quadric, 21, 30, 32, 32);
        glPopMatrix();

        //reset material properties
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient2);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);

        //Disable alpha stuff
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);
	glPopMatrix();

}

void Scenery::draw()
{	 //DRAW SCENERY
	
	glEnable(GL_NORMALIZE);
	glPushMatrix();	
	
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);		
		
		//Draw Moon -1
		Moon();

		//Draw the stars - 3
		DrawStar();

		//Draw grass - 1
		Grass();
		drawFence();

		//Draw road	1
		tm->BindTextureByIndex(6);
        
		InnerRoad();        
        drawColosseumShadow();
        OuterRoad();
        
		glDisable(GL_TEXTURE_2D);

		//Draw the colosseum - 30
		drawColosseum(3, 3);
		glTranslatef(0,6,0);
		//drawColumn();
		//glScaled(0.8,0.8,0.8);        
		//drawColosseum(4, 3);
	glPopMatrix();
	
	//Hills -1
	glPushMatrix();
		glTranslatef(50,-5,-40);
		glColor3f(1,1,1);
		glPushMatrix();
			tm->BindTextureByIndex(1);
			//glColor3f(0.0, 1.0, 0.0);
			gluSphere( quadric, 12,30,30);

			glScaled(2,1,2);
			glTranslatef(-50,0,-5);
			gluSphere( quadric, 12,30,30);

			glScaled(4,1,2);
			glTranslatef(5,-2,35);
			gluSphere( quadric, 12,30,30);

			glDisable(GL_TEXTURE_2D);
			glDisable(GL_CULL_FACE);
		glPopMatrix();
		glTranslatef(50,4,0);
		glColor3f(1,1,1);			

	glPopMatrix();
	//END OF SCENERY
	
	glDisable(GL_NORMALIZE);
}
>>>>>>> first
