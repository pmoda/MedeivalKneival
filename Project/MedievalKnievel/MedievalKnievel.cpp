/*
*	Medieval Knievel
*
*   By: Group 8
*   Katrina Anderson		9106251
*   Christopher Di Fulvio	9614605
*   Patrick Modafferi		9401377
*   Matthew Tam		        9675701
*/

// Link with: opengl32.lib, glu32.lib, glut32.lib.
// glut includes links to the other two libraries.
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <strstream>
#include <iomanip>
#include <time.h>
#include <stdio.h>
#include <string>
#include <Windows.h>
#include <vector>

#include "Image.h"
#include "LightManager.h"
#include "TextureManager.h"
#include "SmokeManager.h"
#include "bike.h"
#include "patBike.h"
#include "katBike.h"
#include "matBike.h"
#include "Scenery.h"
#include "Menus.h"
#include "BikeSelect.h"
#include "Game.h"

//#define _ITERATOR_DEBUG_LEVEL 0
#define PI 3.14159265
using namespace std;

// Initial size of graphics window on your screen.
const int WIDTH  = 800; // in pixels
const int HEIGHT = 600; //
const int MID = 45;
const int LOD = 1;
// increment for idle function rotation and zoom
const double RotSTEP = 0.07;
const double ALL_ROUND = 360;
const double ZoomSTEP = 0.5;
const double zoomFactor = 1.03;

// Current size of window (will change when you resize window)
int width  = WIDTH;
int height = HEIGHT;

// Mouse positions, normalized to [0,1].
double xMouse = 0.5;
double yMouse = 0.5;

// Bounds of viewing frustum, maintain uniform scaling.
double viewWindowLeft =  -4;
double viewWindowRight  = 4;
double viewWindowBottom =  -4;
double viewWindowTop  = 4;
double nearPlane =  1;
double farPlane  = 200;
double zoomZ = 0;
double zoomScale = 1;
// Viewing angle.
double fovy = 60;

// Variables.
double alpha = 0;      // rotation angle about Y axis.
double beta = 0;       // rotation angle about X axis.

double wheel = 0;
double heading = 0;
double direction = 0;
double bikePosX = 25;
double bikePosZ = 0;
double velocityZ = 0;
double velocityX = 0;
double savedVelocity = 0;

double nextdirection = 0;
double nextbikePosX = 0;
double nextbikePosZ = 0;

double halfway = - (farPlane + nearPlane) / 2;	   // half way between near and far planes

float cutoff = 40;
float timePrevious = 0.0f;
int idleEnable = 0;			// flags that set continuous rotation on/off
int perspectiveMode = 1;   // flag for proj type; ortho = 0, perspective = 1
bool firstPerson = false;
bool light1 = false;
bool light2 = true;
bool light3 = false;
bool light4 = false;
bool material = true;
bool metal = false;
bool braking = false;
bool headlight = false;

float sphereX;
float sphereY;
float sphereRad = 0;
bool sphereOn = false;

GLUquadric* quadric;

char str[5] = {'F','P','S', '=' , '\0'};
float fps;

// Material property vectors.
float matAmbAndDif[] = {0.0, 0.0, 0.0, 0.0};
float matSpec[] = { 0.0,0.0, 0,0,0.0 };
float matShine[] = { 0.0 };
TextureManager * tm = new TextureManager();
LightManager * lm = new LightManager();

std::vector<Bike*> bikeVector;
Bike* b1;
Bike* b2;
Scenery* scenery;
Menus* menu;
BikeSelect* bikeSelect;
Game* game;

float bikeSelectRotation = 0.0f;
bool gameEnd = false;
bool clash = false;
bool clashSound = false;
bool enginePlayOnce = false;
bool globalWireframe = false;
int numberOfClashes = 0;

void drawCylinder(GLdouble base, GLdouble top, GLdouble height, GLdouble slices, GLdouble stacks)
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

void resetViewParameters()
{
    // resets 3D synthetic camera parameters to default values

    viewWindowLeft =  -4;
    viewWindowRight  = 4;
    viewWindowBottom =  -4;
    viewWindowTop  = 4;
    nearPlane =  1;
    farPlane  = 140;
    fovy = 60;
    alpha = 0.0;
    beta = 0.0;
    perspectiveMode = 1;
    idleEnable = 0;
    zoomZ = 0;
    xMouse = 0.5;
    yMouse = 0.5;
    zoomScale = 1.0;
    heading = 0;
    direction = 0;
    bikePosX = 0;
    bikePosZ = 0;
    velocityZ = 0;
    velocityX = 0;
    savedVelocity = 0;
    //firstPerson = false;
    light1 = false;
    light2 = true;
    light3 = false;
    light4 = false;
    material = true;
    braking = false;
    headlight = false;
    metal = false;
    sphereRad = 0;
    sphereOn = false;
    nextdirection = 0;
    nextbikePosX = 0;
    nextbikePosZ = 0;
}

void zoomInScale(unsigned char direction)
{
    if (direction == '+') {
        zoomScale = zoomScale*zoomFactor;
    }
    else if (direction == '-') {
        zoomScale = zoomScale/zoomFactor;
    }
}

void zoom(unsigned char direction)
{
    if (direction == '+') {
        if(!perspectiveMode){
        }
            zoomZ -= ZoomSTEP;
        }
        else if (direction == '-') {
        if(!perspectiveMode){
        }
            zoomZ += ZoomSTEP;
        }
}

void MoveBike()
{

    float circ = 22 * PI;
    wheel -= velocityX;
    float rotationToDistance = (wheel/360) * circ;
    bikePosX -= rotationToDistance * cos(direction * PI/180);
    bikePosZ += rotationToDistance * sin(direction * PI/180);
    direction = heading;
    if (wheel > ALL_ROUND)
        wheel -= ALL_ROUND;

}

void printString(void* font, char* string)
{
    for(int i = 0; i < 3; i++)
    {		
        glutBitmapCharacter(font, str[i]);
    }
}

void renderBitmapString(float x, float y, void *font, char *string)
{
    char* p;

    glRasterPos2f(x, y);

    for (p = string; *p; p++)
    {				
        glutBitmapCharacter(font, *p);
    }
}

void viewport1()
{
    //TOP VIEWPORT - DISPLAY FPS
glViewport(0,height - MID,width, MID); 

    // set modelling mode
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();	
    
    //	if(perspectiveMode)//{
            gluPerspective(fovy/0.55, GLfloat(width)/GLfloat(height), nearPlane, farPlane);
    //	else
    //		glOrtho(viewWindowLeft, viewWindowRight, viewWindowBottom,	viewWindowTop, nearPlane, farPlane);
        
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        gluLookAt(0, 0, 10,
                    0, 0.0, 0,
                    0.0, 1.0, 0.0);

        char numstring[21];
        sprintf_s(numstring, "%f", fps);

        string a  = "FPS=";
        a += numstring;
   
        char * cstr;
        cstr = new char [a.size()+1];
  strcpy (cstr, a.c_str());


    glColor3b(0,0,0);
    //glutSolidCube(4);
    //	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'F');
    renderBitmapString(-7, 3, GLUT_BITMAP_TIMES_ROMAN_24, cstr);  	
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex3f(-200, 0, 0);
    glVertex3f(200, 0, 0);
    glEnd();
    glLineWidth(1.0);
}

void view2(){

}

void viewMenu()
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();	
    gluPerspective(fovy/0.55, GLfloat(width)/GLfloat(height), nearPlane, farPlane);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gluLookAt(0, 0, 10,
              0, 0.0, 0,
              0.0, 1.0, 0.0);

    glDisable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColor3f(1,1,1);
    glClearColor (0.1, 0.1, 0.1, 0.0);
}

void view()
{
    viewport1();
    view2();

//DEFINE MAIN VIEWPORT
glViewport(0, 0, width, height - MID); 

glMatrixMode(GL_PROJECTION);
    glLoadIdentity();	

        if(perspectiveMode)//{
            gluPerspective(fovy/zoomScale, GLfloat(width)/GLfloat(height), nearPlane, farPlane);
        else
            glOrtho(viewWindowLeft, viewWindowRight, viewWindowBottom,	viewWindowTop, nearPlane, farPlane);
        
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set modelling mode
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();

    float xBack = 0;
    float zBack = 0;
    if(!firstPerson)
    {
        //3RD PERSON VIEW
        //if(velocityX != 0)
        //{
            xBack = 10 * cos(b1->heading * PI / 180);
            zBack = 10 * sin(b1->heading * PI / 180);
        //}
        float zX = zoomZ * cos(b1->heading * PI / 180);
        float zZ = zoomZ * sin(b1->heading * PI / 180);
        float zY = (10 + zoomZ);
            zY *= 0.7;
        gluLookAt(b1->bikePosX - xBack - zX, zY, b1->bikePosZ + zBack + zZ,
                b1->bikePosX, 0.0, b1->bikePosZ,
                0.0, 1.0 /* zMod*/, 0.0);
        
        if(light4)
        {
            GLfloat light_position1[] = { b1->bikePosX - xBack, zY, b1->bikePosZ + zBack + zZ, 1.0 };
            GLfloat light_direction[] = { xBack, -zY,- zBack - zZ, 1.0 };	
            glLightfv(GL_LIGHT7, GL_POSITION, light_position1);
            glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, light_direction);
            //glEnable(GL_LIGHT7);
        }
        else
        {
            //glDisable(GL_LIGHT7);
        }

    }
    else
    {
        //1ST PERSON VIEW
        //if(velocityX != 0)
        //{
            xBack = 1 * cos(b1->heading * PI / 180);
            zBack = 1 * sin(b1->heading * PI / 180);
        //}
        gluLookAt(b1->bikePosX - xBack, 1.7, b1->bikePosZ + zBack,
                bikePosX + xBack, 1.5, b1->bikePosZ - zBack,
                0.0, 1.0 /* zMod*/, 0.0);
    }
    glTranslated(b1->bikePosX, 0, b1->bikePosZ);
    glRotatef(beta, 0.0, 0.0, 1.0);				// Rotate on x
    glRotatef(alpha, 0.0, 1.0, 0.0);			// Rotate on y	
    glTranslated(-b1->bikePosX, 0, -b1->bikePosZ);
    lm->InitializeLights();
}

void playView()
{   
    resetViewParameters();
    viewport1();
    fovy = 60;
  //  alpha = -90;
  //  beta = -15;
    zoomZ = -10.5f; 
    //DEFINE MAIN VIEWPORT    
    glViewport(0, 0, width, height - MID); 

    glMatrixMode(GL_PROJECTION);
     glLoadIdentity();	

    if(perspectiveMode)//{
        gluPerspective(fovy/zoomScale, GLfloat(width)/GLfloat(height), nearPlane, farPlane);
    else
        glOrtho(viewWindowLeft, viewWindowRight, viewWindowBottom,	viewWindowTop, nearPlane, farPlane);
        
    //	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set modelling mode
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();

    float xBack = 0;
    float zBack = 0;

    //3RD PERSON VIEW
    //if(velocityX != 0)
    //{
        xBack = 10 * cos(direction * PI / 180);
        zBack = 10 * sin(direction * PI / 180);
    //}
    float zX = zoomZ * cos(direction * PI / 180);
    float zZ = zoomZ * sin(direction * PI / 180);
    float zY = (10 + zoomZ);
        zY *= 0.7;
            
    if (firstPerson){
        gluLookAt(-22 + b1->bikePosX, 2 , -2,
            22 - b2->bikePosX, 2, 2,
            0.0, 1.0 /* zMod*/, 0.0);
    }
    else{
        gluLookAt(0, 7 , -21,
            0, 0, 0,
            0.0, 1.0 /* zMod*/, 0.0);
    }

    if(light4)
    {
        GLfloat light_position1[] = { bikePosX - xBack, zY, bikePosZ + zBack + zZ, 1.0 };
        GLfloat light_direction[] = { xBack, -zY,- zBack - zZ, 1.0 };	
        glLightfv(GL_LIGHT7, GL_POSITION, light_position1);
        glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, light_direction);
        //glEnable(GL_LIGHT7);
    }
    else
    {
        glDisable(GL_LIGHT7);
    }
    
    
    glRotatef(beta, 0.0, 0.0, 1.0);				// Rotate on x
    glRotatef(alpha, 0.0, 1.0, 0.0);			// Rotate on y	
    
    lm->InitializeLights();  
}

void bikeSelectView()
{
    b1 = bikeSelect->m_playerBike[bikeSelect->playerBikeIndex];

    // view for option text on the left
    glViewport(0, 0, width / 4, height); 

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy/0.55, GLfloat(width / 4)/GLfloat(height), nearPlane, farPlane);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    gluLookAt(0, 0, 10,
                0, 0.0, 0,
                0.0, 1.0, 0.0);
    
    glDisable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColor3f(1,1,1);
    glClearColor (0.1, 0.1, 0.1, 1.0);
    bikeSelect->displayText();

    // view for bike on the right
    glViewport(width / 4 + 5, 0, width * 3 / 4, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, GLfloat(width * 3 / 4 + 5)/GLfloat(height), nearPlane, farPlane);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        gluLookAt(0, 3, 5,
                  b1->bikePosX, 0.0, b1->bikePosZ,
                  0.0, 1.0, 0.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();        
        glPushMatrix(); 
           // glPushMatrix(); 
             //   glTranslatef(0,5,0);
                //glRotatef(90, 1, 0, 0);           
            //    glEnable(GL_LIGHTING);      
              //  glEnable(GL_LIGHT7);
                lm->selectSpotlight();
           // glPopMatrix();          
            glRotatef(bikeSelectRotation, 0, 1.0, 0);
                
           // glEnable(GL_LIGHT7);
            b1->draw();
            glEnable(GL_COLOR_MATERIAL);
            glColor3f(1,1,1);
            glRotatef(90, 1, 0, 0);
            gluCylinder(quadric, 5, 0, 5, 50, 50);
            //glBegin(GL_QUADS);
            //    glVertex3f(-10,0,-10);
            //    glVertex3f(10,0,-10);
            //    glVertex3f(10,0,10);
            //    glVertex3f(-10,0,10);
            //glEnd();
            //glDisable(GL_LIGHT7);
            glDisable(GL_COLOR_MATERIAL);
        glPopMatrix();

    glPopMatrix();
}

void pregameAnimation()
{
}

void joustAnimation()
{
    if (gameEnd)
    {
        PlaySound(TEXT("Sounds/depairte.WAV"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        clashSound = false;
        clash = false;
        enginePlayOnce = false;
        game->reset(); 
        b1->bikePosX = 0;
        b2->bikePosX = 0;
        b1->alpha =1;
        b2->alpha =1;
        b1->isJousting = false;
        b2->isJousting = false;
        b1->isRotatedBy180 = false;
        b2->isRotatedBy180 = false;
        numberOfClashes = 0;
        velocityX = 0;   
    }            
    else if ((b1->bikePosX > 33 || b1->bikePosX < 11) && numberOfClashes >= 3)
    {                
        velocityX = 0;
        b1->velocityX = 0;
        b2->velocityX = 0;

        //gameEnd = true;
        if (b1->isWinner)
        {
            b2->deathAnimation();
        }
        if (b2->isWinner)
        {
            b1->deathAnimation();
        }
        if (!b1->isWinner && !b2->isWinner)
        {
            // draw   
        }                            
    }
    else
    {
        if ((b1->bikePosX <= 22 && !b1->isRotatedBy180)
            ||(b1->bikePosX >= 22 && b1->isRotatedBy180))
        {
            if (!enginePlayOnce)
            {
                enginePlayOnce = true;
                PlaySound(TEXT("Sounds/motorcycle_hd5.WAV"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
            }
            clashSound = false;
            clash = false;
            b1->accelerate();
            b2->accelerate();
        }
        else if ((b1->bikePosX <= 34 && !b1->isRotatedBy180)
            ||(b1->bikePosX >= 10 && b1->isRotatedBy180))      
        {
            if(!clash)
            {
                clash = true;
                enginePlayOnce = false;
            }
            if (numberOfClashes >= 3)
            {
                if (b1->isWinner)
                {
                    b2->deathAnimation();
                }
                if (b2->isWinner)
                {
                    b1->deathAnimation();
                }
            }
            b1->deccelerate();
            b2->deccelerate();
        }
        else if ((b1->bikePosX <= 36 && !b1->isRotatedBy180)
            || (b1->bikePosX >= 8 && b1->isRotatedBy180))
        {
            b1->deccelerate();
            b2->deccelerate();
        }
    }


    //  Playing clash sound 
    if (clash && !clashSound && !gameEnd)
    {
        PlaySound(TEXT("Sounds/metal_crunch.WAV"), NULL, SND_FILENAME);
        PlaySound(TEXT("Sounds/skids_x.WAV"), NULL, SND_FILENAME | SND_ASYNC );
        clashSound = true;
        numberOfClashes = numberOfClashes + 1;
    }
    //joust animation here
}
// This function is called to display the scene.
void display ()
{
    float start = clock();

    if ((int)menu->state == menu->MAIN ||
        (int)menu->state == menu->CREDITS ||
        (int)menu->state == menu->INSTRUCTIONS)
    {
        viewMenu();
        menu->display();
    }
    else if ((int)menu->state == menu->BIKESELECT)
    {
        bikeSelectView();
    }
    else if ((int)menu->state == menu->GAME)
    {		
        if(!game->getIsGameReady())
        {
            viewMenu();
            game->display();
            gameEnd = false;
            b1->isJousting = true;
            b2->isJousting = true;
        }
        else
        {            
            playView();                
            scenery->draw();
            
            bool superAwesomeCameraIntro = false;
            
            if (superAwesomeCameraIntro)
            {
            }
            else
            {
                joustAnimation();
            }


            // This draws the motorcycles on the screen
                glPushMatrix();
                    if (!globalWireframe)
                    {
                        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                    }
        
                    glPushMatrix();
                        glTranslated(22,0,0.75);   
                        glRotatef(180, 0.0f, 1.0f, 0.0f);
                        b2->draw();                    
                    glPopMatrix();

                    glPushMatrix();
                        glTranslated(-22,0,-0.75);
                        b1->draw();                    
                    glPopMatrix();                
                glPopMatrix();
            
            glPopMatrix(); // the random pop
        }
        
    }
    else if ((int)menu->state == menu->FREE)
    {
        //Set viewing Parameters
         view();             
        //drawScenery() - 48fps;
        scenery->draw();

        //MOTORCYCLE
        glPushMatrix();
             
        //glPushMatrix();
        //    glRotatef(180, 0, 1, 0);
        //    glTranslated(0,0,-0.75);   
        //    b2->blue();
        //    b2->materialType = 1;
        //    b2->draw();
        //glPopMatrix();

        glPushMatrix();               
            glTranslated(0,0,-0.75);         
            b1->blue();
            b1->materialType = 1;
            b1->draw();
            glPopMatrix();
        glPopMatrix();
        glPopMatrix();
    }
   // now swap buffer
    glutSwapBuffers();
   
    double end = clock();
    if(end-timePrevious != 0)
    fps = 1/((end-timePrevious)/CLOCKS_PER_SEC);
    timePrevious = clock();

}

// This function is called when there is nothing else to do.
void idle ()
{
    if(velocityX >= 0)
    {
        braking = true;
    }
    else
    {
        braking = false;
    }
    float circ = 1 * PI;
    wheel += velocityX;
    float rotationToDistance = (velocityX/360) * circ;
    bikePosX -= rotationToDistance * cos(direction * PI/180);
    bikePosZ += rotationToDistance * sin(direction * PI/180);
    
    if(velocityX != 0)
        direction += (heading* abs(velocityX))/360;
    if (wheel > ALL_ROUND)
    {wheel -= ALL_ROUND;}

    bikeSelectRotation += 0.4f;

    if ((int)menu->state == menu->BIKESELECT ||
        (int)menu->state == menu->GAME ||
        (int)menu->state == menu->FREE)
    {
        for (int i = 0; i < 3; ++i)
        {
            bikeVector[i]->sm->idle();
        }
    }

    glutPostRedisplay();
}

void mouseMovement (int mx, int my)
{
   // Normalize mouse coordinates.
   xMouse = double(mx) / double(width);
   yMouse = 1 - double(my) / double(height);

   // Redisplay image.
   glutPostRedisplay();
}

// Respond to window resizing, preserving proportions.
// Parameters give new window size in pixels.
void reshapeMainWindow (int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, GLfloat(width)/GLfloat(height), nearPlane, farPlane);
    
    glutPostRedisplay();
}

// Respond to graphic character keys.
// Parameters give key code and mouse coordinates.
void graphicKeys (unsigned char key, int x, int y)
{
    if ((int)menu->state == menu->MAIN)
    {
        menu->handleKeyPress(key, x, y);
        b1->okToDrawSpear = false;
        b2->okToDrawSpear = false;
    }
    else if ((int)menu->state == menu->BIKESELECT)
    {
        bikeSelect->handleKeyPress(key, x, y);

        if (bikeSelect->readyToPlay)
        {
            if (bikeSelect->enemyBikeIndex == bikeSelect->playerBikeIndex)
            {
                b2 = bikeVector[(bikeSelect->enemyBikeIndex % + 1) % 3];
            }
            else
            {
                b2 = bikeVector[bikeSelect->enemyBikeIndex];
            }
            b1->okToDrawSpear = true;
            b2->okToDrawSpear = true;
            bikeSelect->readyToPlay = false;
            game->reset();
            menu->state = menu->GAME;
        }
    }
    else if ((int)menu->state == menu->GAME)
    {
        bool value1 = game->getIsPlayerWinner();
        bool value2 = game->getIsAIWinner();
        game->handleKeyPress(key, x, y);

        if(game->getIsGameReady())
        {
            value1 = game->getIsPlayerWinner();
            value2 = game->getIsAIWinner();
            b1->isWinner = game->getIsPlayerWinner();
            b2->isWinner = game->getIsAIWinner();
        }
        switch (key)
        {
            //Zoom in/out f/b
        case 102:
            zoom('+');break;
        case 98:
            zoom('-');break;
    
            //Scale Model z/Z
        case 90:
            zoomInScale('+');break;
        case 122:
            zoomInScale('-');break;

            //Perspective View p/P
        case 80:
            perspectiveMode = 1;break;
        case 112:
            perspectiveMode = 1;break;

            //Orthogonal View Model o/O
        case 79:
            perspectiveMode = 0;break;
        case 111:
            perspectiveMode = 0;break;

            //Wire Frame / Shaded Model w/W
        case 87:
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            globalWireframe = true;
            break;
        case 119:
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            globalWireframe = false;
            break;
        case '1':
            firstPerson = true;
          break;
        case '2':
            sphereOn = !sphereOn;
          break;
        case '3':
            firstPerson = false;
            break;        
        case '0':
            gameEnd = true;
            break;
        case 's':
            if(velocityX != 0 ){
            savedVelocity = velocityX;
            b1->velocityX = 0;
            velocityX = 0;}
          break;
        default:
            break;
        }
    }
    else if ((int)menu->state == menu->FREE)
    {
        switch (key)
        {
            //Zoom in/out f/b
        case 102:
            zoom('+');break;
        case 98:
            zoom('-');break;
    
            //Scale Model z/Z
        case 90:
            zoomInScale('+');break;
        case 122:
            zoomInScale('-');break;

            //Perspective View p/P
        case 80:
            perspectiveMode = 1;break;
        case 112:
            perspectiveMode = 1;break;

            //Orthogonal View Model o/O
        case 79:
            perspectiveMode = 0;break;
        case 111:
            perspectiveMode = 0;break;

            //Wire Frame / Shaded Model w/W
        case 87:
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );break;
        case 119:
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );break;

            //Clear Changes resotre default c/C
        case 67:
            resetViewParameters();break;
        case 99:
            resetViewParameters();break;

        case 'a':
            velocityX -= 0.2;		
            glDisable(GL_LIGHT2);
            b1->accelerate();
            //b2->accelerate();
            braking = false;
          break;
       case 'd':
            velocityX += 0.2;
            braking = true;
            b1->deccelerate();
            //b2->deccelerate();
          break;	
        case 'q':
          heading += 40*RotSTEP;
          if (heading >= 360)
                heading = 0;
          b1->heading = heading;
          //b1->bikePosZ -= 0.2;
          break;
       case 'e':
          heading -= 40*RotSTEP;
          if (heading <= -360)
                heading = 0;
          b1->heading = heading;
          //b1->bikePosZ += 0.2;
          break;	 
        case '1':
            firstPerson = true;
          break;
        case '2':
            sphereOn = !sphereOn;
          break;
        case '3':
            firstPerson = false;
            break;
        case 's':
            if(velocityX != 0 ){
            savedVelocity = velocityX;
            b1->velocityX = 0;
            velocityX = 0;}
          break;
        default:
            break;
        }
    }
    if (key == 27)
    {
        gameEnd = false;
        clashSound = false;
        clash = false;
        enginePlayOnce = false;
        game->reset(); 
        b1->bikePosX = 0;
        b2->bikePosX = 0;
        b2->alpha =1;
        b1->alpha =1;
        numberOfClashes = 0;
        velocityX = 0;
        b1->okToDrawSpear = true;
        b2->okToDrawSpear = true;
        b1->isJousting = false;
        b2->isJousting = false;
        b1->isRotatedBy180 = false;
        b2->isRotatedBy180 = false;
        b1->init();
        b2->init();
        menu->state = menu->MAIN;
        PlaySound(TEXT("Sounds/depairte.WAV"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }
    if (key == 'i' || key == 'I')
    {
        if (menu->state == menu->INSTRUCTIONS)
        {
            menu->state = menu->previousState;
        }
        else
        {
            menu->previousState = menu->state;
            menu->state = menu->INSTRUCTIONS;
        }
    }

    glutPostRedisplay();
}

// Respond to function keys.
// Parameters give key code and mouse coordinates.
void functionKeys (int key, int x, int y)
{
    if ((int)menu->state == menu->GAME)
    {
        // game related input goes here
    }
    if ((int)menu->state == menu->FREE || (int)menu->state == menu->GAME)
    {
        switch (key)
        {

        case GLUT_KEY_UP:
            beta += 10*RotSTEP;
            if (beta > ALL_ROUND)
                beta -= ALL_ROUND;
            break;
        case GLUT_KEY_DOWN:
            beta -= 10*RotSTEP;
            if (beta < -ALL_ROUND)
                beta += ALL_ROUND;
            break;   
        case GLUT_KEY_LEFT:
            alpha += 10*RotSTEP;
            if (alpha > ALL_ROUND)
                alpha -= ALL_ROUND;
            break;
        case GLUT_KEY_RIGHT:
              alpha -= 10*RotSTEP;
              if (alpha < -ALL_ROUND)
                alpha += ALL_ROUND;
             break;
        case GLUT_KEY_F1:
            light1 = !light1;
            if(light1) {glEnable(GL_LIGHT0);glEnable(GL_LIGHT3);glEnable(GL_LIGHT4);}
            else{ glDisable(GL_LIGHT0);glDisable(GL_LIGHT3);glDisable(GL_LIGHT4);}
          break;

        case GLUT_KEY_F2:
            light2 = !light2;
            if(light2){ glDisable(GL_LIGHT1);	glDisable(GL_LIGHT5);}
            else {		glDisable(GL_LIGHT1);	glEnable(GL_LIGHT5);}
          break;

        case GLUT_KEY_F3:
            light3 = !light3;
            if(light3) {glDisable(GL_LIGHT6);}
            else{ glDisable(GL_LIGHT6);}
          break;

        case GLUT_KEY_F4:
            light4 = !light4;
            if(light4) {glDisable(GL_LIGHT7);}
            else{ glDisable(GL_LIGHT7);}
          break;

        case GLUT_KEY_F5:
            metal = !metal;
          break;

        case GLUT_KEY_F6:
            material = !material;
            if(material) {glEnable(GL_COLOR_MATERIAL);;}
            else{ glDisable(GL_COLOR_MATERIAL);}
          break;

        case 27:
            menu->state = menu->MAIN;
            PlaySound(TEXT("Sounds/depairte.WAV"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
            break;
        }
    }

    glutPostRedisplay();
}

void main (int argc, char **argv)
{
   // GLUT initialization. Enable double buffer mode
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("Moto A1");
    quadric = gluNewQuadric();
    
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluQuadricTexture(quadric, GL_TRUE);

    // Register call backs for standard tasks such as:
    // display, windowresize, keypress, functionkey press, mouse movement.
    glutDisplayFunc(display);
    glutReshapeFunc(reshapeMainWindow);
    glutKeyboardFunc(graphicKeys);
    glutSpecialFunc(functionKeys);
    glutMotionFunc(mouseMovement);
    glutIdleFunc(idle);

    lm->InitializeLights();
    tm->InitializeTextures(quadric);	

    bikeVector.push_back(new PatBike(tm, lm, new SmokeManager()));
    bikeVector.push_back(new KatBike(tm, lm, new SmokeManager()));
    bikeVector.push_back(new MatBike(tm, lm, new SmokeManager()));

    bikeSelect = new BikeSelect(bikeVector, b1);
    
    b1 = bikeVector[bikeSelect->playerBikeIndex];
    b2 = bikeVector[bikeSelect->enemyBikeIndex];

    scenery = new Scenery(tm, quadric);
    menu = new Menus(tm);
    game = new Game();
    PlaySound(TEXT("Sounds/depairte.WAV"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.33f, 0.20f, 1.0f, 0.0f);

     srand ( time(NULL) );
   // Enter GLUT loop.
    glutMainLoop();
}

