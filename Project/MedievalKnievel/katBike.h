<<<<<<< HEAD
#ifndef __KATBIKE_H__
#define __KATBIKE_H__

#include <GL/glut.h>
#include "bike.h"

class KatBike : public Bike
{
public:
	KatBike();
	KatBike(TextureManager* t, LightManager* l, SmokeManager* s);
	virtual ~KatBike();

    void draw();
    void move();
    void init();


private:

    void  KatBike::DrawMotercycleTireSpoke(GLdouble rotateX, GLdouble rotateY);

    //This method is responsible for drawing a tire 
    //of a motercycle.
    void  KatBike::DrawMotercycleTire(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method is responsible for drawing any cross 
    //bars between the handle bar shafts.
    void  KatBike::DrawHandleCrossBar(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method is responsible for drawing the 
    //inside spokes of a motercycle tire and is a 
    //helper mthod to the DrawMotercycleTire function.
    void  KatBike::DrawHandleBarShaft(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method is responsible for drawing a 
    //handle bar for the motercycle.
    void  KatBike::DrawHandleBars(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method draws the part of the chassy that 
    //holds up the engine, gas tank and seat
    void  KatBike::DrawEngineSupport(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method draws the part of the chassy that 
    //holds up the engine, gas tank and seat
    void  KatBike::DrawOtherEngineSupport(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method draws the trailor hitch for the 
    //motercycle.
    void  KatBike::DrawTrailerHitch(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method is charged with drawing the trailor 
    //for the motercycle.
    void  KatBike::DrawTrailer(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method is responsible for drawing the 
    //motercycle seat.
    void  KatBike::DrawSeat(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method is responsible for drawing the 
    //motercycle seat.
    void  KatBike::DrawGasTank(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method is responsible for drawing the 
    //engine.
    void  KatBike::DrawEngine(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method is responsible for drawing the 
    //exhaust pipe.
    void  KatBike::DrawExhaustPipe(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method is responsible for drawing the 
    //front light
    void  KatBike::DrawFrontLight(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method is responsible for drawing the 
    //back light
    void  KatBike::DrawBackLight(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

};

=======
#ifndef __KATBIKE_H__
#define __KATBIKE_H__

#include <GL/glut.h>
#include "bike.h"

class KatBike : public Bike
{
public:
	KatBike();
	KatBike(TextureManager* t, LightManager* l, SmokeManager* s);
	virtual ~KatBike();

    void draw();
    void move();
    void init();


private:

    void  KatBike::DrawMotercycleTireSpoke(GLdouble rotateX, GLdouble rotateY);

    //This method is responsible for drawing a tire 
    //of a motercycle.
    void  KatBike::DrawMotercycleTire(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method is responsible for drawing any cross 
    //bars between the handle bar shafts.
    void  KatBike::DrawHandleCrossBar(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method is responsible for drawing the 
    //inside spokes of a motercycle tire and is a 
    //helper mthod to the DrawMotercycleTire function.
    void  KatBike::DrawHandleBarShaft(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method is responsible for drawing a 
    //handle bar for the motercycle.
    void  KatBike::DrawHandleBars(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method draws the part of the chassy that 
    //holds up the engine, gas tank and seat
    void  KatBike::DrawEngineSupport(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method draws the part of the chassy that 
    //holds up the engine, gas tank and seat
    void  KatBike::DrawOtherEngineSupport(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method draws the trailor hitch for the 
    //motercycle.
    void  KatBike::DrawTrailerHitch(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method is charged with drawing the trailor 
    //for the motercycle.
    void  KatBike::DrawTrailer(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method is responsible for drawing the 
    //motercycle seat.
    void  KatBike::DrawSeat(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method is responsible for drawing the 
    //motercycle seat.
    void  KatBike::DrawGasTank(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method is responsible for drawing the 
    //engine.
    void  KatBike::DrawEngine(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method is responsible for drawing the 
    //exhaust pipe.
    void  KatBike::DrawExhaustPipe(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method is responsible for drawing the 
    //front light
    void  KatBike::DrawFrontLight(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

    //This method is responsible for drawing the 
    //back light
    void  KatBike::DrawBackLight(GLfloat translateX, GLfloat translateY, GLfloat translateZ);

};

>>>>>>> first
#endif // __KATBIKE_H__