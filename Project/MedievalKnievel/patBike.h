<<<<<<< HEAD
#ifndef __PATBIKE_H__
#define __PATBIKE_H__

#include <GL/glut.h>
#include "bike.h"

class PatBike : public Bike
{
public:
    PatBike();
    PatBike(TextureManager* t, LightManager* l, SmokeManager* sm);
    virtual ~PatBike();

    void draw();
    void move();
    void init();    

    GLint list[3];

private:
    void drawEngine(double topBase, double bottomBase, double backHeight, double width, double frontHeight);
    void drawCylinder(GLdouble base, GLdouble top, GLdouble height, GLdouble slices, GLdouble stacks);
    void drawSeat();
    void drawTrailerHitch();
    void drawBottomBackFrame();
    void drawTire(int spokes);
    void drawSpoke();
    void drawFrontFrame();
    void drawHead();
};

=======
#ifndef __PATBIKE_H__
#define __PATBIKE_H__

#include <GL/glut.h>
#include "bike.h"

class PatBike : public Bike
{
public:
    PatBike();
    PatBike(TextureManager* t, LightManager* l, SmokeManager* sm);
    virtual ~PatBike();

    void draw();
    void move();
    void init();    

    GLint list[3];

private:
    void drawEngine(double topBase, double bottomBase, double backHeight, double width, double frontHeight);
    void drawCylinder(GLdouble base, GLdouble top, GLdouble height, GLdouble slices, GLdouble stacks);
    void drawSeat();
    void drawTrailerHitch();
    void drawBottomBackFrame();
    void drawTire(int spokes);
    void drawSpoke();
    void drawFrontFrame();
    void drawHead();
};

>>>>>>> first
#endif // __PATBIKE_H__