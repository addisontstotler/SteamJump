//Author: Addison Stotler
//map.h

#ifndef MAP
#define MAP

#include <SDL2/SDL.h>
#include <math.h>
#include <iostream>

#include "List.h"
#include "ListNode.h"

class Map
{
  public:
  Map(int mapHeight_, int mapWidth_, SDL_Texture* mapTexture_, SDL_Point start_, SDL_Point end_, List * platformList_, float gravity_);

    //Getters
    float getGravity();
    SDL_Rect getPlayerLocation();
    List* getPlatformList();
    int getMapHeight();
    SDL_Point getEnd();

    //Setters
    void setPlayerLocationY(int newY);
    void resetPlayerLocation();

    //Renders the portion of the map that is within the area of the screen.
    void renderScreen(SDL_Renderer* renderer);
    
    //Moves the screen based on the player's x velocity
    void moveScreenX(float* xVelocity);

    //Moves the screen based on the player's y velocity
    void moveScreenY(float* yVelocity, bool onPlatform);
    
    //Collision Detection in Y
    bool collisionDetectionY();
    
  private:
    //Enemy* enemyArray[10];
    SDL_Rect screen;
    SDL_Rect mapSrc;
    SDL_Texture* mapTexture;
    int mapHeight;
    int mapWidth;
    SDL_Rect playerLocation;
    List * platformList;
    
    SDL_Point start; //screen x and y will start here
    SDL_Point end; //screen x and y will have to be here to finish the level

    //Gravity: In the map class in case I want to change it depending
    //on the map.
    float gravity;
   
};

#endif
