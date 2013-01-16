#ifndef __GAME_H__
#define __GAME_H__

class Game
{

    public:
        Game();
        virtual ~Game();

        //Accessors
        bool getIsGameReady();
        bool getIsPlayerWinner();
        bool getIsAIWinner();
        
        //This method is responsible for displaying the 
        //players jousting strategy choices.
        void display();

        //This method is responsible for taking the input 
        //from the player and initiating the game setup process.
        void handleKeyPress(unsigned char key, int x, int y);

        //This method is responsible for reseting the member
        //variables of the game class.
        void reset();

    private:
        //Member variables
        bool isGameReady;
        bool isPlayerWinner;
        bool isAIWinner;
        bool keyHasBeenChosen;
        char* descriptionString;
        char* choice0;
        char* choice1; 
        char* choice2;
        char* choiceRandom;
        char* returnToMenu;

        //This method is responsible for generating the ai
        //decision and determining the outcome of the joust
        void initialiseGameParameters(int playerChoice);

        //This method is responsible for printing text to 
        //screen, given a font type and x,y position 
        //coordinates, using bitmap character.
        void renderBitmapString(float x, float y, void *font, char *string);
};

#endif __GAME_H__