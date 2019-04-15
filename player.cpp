//Author: Addison Stotler
//Date Modified: 11/07/18
//player.cpp

#include "player.h"
#include "List.h"

Player::Player(SDL_Texture* playerTexture_, SDL_Texture* airTexture_)
  : velosityX(0), velosityY(0), xPosition(310), yPosition(333),
    playerTexture(playerTexture_), airTexture(airTexture_)
{
    //Start on the 2nd frame of going forward
    playerSrcRect.x = 0; playerSrcRect.y = 0;
    playerSrcRect.w = 38; playerSrcRect.h = 50;

    //Where the player is on the screen
    playerDestRect.x = xPosition; playerDestRect.y = yPosition;
    playerDestRect.w = 38; playerDestRect.h = 50;

    airSrcRectUD.x = 0; airSrcRectUD.y = 0;
    airDestRectUD.w = 50; airDestRectUD.h = 25;

    airSrcRectLR.x = 0; airSrcRectLR.y = 0;
    airSrcRectLR.w = 25; airSrcRectLR.h = 50;

    AirGauge * airGauge_ = new AirGauge();
    airGauge = airGauge_;
    
    //Bool Values
    onPlatform = false;
    isJump = false;
    isArrow = false;
    
}

//Getters
float* Player::getVelosityX()
{ return &velosityX; }

float* Player::getVelosityY()
{ return &velosityY; }

int Player::getXPosition()
{ return xPosition; }

int Player::getYPosition()
{ return yPosition; }

SDL_Rect Player::getPlayerDestRect()
{ return playerDestRect; }

bool Player::getOnPlatform()
{ return onPlatform;  }

//Setters
void Player::setIsArrow(bool value)
{ isArrow = value; }

void Player::setVelosityX(float force, Map* map)
{    
    //Is in the air
    if(map->collisionDetectionY() == false)
    {
	onPlatform = false;

	//Limited A and D motion in the air
	if(isArrow == false)
	{
	    if(force > 0)
		velosityX = velosityX + 0.15;
	    else if(force < 0)
		velosityX = velosityX - 0.15;
	}
	
	//Off ground speed reduction (basically air resistance)
	else
	{
	    if(velosityX + force > 10.9)
		velosityX = 10.9;
	    else if (velosityX + force < -10.9)
		velosityX = -10.9;
	    else
		velosityX = (velosityX + force);
	}
	
    }

    //On a platform
    else
    {
	onPlatform = true;
	
	//Max xVelosity while on the ground
	if(velosityX > 7.4 && force >= 0)
	    velosityX = 7.4;
	else if (velosityX < -7.4 && force <= 0)
	    velosityX = -7.4;
	else
	    velosityX = (velosityX + force);
    }

}

//Force is applied in the Y direction(could be movement or gravity)
void Player::setVelosityY(float force, Map* map, bool isAir)
{
    //The player can jump while on the ground
    if(map->collisionDetectionY() == true)
	onPlatform = true;
    else
	onPlatform = false;

    //If the player is not using up or down arrows
    if((isJump == true) && (onPlatform == true))
    {
	velosityY = velosityY + force;
    }

    //Arrow Case
    if(isJump == false)
	velosityY = velosityY + force;

    //Gravity Case
    //NOTE: This will for sure be changed
    else if(force == map->getGravity())
	velosityY = velosityY + force;
    
}

void Player::setXPosition(int newXPosition)
{ xPosition = newXPosition;}

void Player::setYPosition(int newYPosition)
{ yPosition = newYPosition;}

void Player::setJump(bool value)
{ isJump = value; }

void Player::resetLocation()
{
   //Start on the 2nd frame of going forward
    playerSrcRect.x = 0; playerSrcRect.y = 0;
    playerSrcRect.w = 38; playerSrcRect.h = 50;

    //Where the player is on the screen
    playerDestRect.x = xPosition; playerDestRect.y = yPosition;
    playerDestRect.w = 38; playerDestRect.h = 50;

    airSrcRectUD.x = 0; airSrcRectUD.y = 0;
    airDestRectUD.w = 50; airDestRectUD.h = 25;

    airSrcRectLR.x = 0; airSrcRectLR.y = 0;
    airSrcRectLR.w = 25; airSrcRectLR.h = 50;

}

//Animation for walking forward
void Player::forwardAnimation()
{
    //If the player is trying to walk on air
    if(onPlatform == false)
    {
	playerSrcRect.x = 38;
    }
    
    //Any frame from the backward animation
    if(playerSrcRect.x > 266)
    {
	playerSrcRect.x = 0;
    }
    
    else if(playerSrcRect.x != 266)
    {
	//Not on the last frame
	playerSrcRect.x = playerSrcRect.x+38;
    }
    else
    {
	//On the last frame
	playerSrcRect.x = 0;
    }
}

//Sets the animation frame back to forward 1
void Player::forwardAnimationReset()
{
   playerDestRect.x = xPosition;
}

void Player::backwardAnimation()
{
    //If the player is trying to walk on air
    if(onPlatform == false)
    {
	playerSrcRect.x = 532;
    }
    
    //Any frame from the forward animation
    if(playerSrcRect.x < 304)
	playerSrcRect.x = 570;
	
    else if(playerSrcRect.x != 304)
	//Not the last frame
	playerSrcRect.x = playerSrcRect.x-38;
    else
	playerSrcRect.x = 570;    
}

void Player::backwardAnimationReset()
{
    playerSrcRect.x = 570;
}

void Player::renderPlayer(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, playerTexture, &playerSrcRect, &playerDestRect);
    SDL_RenderCopy(renderer, airTexture, &airSrcRectUD, &airDestRectUD);
}

void Player::movePlayerY(float* yVelocity, Map* map)
{
    Node* node = map->getPlatformList()->getHead();
    
    while(node != NULL)
    {
	//If the player is in range of the platform
        if((((map->getPlayerLocation().x + map->getPlayerLocation().w) - 8) > node->getPlatform().x) &&
	   ((map->getPlayerLocation().x + 8) < (node->getPlatform().x + node->getPlatform().w)))
	{
	    //If the player will fall through the platform
	    if((map->getPlayerLocation().y + map->getPlayerLocation().h) <= node->getPlatform().y &&
	       (map->getPlayerLocation().y + map->getPlayerLocation().h + *yVelocity) >= node->getPlatform().y)
	    {
		//How far the player is from the platform
		playerDestRect.y = playerDestRect.y + (node->getPlatform().y - (map->getPlayerLocation().y + map->getPlayerLocation().h));
		map->setPlayerLocationY((node->getPlatform().y - map->getPlayerLocation().h));
		*yVelocity = 0;
	    }
	    
	    //If the player will jump through the platform
	    else if((map->getPlayerLocation().y + *yVelocity) <= (node->getPlatform().y + node->getPlatform().h) &&
		    (node->getPlatform().y + node->getPlatform().h) <= map->getPlayerLocation().y)
	    {
		//How far the player is from the platform
		playerDestRect.y = playerDestRect.y + ((node->getPlatform().y + node->getPlatform().h) - map->getPlayerLocation().y);
		map->setPlayerLocationY((node->getPlatform().y + node->getPlatform().h));
		*yVelocity = 0;
	    }
	}

	node = node->getNext();
    }
    
    map->setPlayerLocationY((floor(map->getPlayerLocation().y + *yVelocity)));
    playerDestRect.y = floor(playerDestRect.y + *yVelocity);


}

//Down Arrow
void Player::downAnimation(SDL_Renderer* renderer, Map* map , bool isHold)
{
    //If the player taped the arrow
    if(isHold == false)
    {
	//If the player is using the down arrow
	if(velosityY < 0)
	{
	    airSrcRectUD.x = 0; airSrcRectUD.y = 0;
	    airDestRectUD.x = (304);
	    airDestRectUD.y = ((map->getPlayerLocation().y + map->getPlayerLocation().h));
	    //SDL_RenderCopy(renderer, airTexture, &airSrcRectUD, &airDestRectUD);
	    isArrow = false;
	    //airSrc
	}
    }
}
//Up Arrow
//void upAnimation();
//Left Arrow
//void leftAnimation();
//Right Arrow
//void rightAnimation();
