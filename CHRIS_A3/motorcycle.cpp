#include <iostream>
#include <sstream>
#include <GL/glut.h>
#include <iomanip>

#define _USE_MATH_DEFINES
#include <math.h>

#include "imageloader.h"

using namespace std;

GLUquadricObj* quadratic;

// window vars
const int WIDTH = 1200;
const int HEIGHT = 700;
int width = WIDTH;
int height = HEIGHT;
double nearPlane =  1;
double farPlane  = 200;
double fovy = 40.0;

// logic vars
static int xRotation = 0;
static int yRotation = 0;
double distance = - (farPlane - nearPlane) / 2;    // Set by mouse Y.
int wireframe = 0;

GLfloat Z = 15.0f;		// Translate screen to z direction (zoom in or out)

bool perspective = true;

string fpsStr = "Frames per second (FPS): ";
double fps = 0.0;

int wheelRotation = 0;

double motoXPos = 5.0;
double motoYPos = 0.0;
double motoZPos = 0.0;
double firstPersonLookX = 0.0;
double firstPersonLookY = 0.0;

double motoRotation = 0;
float lemniscateTAnimation = 3 * M_PI / 2;
bool isAnimating = false;

bool firstPerson = false;
float acceleration = 500;
float doop = 700;
int frameCount = 0;
int currentTime = 0;
int previousTime = 0;

bool floodLightEnabled = true;

float crazyRotation = 0;

GLfloat lightColor0[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightColor1[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightPos0[] = {0.0f, 5.0f, 0.0f, 0.0f};
GLfloat lightDirectionBikeLight[] = {1.0f, -1.0f, 0.0f, 0.0f};
GLfloat lightPosBikeLight[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat lightCameraPos[] = {1.0f, 1.0f, 1.0f, 0.0f};
GLfloat lightCameraDirection[] = {0.0f, 0.0f, 0.0f, 0.0f};

GLuint textureId1;
GLuint textureId2;
GLuint textureId3;

void animateBike(bool isAccelerating)
{
    const double a = 50;
    double x; 
    double y; 
    lemniscateTAnimation = (lemniscateTAnimation + M_1_PI / acceleration);

    double t = lemniscateTAnimation;
    x = (a * sqrt(2.0) * cos(t)) / (pow(sin(t), 2.0) + 1);
    y = (a * sqrt(2.0) * cos(t) * sin(t)) / (pow(sin(t), 2.0) + 1);

    motoXPos = x;
    motoZPos = y;
    
    double degrees = (int)(t * 180 / M_PI) % 360;

    /*if ((degrees >= 87.5 && degrees <= 92.5) ||
        (degrees >= 267.5 && degrees <= 272.5))
    {
        crazyRotation += 180;
    }*/

    // check next one for the camera look direction in first person view
    t = t + M_1_PI / acceleration;
    firstPersonLookX = (a * sqrt(2.0) * cos(t)) / (pow(sin(t), 2.0) + 1);
    firstPersonLookY = (a * sqrt(2.0) * cos(t) * sin(t)) / (pow(sin(t), 2.0) + 1);
    
    motoRotation = atan2(x - firstPersonLookX, y - firstPersonLookY) * 180 / M_PI + 90;
    wheelRotation = (wheelRotation - 5) % 360;

    //if (isAccelerating && acceleration != 100)
    //    acceleration--;
    //else if (!isAccelerating && acceleration != 500)
    //    acceleration++;
}

void drawTrack()
{
    // Lemniscate of bernoulli
    const double a = 50;
    double t = 0.0;

    double x; 
    double y; 

    double width = 5.0;
    glBindTexture(GL_TEXTURE_2D, textureId1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);

        for (; t <= M_PI * 2; t += M_1_PI / 50)
        {
            x = (a * sqrt(2.0) * cos(t)) / (pow(sin(t), 2.0) + 1);
            y = (a * sqrt(2.0) * cos(t) * sin(t)) / (pow(sin(t), 2.0) + 1);

            glTexCoord2f(0, 0);
            glVertex3f(x, 0, y + width);
            glTexCoord2f(0, 1);
            glVertex3f(x + width, 0, y + width);
            glTexCoord2f(1, 1);
            glVertex3f(x + width, 0, y - width);
            glTexCoord2f(1, 0);
            glVertex3f(x, 0, y - width);
        }

    glEnd();
}

// draws a unit right triangular prism, which is used by the 
// gas tank and the seat
void drawUnitTriangularPrism()
{
    glBindTexture(GL_TEXTURE_2D, textureId2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glBegin(GL_TRIANGLES);
        glTexCoord2f(0, 0);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glTexCoord2f(0, 1);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(1, 0);
        glVertex3f(0.0f, 1.0f, 0.0f);

        glTexCoord2f(0, 0);
        glVertex3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0, 1);
        glVertex3f(1.0f, 0.0f, 1.0f);
        glTexCoord2f(1, 0);
        glVertex3f(0.0f, 1.0f, 1.0f);
    glEnd();
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0, 1);
        glVertex3f(0.0f, 1.0f, 1.0f);
        glTexCoord2f(1, 1);
        glVertex3f(1.0f, 0.0f, 1.0f);
        glTexCoord2f(1, 0);
        glVertex3f(1.0f, 0.0f, 0.0f);

        glTexCoord2f(0, 0);
        glVertex3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0, 1);
        glVertex3f(0.0f, 1.0f, 1.0f);
        glTexCoord2f(1, 1);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(1, 0);
        glVertex3f(0.0f, 0.0f, 0.0f);

        glTexCoord2f(0, 0);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glTexCoord2f(0, 1);
        glVertex3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(1, 1);
        glVertex3f(1.0f, 0.0f, 1.0f);
        glTexCoord2f(1, 0);
        glVertex3f(1.0f, 0.0f, 0.0f);
    glEnd();
}

void resetView()
{
    fovy = 40.0;
    xRotation = 0.0;
    yRotation = 0.0;
    wireframe = 0;
    Z = 15.0;
    perspective = true;
}

void drawWheel()
{
    // tire
    glPushMatrix();
        glutSolidTorus(0.10, 0.6, 10, 30);
    glPopMatrix();
    
    // thing in the middle of the tire that holds the spokes lol
    glPushMatrix();
        glutSolidTorus(0.05, 0.2, 5, 20);
    glPopMatrix();
    
    // Draw two caps for the middle of tire
    glPushMatrix();
        glTranslatef(0, 0, -0.04);
        gluDisk(quadratic, 0, 0.21, 20, 5);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, 0, 0.04);
        gluDisk(quadratic, 0, 0.21, 20, 5);
    glPopMatrix();

    // Draw two rims for the wheels
    glPushMatrix();
        glTranslatef(0, 0, 0.085);
        gluDisk(quadratic, 0.5, 0.6, 20, 5);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, 0, -0.085);
        gluDisk(quadratic, 0.5, 0.6, 20, 5);
    glPopMatrix();

    int spokeAngle = 0;
    int numOfSpokes = 10;
    int increment = 360 / numOfSpokes;

    //draw some spokes
    for (int i = 0; i < numOfSpokes; ++i)
    {
        glPushMatrix();
            glRotatef(spokeAngle, 0.0, 0.0, 1.0);
            glRotatef(-90, 1.0, 0.0, 0.0);
            glScalef(0.1, 0.1, 0.55);
            gluCylinder(quadratic, 0.1, 0.1, 1.0, 10, 5);
        glPopMatrix();

        spokeAngle += increment;

        if (spokeAngle > 360)
            break;
    }
}

// draw everything to do with handle bars, including the extensions
// to the front wheel and the connector for the two extensions
void drawHandleBars()
{
    // draw everything in the handle bar region
    glPushMatrix();
        glPushMatrix();
            glTranslatef(1.5, 0.0, -0.2);
            glRotatef(30, 0.0, 0.0, 1.0);
            glRotatef(-90, 1.0, 0.0, 0.0);
            gluCylinder(quadratic, 0.05, 0.05, 1.5, 20, 20);
        glPopMatrix();

        // connector right
        glPushMatrix();
            glTranslatef(1.5, 0.0, 0.0);
            gluCylinder(quadratic, 0.05, 0.05, 0.25, 20, 20);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1.5, 0.0, 0.25);
            gluDisk(quadratic, 0.0, 0.05, 20, 5);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(1.5, 0.0, 0.2);
            glRotatef(30, 0.0, 0.0, 1.0);
            glRotatef(-90, 1.0, 0.0, 0.0);
            gluCylinder(quadratic, 0.05, 0.05, 1.5, 20, 20);
        glPopMatrix();
        
        //connector left
        glPushMatrix();
            glTranslatef(1.5, 0.0, 0.0);
            glRotatef(180, 1.0, 0.0, 0.0);
            gluCylinder(quadratic, 0.05, 0.05, 0.25, 20, 20);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(1.5, 0.0, -0.25);
            //glRotatef(180, 1.0, 0.0, 0.0);
            gluDisk(quadratic, 0.0, 0.05, 20, 5);
        glPopMatrix();

        // draw handle bars
        glPushMatrix();
            glTranslatef(0.75f, 1.3f, 0.2f);
            gluCylinder(quadratic, 0.05, 0.05, 0.6, 20, 20);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.75f, 1.3f, -0.2f);
            glRotatef(180, 1.0, 0.0, 0.0);
            gluCylinder(quadratic, 0.05, 0.05, 0.6, 20, 20);
        glPopMatrix();
        
        // draw the connector of the two extensions and the handle bars
        glPushMatrix();
            glTranslatef(0.87f, 1.1f, 0.0); // move to where the handles are
            glRotatef(30.0f, 0.0, 0.0, 1.0);
            glScalef(0.1f, 0.5f, 0.4f);
            glutSolidCube(1.0);
        glPopMatrix();

    glPopMatrix();
}

// draws the front and back lights
void drawLights()
{   
    glBindTexture(GL_TEXTURE_2D, textureId3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    // draw front light
    glPushMatrix();
        glTranslatef(1.22f, 1.1f, 0.0); // move to the handle bar connector
        glRotatef(-90, 0.0, 1.0f, 0.0);
        glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
        glEnable(GL_TEXTURE_GEN_T);
        glBindTexture(GL_TEXTURE_2D, textureId3);
        glutSolidSphere(0.2f, 20, 20);
        glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
        glDisable(GL_TEXTURE_GEN_T);
        glutSolidCone(0.25f, 0.4f, 20, 10);
    glPopMatrix();

    // draw back light
    glPushMatrix();
        glTranslatef(-2.2f, 0.5f, 0.0f); // move to the handle bar connector
        glRotatef(90, 0.0f, 1.0f, 0.0f);

        glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
        glEnable(GL_TEXTURE_GEN_T);
        glBindTexture(GL_TEXTURE_2D, textureId3);
        glutSolidSphere(0.10f, 20, 20);
        glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
        glDisable(GL_TEXTURE_GEN_T);
        glutSolidCone(0.15f, 0.2f, 20, 10);
    glPopMatrix();
}

// draws the trailer of the motorcycle. This includes the tires,
// axle, cargo area, and hitch that will connect to the hitch
// of the motorcycle
void drawTrailer()
{
    glPushMatrix();
        glTranslatef(-5.0f, 0.0f, 0.0f);

        glBindTexture(GL_TEXTURE_2D, textureId3);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        // draw tires
        glPushMatrix();
            glTranslatef(0.0f, 0.0, 1.0f);
            if (isAnimating)
                {
                    glRotatef(wheelRotation, 0.0, 0.0, 1.0);
                }
            drawWheel();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0f, 0.0, -1.0f);
            if (isAnimating)
                {
                    glRotatef(wheelRotation, 0.0, 0.0, 1.0);
                }
            drawWheel();
        glPopMatrix();

        glBindTexture(GL_TEXTURE_2D, textureId1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        // draw axle between tires
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, -1.0f);
            gluCylinder(quadratic, 0.1f, 0.1f, 2.0f, 20, 20);
        glPopMatrix();

        // draw cargo area, which is 5 cubes making an enclosed area
        // with the top opened up
        glPushMatrix();
            glPushMatrix();
                glTranslatef(0.5f, 0.1f, 0.0f);
                glScalef(2.5f, 0.1f, 1.7f);
                glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
                glEnable(GL_TEXTURE_GEN_T);
                glBindTexture(GL_TEXTURE_2D, textureId1);
                glutSolidCube(1.0f);
                glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
                glDisable(GL_TEXTURE_GEN_T);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0.5f, 0.4f, 0.8f);
                glRotatef(90, 1.0f, 0.0f, 0.0f);
                glScalef(2.5f, 0.2f, 0.7f);
                glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
                glEnable(GL_TEXTURE_GEN_T);
                glBindTexture(GL_TEXTURE_2D, textureId1);
                glutSolidCube(1.0f);
                glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
                glDisable(GL_TEXTURE_GEN_T);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0.5f, 0.4f, -0.8f);
                glRotatef(90, 1.0f, 0.0f, 0.0f);
                glScalef(2.5f, 0.2f, 0.7f);
                glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
                glEnable(GL_TEXTURE_GEN_T);
                glBindTexture(GL_TEXTURE_2D, textureId1);
                glutSolidCube(1.0f);
                glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
                glDisable(GL_TEXTURE_GEN_T);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(-0.7f, 0.4f, 0.0f);
                glScalef(0.2f, 0.7f, 1.8f);
                glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
                glEnable(GL_TEXTURE_GEN_T);
                glBindTexture(GL_TEXTURE_2D, textureId1);
                glutSolidCube(1.0f);
                glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
                glDisable(GL_TEXTURE_GEN_T);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(1.7f, 0.4f, 0.0f);
                glScalef(0.2f, 0.7f, 1.8f);
                glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
                glEnable(GL_TEXTURE_GEN_T);
                glBindTexture(GL_TEXTURE_2D, textureId1);
                glutSolidCube(1.0f);
                glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
                glDisable(GL_TEXTURE_GEN_T);
            glPopMatrix();
        glPopMatrix();

        // draw the connector to the bike
        glPushMatrix();
            glTranslatef(2.0f, 0.6f, 0.0f);
            glPushMatrix();
                glTranslatef(0.3f, -0.05f, 0.0f);
                glRotatef(-90, 1.0f, 0.0f, 0.0f);
                glutSolidCone(0.1f, 0.2f, 20, 20);
            glPopMatrix();
            glPushMatrix();
                glRotatef(25, 0.0f, 0.0f, 1.0f);
                glScalef(0.7f, 0.15f, 0.15f);
                glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
                glEnable(GL_TEXTURE_GEN_T);
                glBindTexture(GL_TEXTURE_2D, textureId1);
                glutSolidCube(1.0f);
                glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
                glDisable(GL_TEXTURE_GEN_T);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

// drawing the body consists of the engine, seat and fuel tank
// with surrounding parts
void drawBody()
{
    glPushMatrix();
        // draw the engine
        glColor3f(0.5, 0.5, 0.5);
        glPushMatrix();
            // draw the two engine things
            glPushMatrix();
                glTranslatef(-0.1f, 0.17f, 0.0f);
                glRotatef(20, 0.0f, 0.0f, 1.0f);
                glScalef(0.85f, 1.0f, 0.6f);
                glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
                glEnable(GL_TEXTURE_GEN_T);
                glBindTexture(GL_TEXTURE_2D, textureId1);
                glutSolidCube(0.6f);
                glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
                glDisable(GL_TEXTURE_GEN_T);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0.4f, 0.17f, 0.0f);
                glRotatef(-20, 0.0f, 0.0f, 1.0f);
                glScalef(0.85f, 1.0f, 0.6f);
                glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
                glEnable(GL_TEXTURE_GEN_T);
                glBindTexture(GL_TEXTURE_2D, textureId1);
                glutSolidCube(0.6f);
                glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
                glDisable(GL_TEXTURE_GEN_T);
            glPopMatrix();

            // draw the muffler coming out of the engine things,
            // and the  big mufflers at the back of the moto
            glPushMatrix();
                glPushMatrix();
                    glTranslatef(-0.2f, 0.2f, 0.2f);
                    glRotatef(15, 0.0f, 0.0f, 1.0f); 
                    glRotatef(80, 1.0f, 0.0f, 0.0f);
                    gluCylinder(quadratic, 0.1f, 0.1f, 0.4f, 20, 20);
                glPopMatrix();
                glPushMatrix();
                    glTranslatef(-2.3f, -0.1f, 0.3f);
                    glRotatef(90, 0.0f, 1.0f, 0.0f);
                    gluCylinder(quadratic, 0.1f, 0.1f, 2.3f, 20, 20);
                glPopMatrix();
                glPushMatrix();
                    glTranslatef(0.5f, 0.2f, 0.2f);
                    glRotatef(-60, 0.0f, 0.0f, 1.0f); 
                    glRotatef(80, 1.0f, 0.0f, 0.0f);
                    gluCylinder(quadratic, 0.1f, 0.1f, 0.7f, 20, 20);
                glPopMatrix();
                glPushMatrix();
                    glTranslatef(-0.2f, 0.2f, -0.2f);
                    glRotatef(15, 0.0f, 0.0f, 1.0f); 
                    glRotatef(100, 1.0f, 0.0f, 0.0f);
                    gluCylinder(quadratic, 0.1f, 0.1f, 0.4f, 20, 20);
                glPopMatrix();
                glPushMatrix();
                    glTranslatef(-2.3f, -0.1f, -0.3f);
                    glRotatef(90, 0.0f, 1.0f, 0.0f);
                    gluCylinder(quadratic, 0.1f, 0.1f, 2.3f, 20, 20);
                glPopMatrix();
                glPushMatrix();
                    glTranslatef(0.5f, 0.2f, -0.2f);
                    glRotatef(-60, 0.0f, 0.0f, 1.0f); 
                    glRotatef(100, 1.0f, 0.0f, 0.0f);
                    gluCylinder(quadratic, 0.1f, 0.1f, 0.7f, 20, 20);
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();

        // draw the gas tank
        glColor3f(1.0, 0.6, 0.6);
        glPushMatrix();
            glTranslatef(0.8f, 0.5f, 0.4f);
            glRotatef(180, 0.0f, 1.0f, 0.0f);
            glScalef(1.4f, 0.7f, 0.8f);
            drawUnitTriangularPrism();
        glPopMatrix();

        // draw the seats
        glColor3f(1.0, 0.1, 0.1);
        glPushMatrix();
            glTranslatef(-0.8f, 0.3f, -0.4f);
            glRotatef(40, 0.0f, 0.0f, 1.0f);
            glScalef(0.8f, 0.7f, 0.8f);
            drawUnitTriangularPrism();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-1.6f, 0.75f, 0.0f);
            glScalef(0.7f, 0.2f, 0.8f);
            glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
            glEnable(GL_TEXTURE_GEN_T);
            glBindTexture(GL_TEXTURE_2D, textureId2);
            glutSolidCube(1.0f);
            glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
            glDisable(GL_TEXTURE_GEN_T);
        glPopMatrix();

        // draw the hitch
        glPushMatrix();
            glTranslatef(-2.7f, 0.50f, 0.0f);
            glutSolidSphere(0.1f, 20, 20);
            glRotatef(22, 0.0f, 0.0f, 1.0f);
            glRotatef(90, 0.0f, 1.0f, 0.0f);
            gluCylinder(quadratic, 0.05f, 0.07f, 0.8f, 20, 20);
        glPopMatrix();
    glPopMatrix();
}

void calculateFPS()
{
    frameCount++;

    currentTime = glutGet(GLUT_ELAPSED_TIME);

    int interval = currentTime - previousTime;

    if (interval > 1000)
    {
        fps = frameCount / (interval / 1000.0f);
        previousTime = currentTime;
        frameCount = 0;
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // fps viewport at the bottom
    glViewport(0, 0, width, height / 10);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (perspective)
        gluPerspective(45, GLfloat(width) / GLfloat(height / 10), nearPlane, farPlane);
    else
        glOrtho(-5, 5, -5, 5, nearPlane, farPlane);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 10,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    glColor3f(1.0, 1.0, 0.0);
    stringstream fpsStrAndNumber;
    fpsStrAndNumber << fpsStr << fps;
    
   // glDisable(GL_LIGHTING);
    glRasterPos2f(0, 0);
    for (int i = 0; i < fpsStrAndNumber.str().length(); ++i)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, fpsStrAndNumber.str()[i]);
    }
    //glEnable(GL_LIGHTING);

    // motorcycle viewport at the top
    glViewport(0, height / 10, width, height * 9 / 10);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (perspective)
        gluPerspective(fovy, GLfloat(width) / GLfloat(height), nearPlane, farPlane);
    else
        glOrtho(-5, 5, -5, 5, nearPlane, farPlane);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (!firstPerson)
    {
        gluLookAt(5.0, 0.0, Z,
                  motoXPos, motoYPos, motoZPos,
                  0.0, 1.0, 0.0);
    }
    else
    {
        gluLookAt(motoXPos, motoYPos + 1.5, motoZPos,
                  firstPersonLookX, 1.5, firstPersonLookY,
                  0, 1, 0);
    }

    if (wireframe == 1)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    lightPosBikeLight[0] = motoXPos;
    lightPosBikeLight[1] = motoYPos;
    lightPosBikeLight[2] = motoZPos + 2;
    lightDirectionBikeLight[0] = firstPersonLookX;
    lightDirectionBikeLight[1] = 1.5;
    lightDirectionBikeLight[2] = firstPersonLookY;
    lightPos0[0] = motoXPos;
    lightPos0[2] = motoZPos;
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    glLightfv(GL_LIGHT1, GL_POSITION, lightPosBikeLight);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightDirectionBikeLight);

    // draw motorcycle
    glPushMatrix();
        glRotatef(xRotation, 0.0, 1.0, 0.0);
        glRotatef(yRotation, 1.0, 0.0, 0.0);

        glPushMatrix();
            glTranslatef(motoXPos, motoYPos, motoZPos);
            glRotatef(motoRotation, 0.0, 1.0, 0.0);
            glRotatef(crazyRotation, 0.0, 1.0, 0.0);

            // colors will be chosen in this method
            drawBody();

            // draw wheels red
            glColor3f(1.0, 0.0, 0.0);
            glBindTexture(GL_TEXTURE_2D, textureId3);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            // draw front wheel
            glPushMatrix();
                glTranslatef(1.5, 0.0, 0.0);
                if (isAnimating)
                {
                    glRotatef(wheelRotation, 0.0, 0.0, 1.0);
                }
                drawWheel();
            glPopMatrix();

            //draw back wheel
            glPushMatrix();
                glTranslatef(-1.5, 0.0, 0.0);
                if (isAnimating)
                {
                    glRotatef(wheelRotation, 0.0, 0.0, 1.0);
                }
                drawWheel();
            glPopMatrix();

            glColor3f(0.0, 0.0, 1.0);
            drawHandleBars();

            glColor3f(1.0, 1.0, 1.0);
            drawLights();

            glColor3f(0.0, 0.3, 1.0);
            drawTrailer();

        glPopMatrix();
        
        glColor3f(1.0, 1.0, 1.0);
        glPushMatrix();
            glTranslatef(0, -0.75, 0);
            drawTrack();
        glPopMatrix();
    glPopMatrix();
    

    glutSwapBuffers();
}

// Respond to window resizing, preserving proportions.
// Parameters give new window size in pixels.
void reshapeMainWindow (int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;
    ////glViewport(0, 0, width, height);
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //gluPerspective(fovy, GLfloat(width) / GLfloat(height), nearPlane, farPlane);
}

void keys (unsigned char key, int x, int y)
{
    switch (key)
    {
    case '1':
        firstPerson = true;
        glutPostRedisplay();
        break;
    case '3':
        firstPerson = false;
        glutPostRedisplay();
        break;
    case 'A':
        acceleration -= 10;
        if (acceleration <= 0)
            acceleration = 1;
        break;
    case 'a':
        acceleration += 10;
        break;
    case 'c':
    case 'C':
        resetView();
        glutPostRedisplay();
        break;
    case 'e':
    case 'E':
        isAnimating = false;
        break;
    case 'f':
        Z -= 0.2f;
        glutPostRedisplay();
        break;
    case 'b':
        Z += 0.2f;
        glutPostRedisplay();
        break;
    case 'o':
    case 'O':
        perspective = false;
        glutPostRedisplay();
        break;
    case 'p':
    case 'P':
        perspective = true;
        glutPostRedisplay();
        break;
    case 's':
    case 'S':
        isAnimating = true;
        break;
    case 'w':
    case 'W':
        wireframe = 1 - wireframe;
        glutPostRedisplay();
        break;
    case 'z':
        fovy += 0.6;
        glutPostRedisplay();
        break;
    case 'Z':
        fovy -= 0.6;
        if (fovy < 0.0)
        {
            fovy = 0.0;
        }
        glutPostRedisplay();
        break;
    case 'g':
        doop += 1;
        break;
    case 'h':
        doop -= 1;
        break;
    case 27:
        exit(0);
    default:
        break;
    }
}

void specialKeys(int key, int x, int y)
{
    // The keys below are using the gluLookAt() function for navigation
    // Check which key is pressed
    switch(key) {
        case GLUT_KEY_LEFT :    // Rotate around the y axis
            xRotation = (xRotation + 5) % 360;
            break;
        case GLUT_KEY_RIGHT :	// Rotate around the y axis (opposite)
            xRotation = (xRotation - 5) % 360;
            break;
        case GLUT_KEY_UP :		// Rotate around the x axis
            yRotation = (yRotation + 5) % 360;
            break;
        case GLUT_KEY_DOWN :	// Rotate around the x axis (opposite)
            yRotation = (yRotation - 5) % 360;
            break;
        case GLUT_KEY_F1:
            if (floodLightEnabled)
            {
                glDisable(GL_LIGHT0);
                floodLightEnabled = false;
            }
            else
            {
                glEnable(GL_LIGHT0);
                floodLightEnabled = true;
            }
            break;
    }
    glutPostRedisplay();		// Redraw the scene
}

void idle()
{
    if (isAnimating)
    {
        animateBike(true);
        glutPostRedisplay();
        calculateFPS();
    }
}

GLuint loadTexture(Image* image) {
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

void main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("Pro Motorcycle");

    quadratic = gluNewQuadric();

    // Register call backs.
    glutDisplayFunc(display);
    glutReshapeFunc(reshapeMainWindow);
    glutKeyboardFunc(keys);
    glutSpecialFunc(specialKeys);
    glutIdleFunc(idle);

    /* set up depth-buffering */
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);

    Image* image = loadBMP("metal.bmp");
    textureId1 = loadTexture(image);
    delete image;

    image = loadBMP("leather.bmp");
    textureId2 = loadTexture(image);
    delete image;

    image = loadBMP("blood.bmp");
    textureId3 = loadTexture(image);
    delete image;

    gluQuadricTexture(quadratic, GL_TRUE);
    glEnable(GL_TEXTURE_2D);

    glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    //glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 15);
    //glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutMainLoop();
}