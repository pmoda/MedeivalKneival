#ifndef __MENUS_H__
#define __MENUS_H__

#include "TextureManager.h"

class Menus
{
public:
    enum MenuState
    {
        MAIN,
        BIKESELECT,
        GAME,
        FREE,
        CREDITS,
        INSTRUCTIONS
    };

    Menus(TextureManager* tm);
    virtual ~Menus();

    MenuState state;
    MenuState previousState;
    TextureManager* tm;

    void display();
    void handleKeyPress(unsigned char key, int x, int y);

    //This method is responsible for printing text to 
    //screen, given a font type and x,y position 
    //coordinates, using bitmap character.
    void renderBitmapString(float x, float y, void *font, char *string);

private:
    char* playString;
    char* freeString;
    char* creditsString; 
    char* exitString;
    char* title;
    char* instructionString;
};

#endif __MENUS_H__