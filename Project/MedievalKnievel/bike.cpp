<<<<<<< HEAD
#include <assert.h>
#include <fstream>

#include <stdlib.h>
#include <GL/glut.h>

#include "bike.h"

using namespace std;

const int NUM_TEXTURES = 10;

Bike::Bike(){
    quadric = gluNewQuadric();
    heading = 0;
    bikePosX = 0;
    bikePosZ = 0;
    velocityZ = 0;
    velocityX = 0;
    isWinner = false;
    wheelRotationDegree = 0;
    alpha = 1;
    okToDrawSpear = false;
    isJousting = false;
    isRotatedBy180 = false;
}

Bike::~Bike() {
}

void Bike::accelerate()
{
    velocityX -= 0.005;
}

void Bike::deccelerate()
{
    velocityX += 0.005;
}

void Bike::red()
{
    r = 1;
    g = 0;
    b = 0;
}
void Bike::green()
{
    r = 0;
    g = 1;
    b = 0;
}
void Bike::blue()
{
    r = 0;
    g = 0;
    b = 1;
}

void Bike::applyColor()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glAlphaFunc(GL_GREATER,0.2);
    glEnable(GL_ALPHA_TEST);
    float mat_ambient[] = {r, g, b, alpha};
    float mat_diffuse[] = {r, g, b, alpha};
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
}

void Bike::setMaterial()
{
    // Enable buffers to allow for alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glAlphaFunc(GL_GREATER,0.2);
    glEnable(GL_ALPHA_TEST);

    glDisable(GL_COLOR_MATERIAL);

    if(materialType == 0)
    {
        float no_mat[] = {0.0f, 0.0f, 0.0f, alpha};
        float mat_ambient[] = {0.0f, 0.0f, 0.0f, alpha};
        float mat_diffuse[] = {0.1f, 0.1f, 0.1f, alpha};
        float mat_specular[] = {0.15f, 0.15f, 0.15f, alpha};
        float low_shininess[] = {2.0f};

        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    }
    else if(materialType == 1){
        float no_mat[] = {0.0f, 0.0f, 0.0f, alpha};
        float mat_ambient[] = {0.7f, 0.7f, 0.7f, alpha};
        float mat_diffuse[] = {0.25f, 0.5f, 0.60f, alpha};
        float mat_specular[] = {1.0f, 1.0f, 1.0f, alpha};
        float low_shininess[] = {100.0f};

        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    }
    else if(materialType == 2)
    {
        float no_mat[] = {0.0f, 0.0f, 0.0f, alpha};
        float mat_ambient[] = {0.65f, 0.6f, 0.1f, alpha};
        float mat_diffuse[] = {0.7f, 0.5f, 0.15f, alpha};
        float mat_specular[] = {1.0f, 1.0f, 1.0f, alpha};
        float low_shininess[] = {300.0f};
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    }
}

void Bike::deathAnimation()
{
    //alpha = 0;
    if (alpha > 0)
    {
        alpha -= 0.025f;
    }	
    //glColor4f(1.0f,1.0f,1.0f, alpha);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );	
}

void Bike::drawSpearHandle(GLdouble base, GLdouble height, GLdouble slices, GLdouble stacks)
{
    base = base / 3;
    height = height / 5;
    slices = slices/3;
    stacks = stacks/3;

    glPushMatrix();
        glTranslatef(0.0f, 0.0f, -height);
        gluCylinder(quadric, base, base, height, slices, stacks); 
        glRotatef(180, 0,1,0); 
        gluDisk(quadric, 0.0f, base, slices, 1);
        glRotatef(180, 1,0,0); 
        glTranslatef(0.0f, 0.0f, height);
    glPopMatrix();
}

void Bike::drawSpearTip(GLdouble base, GLdouble top, GLdouble height, GLdouble slices, GLdouble stacks)
{
    //cylinder with nice disks to cap off
    slices = slices/3;
    stacks = stacks/3;

    glPushMatrix();		
        gluCylinder(quadric, base, top, height, slices, stacks); 
        glRotatef(180, 0,1,0); 
        gluDisk(quadric, 0.0f, base, slices, 1);
        glRotatef(180, 1,0,0); 
        glTranslatef(0.0f, 0.0f, height);
    glPopMatrix();
}

void Bike::drawSpear(GLdouble base, GLdouble top, GLdouble height, GLdouble slices, GLdouble stacks)
{	
    glPushMatrix();
        drawSpearTip(base, top, height, slices, stacks);
        drawSpearHandle(base, height, slices, stacks);
    glPopMatrix();	    
}

void Bike::joust()
{    
    float circ = tireRad * 2 * PI;
    float curveAngle = 0;    
    int pieces = 128;

    if(velocityX >= 0)
    {
        braking = true;
    }
    else
    {
        braking = false;
    }
    
    if (bikePosX >= 44 && !isRotatedBy180) // first turn
    {   
        curveAngle = (turnCounter/pieces) * PI + 3*PI / 2;            
        heading = -(turnCounter * 180)/pieces;
        
        bikePosX = -velocityX * cos(curveAngle) + 44.1;        
        bikePosZ = 0.75 * sin(curveAngle) + 0.75;

        if (heading <= -180)
        {
            velocityX = 0;
            isRotatedBy180 = true;
        }
        turnCounter += 2;


        wheelRotationDegree += (velocityX/circ) * 360  * cos(heading) ;
        //bikePosX -= rotationToDistance;
        if(wheelRotationDegree > ALLROUND)
        {wheelRotationDegree -= ALLROUND;}

    }
    else if (bikePosX <= 0 && isRotatedBy180) // second turn
    {        
        curveAngle = (turnCounter/pieces) * PI + PI / 2;            
        heading = -(turnCounter * 180 + pieces*180)/pieces;
        
        bikePosX = -velocityX * cos(curveAngle);        
        bikePosZ = 0.75 * sin(curveAngle) + 0.75;

        if (heading <= -360)
        {
            velocityX = 0;
            isRotatedBy180 = false;
        }
        turnCounter += 2;

        wheelRotationDegree += (velocityX/circ) * 360  * cos(heading) ;
        //bikePosX -= rotationToDistance;
        if(wheelRotationDegree > ALLROUND)
        {wheelRotationDegree -= ALLROUND;}
    }
    else if (isRotatedBy180) // on the other side of the fence
    {
        heading = 180;
        bikePosX -= velocityX * cos(PI);
        
        wheelRotationDegree += (velocityX/circ) * 360  * cos(heading) ;
        //bikePosX -= rotationToDistance;
        if(wheelRotationDegree > ALLROUND)
        {wheelRotationDegree -= ALLROUND;}
        turnCounter = 0;
    }
    else // starting position
    {
        heading = 0;
        bikePosX -= velocityX * cos(heading);

        wheelRotationDegree += (velocityX/circ) * 360  * cos(heading) ;
        //bikePosX -= rotationToDistance;
        if(wheelRotationDegree > ALLROUND)
        {wheelRotationDegree -= ALLROUND;}
        turnCounter = 0;
    }
    
}
=======
#include <assert.h>
#include <fstream>

#include <stdlib.h>
#include <GL/glut.h>

#include "bike.h"

using namespace std;

const int NUM_TEXTURES = 10;

Bike::Bike(){
    quadric = gluNewQuadric();
    heading = 0;
    bikePosX = 0;
    bikePosZ = 0;
    velocityZ = 0;
    velocityX = 0;
    isWinner = false;
    wheelRotationDegree = 0;
    alpha = 1;
    okToDrawSpear = false;
    isJousting = false;
    isRotatedBy180 = false;
}

Bike::~Bike() {
}

void Bike::accelerate()
{
    velocityX -= 0.005;
}

void Bike::deccelerate()
{
    velocityX += 0.005;
}

void Bike::red()
{
    r = 1;
    g = 0;
    b = 0;
}
void Bike::green()
{
    r = 0;
    g = 1;
    b = 0;
}
void Bike::blue()
{
    r = 0;
    g = 0;
    b = 1;
}

void Bike::applyColor()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glAlphaFunc(GL_GREATER,0.2);
    glEnable(GL_ALPHA_TEST);
    float mat_ambient[] = {r, g, b, alpha};
    float mat_diffuse[] = {r, g, b, alpha};
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
}

void Bike::setMaterial()
{
    // Enable buffers to allow for alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glAlphaFunc(GL_GREATER,0.2);
    glEnable(GL_ALPHA_TEST);

    glDisable(GL_COLOR_MATERIAL);

    if(materialType == 0)
    {
        float no_mat[] = {0.0f, 0.0f, 0.0f, alpha};
        float mat_ambient[] = {0.0f, 0.0f, 0.0f, alpha};
        float mat_diffuse[] = {0.1f, 0.1f, 0.1f, alpha};
        float mat_specular[] = {0.15f, 0.15f, 0.15f, alpha};
        float low_shininess[] = {2.0f};

        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    }
    else if(materialType == 1){
        float no_mat[] = {0.0f, 0.0f, 0.0f, alpha};
        float mat_ambient[] = {0.7f, 0.7f, 0.7f, alpha};
        float mat_diffuse[] = {0.25f, 0.5f, 0.60f, alpha};
        float mat_specular[] = {1.0f, 1.0f, 1.0f, alpha};
        float low_shininess[] = {100.0f};

        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    }
    else if(materialType == 2)
    {
        float no_mat[] = {0.0f, 0.0f, 0.0f, alpha};
        float mat_ambient[] = {0.65f, 0.6f, 0.1f, alpha};
        float mat_diffuse[] = {0.7f, 0.5f, 0.15f, alpha};
        float mat_specular[] = {1.0f, 1.0f, 1.0f, alpha};
        float low_shininess[] = {300.0f};
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    }
}

void Bike::deathAnimation()
{
    //alpha = 0;
    if (alpha > 0)
    {
        alpha -= 0.025f;
    }	
    //glColor4f(1.0f,1.0f,1.0f, alpha);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );	
}

void Bike::drawSpearHandle(GLdouble base, GLdouble height, GLdouble slices, GLdouble stacks)
{
    base = base / 3;
    height = height / 5;
    slices = slices/3;
    stacks = stacks/3;

    glPushMatrix();
        glTranslatef(0.0f, 0.0f, -height);
        gluCylinder(quadric, base, base, height, slices, stacks); 
        glRotatef(180, 0,1,0); 
        gluDisk(quadric, 0.0f, base, slices, 1);
        glRotatef(180, 1,0,0); 
        glTranslatef(0.0f, 0.0f, height);
    glPopMatrix();
}

void Bike::drawSpearTip(GLdouble base, GLdouble top, GLdouble height, GLdouble slices, GLdouble stacks)
{
    //cylinder with nice disks to cap off
    slices = slices/3;
    stacks = stacks/3;

    glPushMatrix();		
        gluCylinder(quadric, base, top, height, slices, stacks); 
        glRotatef(180, 0,1,0); 
        gluDisk(quadric, 0.0f, base, slices, 1);
        glRotatef(180, 1,0,0); 
        glTranslatef(0.0f, 0.0f, height);
    glPopMatrix();
}

void Bike::drawSpear(GLdouble base, GLdouble top, GLdouble height, GLdouble slices, GLdouble stacks)
{	
    glPushMatrix();
        drawSpearTip(base, top, height, slices, stacks);
        drawSpearHandle(base, height, slices, stacks);
    glPopMatrix();	    
}

void Bike::joust()
{    
    float circ = tireRad * 2 * PI;
    float curveAngle = 0;    
    int pieces = 128;

    if(velocityX >= 0)
    {
        braking = true;
    }
    else
    {
        braking = false;
    }
    
    if (bikePosX >= 44 && !isRotatedBy180) // first turn
    {   
        curveAngle = (turnCounter/pieces) * PI + 3*PI / 2;            
        heading = -(turnCounter * 180)/pieces;
        
        bikePosX = -velocityX * cos(curveAngle) + 44.1;        
        bikePosZ = 0.75 * sin(curveAngle) + 0.75;

        if (heading <= -180)
        {
            velocityX = 0;
            isRotatedBy180 = true;
        }
        turnCounter += 2;


        wheelRotationDegree += (velocityX/circ) * 360  * cos(heading) ;
        //bikePosX -= rotationToDistance;
        if(wheelRotationDegree > ALLROUND)
        {wheelRotationDegree -= ALLROUND;}

    }
    else if (bikePosX <= 0 && isRotatedBy180) // second turn
    {        
        curveAngle = (turnCounter/pieces) * PI + PI / 2;            
        heading = -(turnCounter * 180 + pieces*180)/pieces;
        
        bikePosX = -velocityX * cos(curveAngle);        
        bikePosZ = 0.75 * sin(curveAngle) + 0.75;

        if (heading <= -360)
        {
            velocityX = 0;
            isRotatedBy180 = false;
        }
        turnCounter += 2;

        wheelRotationDegree += (velocityX/circ) * 360  * cos(heading) ;
        //bikePosX -= rotationToDistance;
        if(wheelRotationDegree > ALLROUND)
        {wheelRotationDegree -= ALLROUND;}
    }
    else if (isRotatedBy180) // on the other side of the fence
    {
        heading = 180;
        bikePosX -= velocityX * cos(PI);
        
        wheelRotationDegree += (velocityX/circ) * 360  * cos(heading) ;
        //bikePosX -= rotationToDistance;
        if(wheelRotationDegree > ALLROUND)
        {wheelRotationDegree -= ALLROUND;}
        turnCounter = 0;
    }
    else // starting position
    {
        heading = 0;
        bikePosX -= velocityX * cos(heading);

        wheelRotationDegree += (velocityX/circ) * 360  * cos(heading) ;
        //bikePosX -= rotationToDistance;
        if(wheelRotationDegree > ALLROUND)
        {wheelRotationDegree -= ALLROUND;}
        turnCounter = 0;
    }
    
}
>>>>>>> first
