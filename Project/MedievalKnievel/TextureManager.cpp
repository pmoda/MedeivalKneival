<<<<<<< HEAD
#include <assert.h>
#include <fstream>

#include <stdlib.h>
#include <GL/glut.h>

#include "TextureManager.h"

using namespace std;

const int NUM_TEXTURES = 12;

TextureManager::TextureManager()
{	
}

TextureManager::TextureManager(GLUquadric* quadric)
{	
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);	
}
void TextureManager::makeTexture(GLuint texID)
{
	glEnable(GL_TEXTURE_2D);	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
	glBindTexture(GL_TEXTURE_2D, texID);

}

//Preload an image and save it's texture id for later usage
void TextureManager::preloadTex(Image* image, GLuint texID)
{
	//GLuint textureid;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1 );
	glBindTexture(GL_TEXTURE_2D, texID); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, 
				GL_RGB, GL_UNSIGNED_BYTE, image->pixels);	
}

void TextureManager::InitializeTextures(GLUquadric* quadric)
{	
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);

	glGenTextures(NUM_TEXTURES, texture);
	Image *image[NUM_TEXTURES];
	image[0] = loadBMP("Metal4.bmp");
	image[1] = loadBMP("grass.bmp");
	image[2] = loadBMP("Metal1.bmp");
	image[3] = loadBMP("Motor.bmp");
	image[4] = loadBMP("Sand.bmp");
	image[5] = loadBMP("Brick.bmp");
	image[6] = loadBMP("SoilBeach1.bmp");
	image[7] = loadBMP("SoilBeach2.bmp");
	image[8] = loadBMP("Bark.bmp");
	image[9] = loadBMP("Metal5.bmp");
	image[10] = loadBMP("Instruction.bmp");
	image[11] = loadBMP("Moon.bmp");
	//image[0] = loadBMP("nic.bmp");
	//image[1] = loadBMP("nic.bmp");
	//image[2] = loadBMP("nic.bmp");
	//image[3] = loadBMP("nic.bmp");
	//image[4] = loadBMP("nic.bmp");
	//image[5] = loadBMP("nic.bmp");
	//image[6] = loadBMP("nic.bmp");
	//image[7] = loadBMP("nic.bmp");
	//image[8] = loadBMP("nic.bmp");
	//image[9] = loadBMP("nic.bmp");
	//image[10] = loadBMP("nic.bmp");
	//image[11] = loadBMP("nic.bmp");
	for(int i = 0; i < NUM_TEXTURES; i++ ) 
	{		
		preloadTex(image[i], texture[i]);
	}
}

int TextureManager::BindTextureByIndex(int index)
{
	glEnable(GL_TEXTURE_2D); 
	glColor3f(1,1,1);
	// Specify how texture values combine with current surface color values.
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
	glBindTexture(GL_TEXTURE_2D, texture[index]);
	return texture[index];
}

int TextureManager::BindTextureByIndex(int index, GLUquadric* quad)
{
	glColor3f(1,1,1);
	makeTexture(texture[index]);
	glNewList(list[0], GL_COMPILE);
	quad = gluNewQuadric();
	gluQuadricDrawStyle( quad, GLU_FILL);
	gluQuadricNormals( quad, GLU_SMOOTH);
	gluQuadricOrientation( quad, GLU_OUTSIDE);
	gluQuadricTexture( quad, GL_TRUE);
		gluSphere(quad,1,70,70);
	glEndList();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);

	return texture[index];
}

TextureManager::~TextureManager() {
}
=======
#include <assert.h>
#include <fstream>

#include <stdlib.h>
#include <GL/glut.h>

#include "TextureManager.h"

using namespace std;

const int NUM_TEXTURES = 12;

TextureManager::TextureManager()
{	
}

TextureManager::TextureManager(GLUquadric* quadric)
{	
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);	
}
void TextureManager::makeTexture(GLuint texID)
{
	glEnable(GL_TEXTURE_2D);	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
	glBindTexture(GL_TEXTURE_2D, texID);

}

//Preload an image and save it's texture id for later usage
void TextureManager::preloadTex(Image* image, GLuint texID)
{
	//GLuint textureid;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1 );
	glBindTexture(GL_TEXTURE_2D, texID); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, 
				GL_RGB, GL_UNSIGNED_BYTE, image->pixels);	
}

void TextureManager::InitializeTextures(GLUquadric* quadric)
{	
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);

	glGenTextures(NUM_TEXTURES, texture);
	Image *image[NUM_TEXTURES];
	image[0] = loadBMP("Metal4.bmp");
	image[1] = loadBMP("grass.bmp");
	image[2] = loadBMP("Metal1.bmp");
	image[3] = loadBMP("Motor.bmp");
	image[4] = loadBMP("Sand.bmp");
	image[5] = loadBMP("Brick.bmp");
	image[6] = loadBMP("SoilBeach1.bmp");
	image[7] = loadBMP("SoilBeach2.bmp");
	image[8] = loadBMP("Bark.bmp");
	image[9] = loadBMP("Metal5.bmp");
	image[10] = loadBMP("Instruction.bmp");
	image[11] = loadBMP("Moon.bmp");
	//image[0] = loadBMP("nic.bmp");
	//image[1] = loadBMP("nic.bmp");
	//image[2] = loadBMP("nic.bmp");
	//image[3] = loadBMP("nic.bmp");
	//image[4] = loadBMP("nic.bmp");
	//image[5] = loadBMP("nic.bmp");
	//image[6] = loadBMP("nic.bmp");
	//image[7] = loadBMP("nic.bmp");
	//image[8] = loadBMP("nic.bmp");
	//image[9] = loadBMP("nic.bmp");
	//image[10] = loadBMP("nic.bmp");
	//image[11] = loadBMP("nic.bmp");
	for(int i = 0; i < NUM_TEXTURES; i++ ) 
	{		
		preloadTex(image[i], texture[i]);
	}
}

int TextureManager::BindTextureByIndex(int index)
{
	glEnable(GL_TEXTURE_2D); 
	glColor3f(1,1,1);
	// Specify how texture values combine with current surface color values.
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
	glBindTexture(GL_TEXTURE_2D, texture[index]);
	return texture[index];
}

int TextureManager::BindTextureByIndex(int index, GLUquadric* quad)
{
	glColor3f(1,1,1);
	makeTexture(texture[index]);
	glNewList(list[0], GL_COMPILE);
	quad = gluNewQuadric();
	gluQuadricDrawStyle( quad, GLU_FILL);
	gluQuadricNormals( quad, GLU_SMOOTH);
	gluQuadricOrientation( quad, GLU_OUTSIDE);
	gluQuadricTexture( quad, GL_TRUE);
		gluSphere(quad,1,70,70);
	glEndList();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);

	return texture[index];
}

TextureManager::~TextureManager() {
}
>>>>>>> first
