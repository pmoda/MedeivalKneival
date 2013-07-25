#ifndef TEXTUREMANAGER_H_INCLUDED
#define TEXTUREMANAGER_H_INCLUDED

#include <stdlib.h>
#include <GL/glut.h>

#include "Image.h"

class TextureManager {
	
	public:
		TextureManager();
		TextureManager(GLUquadric*);
		~TextureManager();
		
		int id;
		Image* image1;
		Image* image2;
		Image* image3;
		unsigned int texture[10];// Array of texture indices.
		GLint list[3];

		void TextureManager::makeTexture(GLuint texID);
		void TextureManager::preloadTex(Image* image, GLuint texID);
		void InitializeTextures(GLUquadric*);
		int BindTextureByIndex(int index);
		int BindTextureByIndex(int index, GLUquadric* quad);
};


#endif