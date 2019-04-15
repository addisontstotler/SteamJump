//Author: Addison Stotler
//map.cpp

#include "map.h"
#include <iostream>

//Initalizes all variables in the Map.
//This may be done using a file later on.
Map::Map(int mapHeight_, int mapWidth_, SDL_Texture* mapTexture_, SDL_Point start_, SDL_Point end_, List * platformList_, float gravity_)
  : mapHeight(mapHeight_), mapWidth(mapWidth_), mapTexture(mapTexture_), start(start_), end(end_), platformList(platformList_), gravity(gravity_)
{
    //Initalize screen (aka src rect)
    screen.x = 0; screen.y = 0;
    screen.w = 640; screen.h = 480;

    mapSrc.x = start.x; mapSrc.y = start.y;
    mapSrc.w = 640; mapSrc.h = 480;

    //Location based on where the player is on the screen and where the screen is on the map
    playerLocation.x = mapSrc.x + 310; playerLocation.y = mapSrc.y + 333;
    playerLocation.w = 38; playerLocation.h = 50;

}

//Getters
float Map::getGravity()
{ return gravity; }

SDL_Rect Map::getPlayerLocation()
{ return playerLocation; }

List* Map::getPlatformList()
{ return platformList;}

int Map::getMapHeight()
{ return mapHeight;}

SDL_Point Map::getEnd()
{ return end;}

//Setters
void Map::setPlayerLocationY(int newY)
{ playerLocation.y = newY; }

void Map::resetPlayerLocation()
{
   //Initalize screen (aka src rect)
    screen.x = 0; screen.y = 0;
    screen.w = 640; screen.h = 480;

    mapSrc.x = start.x; mapSrc.y = start.y;
    mapSrc.w = 640; mapSrc.h = 480;

    //Location based on where the player is on the screen and where the screen is on the map
    playerLocation.x = mapSrc.x + 310; playerLocation.y = mapSrc.y + 333;
    playerLocation.w = 38; playerLocation.h = 50;
}

//Moves the screen based on the player's x velocity
void Map::moveScreenX(float* xVelocity)
{  
    Node* node = platformList->getHead();
    
    while(node != NULL)
    {
	//The player is parallel with a platform
	if(((playerLocation.y < node->getPlatform().y) && ((playerLocation.y + playerLocation.h) > node->getPlatform().y)) ||
	   ((playerLocation.y < (node->getPlatform().y + node->getPlatform().h)) && ((playerLocation.y + playerLocation.h) > (node->getPlatform().y + node->getPlatform().h)))
	   || ((playerLocation.y > node->getPlatform().y) && ((playerLocation.y + playerLocation.h) <= (node->getPlatform().y + node->getPlatform().h))))
	{
	    //Runs into the right side of a platform
	    if((playerLocation.x + playerLocation.w + *xVelocity) >= node->getPlatform().x && *xVelocity > 0 && (playerLocation.x) < node->getPlatform().x)
	    {
		//How far away the player is from the platform
		mapSrc.x = mapSrc.x + (node->getPlatform().x - (playerLocation.x + playerLocation.w));
		playerLocation.x = (node->getPlatform().x - playerLocation.w);
		*xVelocity = 0;
	    }

	    //Runs into the left side of a platform
	    else if((playerLocation.x + *xVelocity) <= (node->getPlatform().x + node->getPlatform().w) && *xVelocity < 0 && playerLocation.x >= node->getPlatform().x)
	    {
		//How far away the player is from the platform
		mapSrc.x = mapSrc.x + ((node->getPlatform().x + node->getPlatform().w) - playerLocation.x);
		playerLocation.x = (node->getPlatform().x + node->getPlatform().w);
		*xVelocity = 0;
	    }
	    
	}

	node = node->getNext();
    }
      
    //If the player walks backward off the map
    if((mapSrc.x + *xVelocity) <= 0)
    {
	mapSrc.x = 0;
	playerLocation.x = 310;
	*xVelocity = 0;
    }
    
    //If the player walks forward off the map
    else if((mapSrc.x + mapSrc.w + *xVelocity) >= mapWidth)
    {
	mapSrc.x = (mapWidth - mapSrc.w);
	playerLocation.x = (mapSrc.x + 310);
	*xVelocity = 0;
    }
    
    //Move normaly
    else
    {
	mapSrc.x = floor(mapSrc.x + *xVelocity);
	playerLocation.x = floor(playerLocation.x + *xVelocity);
    }
    
}

//Renders the portion of the map that is within the are of the screen
void Map::renderScreen(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, mapTexture, &mapSrc, &screen);
}

//Collision Detection for Y
bool Map::collisionDetectionY()
{
    Node * node = platformList->getHead();

    while(node != NULL)
    {
	//The player is over a platform
	if((((playerLocation.x + playerLocation.w) - 8) > node->getPlatform().x) && ((playerLocation.x + 8) < (node->getPlatform().x + node->getPlatform().w)) &&
	   ((playerLocation.y + playerLocation.h) <= node->getPlatform().y))
	{
	    //The player is on the platform
	    if((playerLocation.y + playerLocation.h) == node->getPlatform().y)
	    {
		return true;
	    }
	    
	    //The player hit their head
	    if(playerLocation.y == (node->getPlatform().y + node->getPlatform().h))
	    {
		return true;
	    }
	}
	//Go to next node
	node = node->getNext();
    }
    //The player is not on a platform
    return false;
}

