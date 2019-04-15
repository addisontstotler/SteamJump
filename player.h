//Author: Addison Stotler
//player.h

#ifndef PLAYER
#define PLAYER

#include <iostream>
#include <SDL2/SDL.h>

#include "map.h"
#include "airGauge.h"

//Player class
class Player
{
  public:
    //Constructor: Only paramiters should be settings?
  Player(SDL_Texture* playerTexture_, SDL_Texture* airTexture_);
    
    //Getters
    float* getVelosityX();
    float* getVelosityY();
    int getXPosition();
    int getYPosition();
    SDL_Rect getPlayerDestRect();
    bool getJump();
    bool getOnPlatform();
    
    //Setters
    void setIsArrow(bool value);
    void setVelosityX(float force, Map* map);
    void setVelosityY(float force, Map* map, bool isAir);
    void setXPosition(int newXPosition);
    void setYPosition(int newYPosition);
    void setJump(bool value);
    void resetLocation();

    //Movement: Called during cirtain SDL events
    //KeyDown D
    void forwardAnimation();
    //KeyUp D
    void forwardAnimationReset();
    //KeyDown A
    void backwardAnimation();
    //KeyUp A
    void backwardAnimationReset();
    //Down
    void downAnimation(SDL_Renderer* renderer, Map* map, bool isHold);
    //Up
    void upAnimation();
    //Left
    void leftAnimation();
    //Right
    void rightAnimation();

    //Render Player
    void renderPlayer(SDL_Renderer* renderer);

    //Velosity:
    //X will be influenced by movement and friction
    //Y will be influenced by movement and gravity
    float velosityX;
    float velosityY;

    void movePlayerY(float* yVelocity, Map* map);

    AirGauge* airGauge;
  
  private:

    //X and Y position for the player
    int xPosition;
    int yPosition;

    //SDL stuff
    SDL_Rect playerSrcRect;
    SDL_Rect playerDestRect;
    SDL_Texture* playerTexture;
    SDL_Rect airSrcRectUD;
    SDL_Rect airDestRectUD;
    SDL_Rect airSrcRectLR;
    SDL_Rect airDestRectLR;
    SDL_Texture* airTexture;

    //Bool values
    bool onPlatform;
    bool isJump;
    bool isArrow;

};

#endif
