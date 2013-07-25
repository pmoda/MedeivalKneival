#include <cstdlib>
#include <GL/glut.h>
#include <windows.h>

#include "Menus.h"

Menus::Menus(TextureManager* tem)
{
    tm = tem;
    state = MAIN;
    previousState = MAIN;
    playString =  "(P)lay";
    freeString = "(F)ree Roam";
    instructionString = "(I)nstructions";
    creditsString = "(C)redits";
    exitString = "(E)xit";
    title = "Medieval Knievel";
}

Menus::~Menus()
{
    delete[] playString;
    delete[] freeString;
    delete[] creditsString;
    delete[] exitString;
    delete[] title;
    delete[] instructionString;
}

void Menus::display()
{
    if ((int)state == MAIN)
    {
        renderBitmapString(-8, 2, GLUT_BITMAP_TIMES_ROMAN_24, title);
        renderBitmapString(-8, -2, GLUT_BITMAP_TIMES_ROMAN_24, playString);
        renderBitmapString(-8, -4, GLUT_BITMAP_TIMES_ROMAN_24, freeString);
        renderBitmapString(-8, -6, GLUT_BITMAP_TIMES_ROMAN_24, instructionString); 
        renderBitmapString(-8, -8, GLUT_BITMAP_TIMES_ROMAN_24, creditsString);
        renderBitmapString(-8, -10, GLUT_BITMAP_TIMES_ROMAN_24, exitString);
    }
    else if ((int)state == CREDITS)
    {
        char* credits = "Developers: Pat Kat Matt and Chris";
        renderBitmapString(-14, -4, GLUT_BITMAP_TIMES_ROMAN_24, credits);
    }
    else if ((int)state == INSTRUCTIONS)
    {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glPushMatrix();
            glTranslatef(-14, -14, 0);
            glScalef(28, 28, 1);
            tm->BindTextureByIndex(10);
            glBegin(GL_QUADS);
                glTexCoord2f(0, 0);glVertex2f(0, 0);
                glTexCoord2f(1, 0);glVertex2f(1, 0);
                glTexCoord2f(1, 1);glVertex2f(1, 1);
                glTexCoord2f(0, 1);glVertex2f(0, 1);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}

void Menus::handleKeyPress(unsigned char key, int x, int y)
{
    if ((int)state == MAIN)
    {
        switch (key)
        {
        case 'P':
        case 'p':
            state = BIKESELECT;
            break;
        case 'F':
        case 'f':
            state = FREE;
            PlaySound(TEXT("Sounds/fanfare2.WAV"), NULL, SND_FILENAME | SND_ASYNC);
            break;
        case 'C':
        case 'c':
            state = CREDITS;
            PlaySound(TEXT("Sounds/fanfare_x.WAV"), NULL, SND_FILENAME | SND_ASYNC);
            break;
        case 'E':
        case 'e':
            std::exit(0);
            break;
        }
    }
}

void Menus::renderBitmapString(float x, float y, void *font, char *string)
{
    char* p;

    glRasterPos2f(x, y);

    for (p = string; *p; p++)
    {				
        glutBitmapCharacter(font, *p);
    }
}