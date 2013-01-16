#ifndef __BIKE_H__
#define __BIKE_H__

#include "Image.h"
#include "LightManager.h"
#include "TextureManager.h"
#include "SmokeManager.h"

#define PI 3.14159265
#define ALLROUND 360

class Bike
{
public:
    Bike();
    Bike(TextureManager* t, LightManager* l, SmokeManager* s);
    virtual ~Bike();
        
    TextureManager * tm;
    LightManager * lm;
    SmokeManager * sm;

    bool isWinner;
    bool okToDrawSpear;
    bool isRotatedBy180;
    bool isJousting;
    float turnCounter;

    float wheelRotationDegree;
    float heading;
    float bikePosX;
    float bikePosZ;
    float velocityX;
    float velocityZ;
    float tireRad;
    float alpha;

    //0 - normal
    //1 - metal
    //2 - gold	
    int materialType;
    int r;
    int g;
    int b;

    bool braking;
    GLUquadric* quadric;

    virtual void draw() = 0;
    virtual void move() = 0;
    virtual void init() = 0;

    void blue();
    void red();
    void green();
    void setMaterial();
    void applyColor();

    void accelerate();
    void deccelerate();

    void deathAnimation();

    void Bike::drawSpearHandle(GLdouble base, GLdouble height, GLdouble slices, GLdouble stacks);
    void Bike::drawSpearTip(GLdouble base, GLdouble top, GLdouble height, GLdouble slices, GLdouble stacks);
    void Bike::drawSpear(GLdouble base, GLdouble top, GLdouble height, GLdouble slices, GLdouble stacks);

    void Bike::joust();
    //when we set up player 2
    //we simple set heading to 180 and position to + X
};

#endif // __BIKE_H__