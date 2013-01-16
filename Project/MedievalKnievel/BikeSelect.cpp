#include "BikeSelect.h"
#include <GL/glut.h>

BikeSelect::BikeSelect(std::vector<Bike*> playerBike, Bike* player) :
    bikeSelectText(),
    playerBikeIndex(0),
    enemyBikeIndex(1)
{
    readyToPlay = false;
    this->m_playerBike = playerBike;
    player = playerBike[0];
    m_player = player;

    bikeSelectText.push_back("Choose Bike:");
    bikeSelectText.push_back("1. PatBike");
    bikeSelectText.push_back("2. KatBike");
    bikeSelectText.push_back("3. MatBike");
    bikeSelectText.push_back("Color:");
    bikeSelectText.push_back("(R)ed");
    bikeSelectText.push_back("(G)reen");
    bikeSelectText.push_back("(B)lue");
    bikeSelectText.push_back(" ");
    bikeSelectText.push_back("Material:");
    bikeSelectText.push_back("(N)ormal");
    bikeSelectText.push_back("(M)etal");
    bikeSelectText.push_back("G(o)ld");
    bikeSelectText.push_back(" ");
    bikeSelectText.push_back("Press Enter");
    bikeSelectText.push_back("to play");
}

BikeSelect::~BikeSelect()
{
    for (int i = 0; i < bikeSelectText.size(); ++i)
    {
        delete[] bikeSelectText[i];
    }
}

void BikeSelect::displayText()
{	
    int xPos = -4;
    int yPos = 6;

    for (int i = 0; i < bikeSelectText.size(); ++i)
    {
        renderBitmapString(xPos, yPos, GLUT_BITMAP_HELVETICA_18, bikeSelectText[i]);
        yPos -= 1;
    }
}

void BikeSelect::handleKeyPress(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'R':
    case 'r':
        m_player->red();
        break;
    case 'G':
    case 'g':
        m_player->green();
        break;
    case 'B':
    case 'b':
        m_player->blue();
        break;
    case 'N':
    case 'n':
        m_player->materialType = 0;
        break;
    case 'M':
    case 'm':
        m_player->materialType = 1;
        break;
    case 'O':
    case 'o':
        m_player->materialType = 2;
        break;
    case '1':
        playerBikeIndex = 0;
        m_player = m_playerBike[playerBikeIndex];
        break;
    case '2':
        playerBikeIndex = 1;
        m_player = m_playerBike[playerBikeIndex];
        break;
    case '3':
        playerBikeIndex = 2;
        m_player = m_playerBike[playerBikeIndex];
        break;
    case '7':
        glEnable(GL_LIGHT7);
        break;   
    case '8':
        glDisable(GL_LIGHT7);
        break;
    case 13:
        readyToPlay = true;
        break;
    }
}

void BikeSelect::renderBitmapString(float x, float y, void *font, char *string)
{
    char* p;

    glRasterPos2f(x, y);

    for (p = string; *p; p++)
    {				
        glutBitmapCharacter(font, *p);
    }
}