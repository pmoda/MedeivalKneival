#ifndef __SCENERY_H__
#define __SCENERY_H__

#include "TextureManager.h"

class Scenery
{
public:
	Scenery();
	Scenery(TextureManager* t, GLUquadric* quad);

	bool init;
	
	struct Star {   // Declare Star struct type
		float starPitch;
		float starYaw;
		float size;
		bool initial;
	} stars[500];   // Define object of type Star
	
	int LOD;
	GLint list[3];

	GLUquadric* quadric;
	TextureManager* tm;

	void draw();

	void Scenery::drawCylinder(GLdouble base, GLdouble top, GLdouble height, GLdouble slices, GLdouble stacks, int scene);
	void Scenery::drawSkyrim();

	void drawCube();

	void Scenery::drawColumn();

	void Scenery::drawArch(float x1, float x2);

	void Scenery::drawColosseumRing(float amountOfArches, int x1, int x2, bool half, int layer);

	void Scenery::initColosseum(int x1, int x2);
	void Scenery::drawColosseum();

    //This method is responsible for drawing a hard 
    //shadow for the coliseum.
    void Scenery::drawColosseumShadow();

	void initFence();
	void Scenery::drawFence();

	void Scenery::drawWoodPlank(float height, float base);

	void Scenery::InnerRoad();
    void Scenery::OuterRoad();

	void Scenery::Grass();

	void Scenery::Moon();

	void Scenery::DrawStar();
};

#endif // __SCENERY_H__