#ifndef __MATBIKE_H__
#define __MATBIKE_H__

#include <GL/glut.h>
#include "bike.h"

class MatBike : public Bike
{
public:

	MatBike();
	MatBike(TextureManager* t, LightManager* l, SmokeManager* s);

	virtual ~MatBike();
	
    bool wire;

	void draw();
	void move();
	void init();

private:
	float MatBike::handleBarYPos(void);

	float MatBike::handleBarXPos(void);

	void MatBike::drawSteering();

	void MatBike::drawSeat();

	void MatBike::drawBody();

	void MatBike::drawGasTank();

	void MatBike::drawEngine();

	void MatBike::drawBackWheel();

	void MatBike::drawFrontWheel();

	void MatBike::drawTrailer();    
};

#endif // __MATBIKE_H__
