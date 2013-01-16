#ifndef __BIKESELECT_H__
#define __BIKESELECT_H__
#include <vector>

#include "bike.h"

class BikeSelect
{
public:
    BikeSelect(std::vector<Bike*> playerBike, Bike* player);
    ~BikeSelect();

    bool readyToPlay;

    void displayText();
    void handleKeyPress(unsigned char key, int x, int y);

    std::vector<Bike*> m_playerBike;
    Bike* m_player;
    int playerBikeIndex;
    int enemyBikeIndex;

private:
    std::vector<char*> bikeSelectText;
    void renderBitmapString(float x, float y, void *font, char *string);
};

#endif // __BIKESELECT_H__