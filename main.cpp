//Author: Addison Stotler
//Final Project
//Date Modified: 11/03/2018

//c++
#include <iostream>
#include <time.h>
#include <fstream>
#include <stdlib.h>

//SDL
#include <SDL2/SDL.h>
#include "SDL_image.h"
#include "SDLfunctions.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h" 

//My files
#include "player.h"
#include "map.h"
#include "List.h"
#include "score.h"

//SDL Variables
SDL_Window* window;
SDL_Event event;
SDL_Renderer* renderer;
Mix_Chunk* sounds[10];
Mix_Music* musics[10];

//Functions
List * makeMap();
void mainMenu();
void controls();
void scores(int score);
void bestScores();

int score = 0;

int main()
{
    //Sets up the Window and Renderer
    setupSDL(window, renderer);
  
    //Rectangles: Source
    SDL_Rect srcGenericTestMap;

    //Rectangles: Destination
    SDL_Rect destGenericTestMap;

    //Sets the color to Black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    //loadImage: Loads all images into textures
    SDL_Texture* playerTexture = loadImage(renderer, "SteamPersonWalk.png");
    SDL_Texture* map1Texture = loadImage(renderer, "Map1test.png");
    SDL_Texture* airBlastTexture = loadImage(renderer, "air-blast.png");
    
    //loadAudio
    sounds[0] = Mix_LoadWAV("steam hisses - Marker #4.wav");
    musics[0] = Mix_LoadMUS("song21.mp3");
    musics[1] = Mix_LoadMUS("OnMyWay.wav");
    
    //Initalizes the Player
    Player* player = new Player(playerTexture, airBlastTexture);

    //Makes the test map
    List * testMapList = makeMap();
    
    //Initalizes the Maps
    Map* levels[10];
    int levelNumber = 0;
    
    SDL_Point start; start.x = 0; start.y = 0;
    SDL_Point end; end.x = 3220; end.y = 400;
    Map* map1 = new Map(480, 4105, map1Texture, start, end, testMapList, 1.3);

    levels[0] = map1;

    //Hold detector
    int holdDetectX = 0;
    int holdDetectY = 0;
    bool usedAir = false;

    //Time and win
    time_t begin;
    time_t finish;
    bool win = false;

while(1)
{
    //Renders the main menu
    mainMenu();

    //Start game music
    Mix_FadeInMusic(musics[1], -1, 2000);

    //Start time
    time(&begin);
    
    while(win == false)
    {		
        SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);

	//Handles movement input
	SDL_PumpEvents();
	const Uint8 *keystate;
        keystate = SDL_GetKeyboardState(NULL);
	
	if(keystate[SDL_SCANCODE_D])
	{
	    player->forwardAnimation();
	    player->setVelosityX(2.1, levels[levelNumber]);
	}

	if(keystate[SDL_SCANCODE_A])
	{
	    player->backwardAnimation();
	    player->setVelosityX(-2.1, levels[levelNumber]);
	}

	//Player jumping
	//Note: Different cases are handeled in the Map and Player calsses
	if(keystate[SDL_SCANCODE_W])
	{
	    //Jump force still TBD
	    player->setJump(true);
	    player->setVelosityY(-38.8, levels[levelNumber], false);
	    player->setJump(false);

	    //Prevents crazy high jump
	    if(holdDetectY > 4)
		holdDetectY = 0;
	}
	
	//Arrow Down
	if(keystate[SDL_SCANCODE_DOWN])
	{
	    if(holdDetectY > 4)
	    {
	      	player->setIsArrow(true);
		player->setVelosityY(player->airGauge->airHoldY(true, levels[levelNumber]->getGravity()), levels[levelNumber], true);
		player->downAnimation(renderer, levels[levelNumber], false);
	    }
		
	    holdDetectY++;
	}

	//Arrow up
	if(keystate[SDL_SCANCODE_UP])
	{
	    if(holdDetectY > 4)
	    {
		player->setIsArrow(true);	    
		player->setVelosityY(player->airGauge->airHoldY(false, levels[levelNumber]->getGravity()), levels[levelNumber], true);
	    }
		
	    holdDetectY++;
	}

	//Arrow left
	if(keystate[SDL_SCANCODE_LEFT])
	{
	    if(holdDetectX > 4)
	    {
		player->setIsArrow(true);
		player->setVelosityX(player->airGauge->airHoldX(true), levels[levelNumber]);
	    }
		
	    holdDetectY++;
	}

	//Arrow right
	if(keystate[SDL_SCANCODE_RIGHT])
	{
	    if(holdDetectX > 4)
	    {
		player->setIsArrow(true);		    
		player->setVelosityX(player->airGauge->airHoldX(false), levels[levelNumber]);
	    }
		
	    holdDetectX++;
	}
	  
	//Gravity:
	//If the player is slower than maximim fall speed
	//Note: A player can be moving faster because of upArrow
	if(*player->getVelosityY() < 8.2)
	{
	    player->setVelosityY(3.8, levels[levelNumber], false);
	}

	//Holding down arrow will match the force of gravity
	
	while(SDL_PollEvent(&event) > 0)
	{
	    switch(event.type)
	    {
	    case SDL_QUIT:
		//Quits the game
		exit(0);

	    case SDL_KEYUP:
		//Stop moving forward
		if(event.key.keysym.scancode == SDL_SCANCODE_D)
		{
		    player->forwardAnimationReset();
		}

		//Stop moving backward
		if(event.key.keysym.scancode == SDL_SCANCODE_A)
		{
		    player->backwardAnimationReset();
		}

		if(event.key.keysym.scancode == SDL_SCANCODE_DOWN)
		{
		    usedAir = true;
		    player->setIsArrow(true);
		    
		    //Was a hold
		    if(holdDetectY > 4)
		    {
			holdDetectY = 0;
		    }
		    //Was a tap
		    else
		    {
			holdDetectY = 0;
			player->setVelosityY(player->airGauge->airTapY(true), levels[levelNumber], true);
			player->downAnimation(renderer, levels[levelNumber], false);
			do_sounds(sounds[0]);
		    }
		    usedAir = false;
		}

		if(event.key.keysym.scancode == SDL_SCANCODE_UP)
		{
		    usedAir = true;
		    player->setIsArrow(true);
		    
		    //Was a hold
		    if(holdDetectY > 4)
		    {
			holdDetectY = 0;
		    }
		    //Was a tap
		    else
		    {
			holdDetectY = 0;
			if(*player->getVelosityY() < 0)
			{
			    *player->getVelosityY() = 0;
			}
			player->setVelosityY(player->airGauge->airTapY(false), levels[levelNumber], true);
			do_sounds(sounds[0]);
		    }
		    usedAir = false;
		}

		if(event.key.keysym.scancode == SDL_SCANCODE_LEFT)
		{
		    usedAir = true;
		    player->setIsArrow(true);
		    
		    //Was a hold
		    if(holdDetectX > 4)
		    {
			holdDetectX = 0;
		    }
		    //Was a tap
		    else
		    {
			holdDetectX = 0;
			player->setVelosityX(player->airGauge->airTapX(true), levels[levelNumber]);
			do_sounds(sounds[0]);
		    }
		    usedAir = false;
		}

		if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
		{
		    usedAir = true;
		    player->setIsArrow(true);
		    
		    //Was a hold
		    if(holdDetectX > 2)
		    {
			holdDetectX = 0;
		    }
		    //Was a tap
		    else
		    {
			holdDetectX = 0;
			player->setVelosityX(player->airGauge->airTapX(false), levels[levelNumber]);
			do_sounds(sounds[0]);
		    }
		    usedAir = false;
		}

		break;
            }
	}
	
	//Apply Gravity
	player->setVelosityY(levels[levelNumber]->getGravity(), levels[levelNumber], false);

	//Apply Friction
	if(levels[levelNumber]->collisionDetectionY() == true)
	{
	    //The player is moving forward
	    if(*player->getVelosityX() > 0)
	    {
		if((*player->getVelosityX() - 1.2) < 0)
		{
		    *player->getVelosityX() = 0;
		}
		else
		    player->setVelosityX(-1.2, levels[levelNumber]);
	    }
	    //The player is moving left
	    else if(*player->getVelosityX() <= 0)
	    {
		if((*player->getVelosityX() + 1.2) > 0)
		{
		    *player->getVelosityX() = 0;
		}
		else
		    player->setVelosityX(1.2, levels[levelNumber]);
	    }
	   
	}

	//Move the screen and player
        levels[levelNumber]->moveScreenX(player->getVelosityX());
	player->movePlayerY(player->getVelosityY(), levels[levelNumber]);
	
	//Draws the map
	levels[levelNumber]->renderScreen(renderer);
	
	//Render Player
	player->renderPlayer(renderer);

	//Checks if player wins
	if(((levels[levelNumber]->getPlayerLocation().x >= levels[levelNumber]->getEnd().x) &&
	    (levels[levelNumber]->getPlayerLocation().x + levels[levelNumber]->getPlayerLocation().w <= levels[levelNumber]->getEnd().x + 47)) &&
	   ((levels[levelNumber]->getPlayerLocation().y >= levels[levelNumber]->getEnd().y) &&
	    (levels[levelNumber]->getPlayerLocation().y + levels[levelNumber]->getPlayerLocation().h <= levels[levelNumber]->getEnd().y + 64)))
	    {
		win = true;
	    }
	
	SDL_Delay(40);

    }

    time(&finish);

    score = (difftime(finish, begin) * 12) + player->airGauge->getAirTotal();
    std::cout << score << std::endl;

    scores(score);
    
    //Value resets
    win = false;
    player->airGauge->resetTotal();
    levels[levelNumber]->resetPlayerLocation();
    player->resetLocation();
    
}
    
    SDL_QUIT;
    return 0;
}

List * makeMap()
{
    List * mapList = new List();
    SDL_Rect tempRect;

    //Celing
    mapList->insertHead(tempRect);
    mapList->getHead()->setX(0);
    mapList->getHead()->setY(0);
    mapList->getHead()->setW(6000);
    mapList->getHead()->setH(18);
    mapList->getHead()->setNumber(1);

     //Floor
    mapList->insertHead(tempRect);
    mapList->getHead()->setX(0);
    mapList->getHead()->setY(464);
    mapList->getHead()->setW(6000);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(2);

    //Right Side
    mapList->insertHead(tempRect);
    mapList->getHead()->setX(3758);
    mapList->getHead()->setY(0);
    mapList->getHead()->setW(48);
    mapList->getHead()->setH(480);
    mapList->getHead()->setNumber(3);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(309);
    mapList->getHead()->setY(176);
    mapList->getHead()->setW(48);
    mapList->getHead()->setH(48);
    mapList->getHead()->setNumber(4);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(384);
    mapList->getHead()->setY(335);
    mapList->getHead()->setW(32);
    mapList->getHead()->setH(128);
    mapList->getHead()->setNumber(5);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(384);
    mapList->getHead()->setY(335);
    mapList->getHead()->setW(96);
    mapList->getHead()->setH(32);
    mapList->getHead()->setNumber(6);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(507);
    mapList->getHead()->setY(144);
    mapList->getHead()->setW(16);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(7);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(550);
    mapList->getHead()->setY(240);
    mapList->getHead()->setW(208);
    mapList->getHead()->setH(48);
    mapList->getHead()->setNumber(8);
    
    mapList->insertHead(tempRect);
    mapList->getHead()->setX(710);
    mapList->getHead()->setY(352);
    mapList->getHead()->setW(144);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(9);
    
    mapList->insertHead(tempRect);
    mapList->getHead()->setX(838);
    mapList->getHead()->setY(240);
    mapList->getHead()->setW(112);
    mapList->getHead()->setH(48);
    mapList->getHead()->setNumber(10);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(910);
    mapList->getHead()->setY(432);
    mapList->getHead()->setW(32);
    mapList->getHead()->setH(32);
    mapList->getHead()->setNumber(11);
    
    mapList->insertHead(tempRect);
    mapList->getHead()->setX(1008);
    mapList->getHead()->setY(18);
    mapList->getHead()->setW(48);
    mapList->getHead()->setH(192);
    mapList->getHead()->setNumber(12);
    
    mapList->insertHead(tempRect);
    mapList->getHead()->setX(1008);
    mapList->getHead()->setY(162);
    mapList->getHead()->setW(96);
    mapList->getHead()->setH(48);
    mapList->getHead()->setNumber(13);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(1200);
    mapList->getHead()->setY(272);
    mapList->getHead()->setW(48);
    mapList->getHead()->setH(192);
    mapList->getHead()->setNumber(14);
    
    mapList->insertHead(tempRect);
    mapList->getHead()->setX(1232);
    mapList->getHead()->setY(85);
    mapList->getHead()->setW(16);
    mapList->getHead()->setH(64);
    mapList->getHead()->setNumber(15);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(1248);
    mapList->getHead()->setY(85);
    mapList->getHead()->setW(431);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(16);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(1456);
    mapList->getHead()->setY(382);
    mapList->getHead()->setW(64);
    mapList->getHead()->setH(32);
    mapList->getHead()->setNumber(17);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(1584);
    mapList->getHead()->setY(312);
    mapList->getHead()->setW(32);
    mapList->getHead()->setH(32);
    mapList->getHead()->setNumber(18);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(1584);
    mapList->getHead()->setY(432);
    mapList->getHead()->setW(32);
    mapList->getHead()->setH(32);
    mapList->getHead()->setNumber(19);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(1686);
    mapList->getHead()->setY(224);
    mapList->getHead()->setW(98);
    mapList->getHead()->setH(32);
    mapList->getHead()->setNumber(20);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(1680);
    mapList->getHead()->setY(365);
    mapList->getHead()->setW(32);
    mapList->getHead()->setH(32);
    mapList->getHead()->setNumber(21);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(1808);
    mapList->getHead()->setY(280);
    mapList->getHead()->setW(32);
    mapList->getHead()->setH(32);
    mapList->getHead()->setNumber(22);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(1808);
    mapList->getHead()->setY(398);
    mapList->getHead()->setW(32);
    mapList->getHead()->setH(32);
    mapList->getHead()->setNumber(23);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(1902);
    mapList->getHead()->setY(341);
    mapList->getHead()->setW(32);
    mapList->getHead()->setH(32);
    mapList->getHead()->setNumber(24);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(1776);
    mapList->getHead()->setY(85);
    mapList->getHead()->setW(240);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(25);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2064);
    mapList->getHead()->setY(272);
    mapList->getHead()->setW(48);
    mapList->getHead()->setH(48);
    mapList->getHead()->setNumber(26);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2112);
    mapList->getHead()->setY(272);
    mapList->getHead()->setW(48);
    mapList->getHead()->setH(192);
    mapList->getHead()->setNumber(27);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(228);
    mapList->getHead()->setY(272);
    mapList->getHead()->setW(48);
    mapList->getHead()->setH(96);
    mapList->getHead()->setNumber(28);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2276);
    mapList->getHead()->setY(320);
    mapList->getHead()->setW(144);
    mapList->getHead()->setH(48);
    mapList->getHead()->setNumber(29);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2388);
    mapList->getHead()->setY(368);
    mapList->getHead()->setW(32);
    mapList->getHead()->setH(32);
    mapList->getHead()->setNumber(30);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2292);
    mapList->getHead()->setY(120);
    mapList->getHead()->setW(32);
    mapList->getHead()->setH(32);
    mapList->getHead()->setNumber(31);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2404);
    mapList->getHead()->setY(117);
    mapList->getHead()->setW(16);
    mapList->getHead()->setH(144);
    mapList->getHead()->setNumber(32);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2420);
    mapList->getHead()->setY(165);
    mapList->getHead()->setW(48);
    mapList->getHead()->setH(48);
    mapList->getHead()->setNumber(33);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2548);
    mapList->getHead()->setY(216);
    mapList->getHead()->setW(16);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(34);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2532);
    mapList->getHead()->setY(304);
    mapList->getHead()->setW(16);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(35);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2628);
    mapList->getHead()->setY(240);
    mapList->getHead()->setW(16);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(36);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2516);
    mapList->getHead()->setY(395);
    mapList->getHead()->setW(16);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(37);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2608);
    mapList->getHead()->setY(366);
    mapList->getHead()->setW(16);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(38);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2672);
    mapList->getHead()->setY(374);
    mapList->getHead()->setW(16);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(39);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2672);
    mapList->getHead()->setY(448);
    mapList->getHead()->setW(16);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(40);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2742);
    mapList->getHead()->setY(144);
    mapList->getHead()->setW(16);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(41);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2789);
    mapList->getHead()->setY(277);
    mapList->getHead()->setW(16);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(42);


    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2789);
    mapList->getHead()->setY(379);
    mapList->getHead()->setW(16);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(43);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2865);
    mapList->getHead()->setY(195);
    mapList->getHead()->setW(16);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(44);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2915);
    mapList->getHead()->setY(314);
    mapList->getHead()->setW(16);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(45);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2956);
    mapList->getHead()->setY(429);
    mapList->getHead()->setW(16);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(46);
    
    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2973);
    mapList->getHead()->setY(224);
    mapList->getHead()->setW(16);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(47);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(3016);
    mapList->getHead()->setY(284);
    mapList->getHead()->setW(16);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(48);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(3164);
    mapList->getHead()->setY(350);
    mapList->getHead()->setW(32);
    mapList->getHead()->setH(32);
    mapList->getHead()->setNumber(49);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(3164);
    mapList->getHead()->setY(382);
    mapList->getHead()->setW(16);
    mapList->getHead()->setH(48);
    mapList->getHead()->setNumber(50);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(3180);
    mapList->getHead()->setY(302);
    mapList->getHead()->setW(16);
    mapList->getHead()->setH(48);
    mapList->getHead()->setNumber(51);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(3220);
    mapList->getHead()->setY(194);
    mapList->getHead()->setW(48);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(52);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(3196);
    mapList->getHead()->setY(350);
    mapList->getHead()->setW(384);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(53);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(3436);
    mapList->getHead()->setY(186);
    mapList->getHead()->setW(48);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(54);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(3507);
    mapList->getHead()->setY(93);
    mapList->getHead()->setW(48);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(55);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(3607);
    mapList->getHead()->setY(181);
    mapList->getHead()->setW(48);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(56);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(1984);
    mapList->getHead()->setY(432);
    mapList->getHead()->setW(32);
    mapList->getHead()->setH(32);
    mapList->getHead()->setNumber(57);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2228);
    mapList->getHead()->setY(273);
    mapList->getHead()->setW(48);
    mapList->getHead()->setH(96);
    mapList->getHead()->setNumber(58);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(2973);
    mapList->getHead()->setY(106);
    mapList->getHead()->setW(16);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(59);

    mapList->insertHead(tempRect);
    mapList->getHead()->setX(3032);
    mapList->getHead()->setY(50);
    mapList->getHead()->setW(16);
    mapList->getHead()->setH(16);
    mapList->getHead()->setNumber(60);
    
    return mapList;
}

void mainMenu()
{
    bool buttonClicked = false;

    const Uint8 *keystate;

    SDL_Rect menu;
    menu.x = 0; menu.y = 0;
    menu.w = 640; menu.h = 480;
    
    //Renders the mainMenu
    SDL_Texture* home = loadImage(renderer, "MainMenu.png");
    Mix_FadeInMusic(musics[0], -1, 2000);

    while(buttonClicked == false)
    {
	//Quits the game
	if(SDL_PollEvent(&event) > 0)
	{
	    switch(event.type)
	    {
	    case SDL_QUIT:
		exit(0);
	    case SDL_MOUSEBUTTONDOWN:
		//Start Game Button
		if((event.motion.x >= 199) && (event.motion.x <= 522) && (event.motion.y >= 220) && (event.motion.y <= 280))
		{
		    buttonClicked = true;
		    Mix_HaltMusic();
		}

		//Controls Button
		else if((event.motion.x >= 199) && (event.motion.x <= 522) && (event.motion.y >= 294) && (event.motion.y <= 353))
		{
		    controls();
		}

		//High Score Button
		else if((event.motion.x >= 199) && (event.motion.x <= 522) && (event.motion.y >= 368) && (event.motion.y <= 428))
		{
		    scores(score);
		}
	    }
	}
	    
	SDL_RenderCopy(renderer, home, &menu, &menu);
	SDL_RenderPresent(renderer);
	SDL_Delay(5);
    }
}

void controls()
{
    SDL_Texture* controls = loadImage(renderer, "Controls.png");

    const Uint8 *keystate;

    SDL_Rect menu;
    menu.x = 0; menu.y = 0;
    menu.w = 640; menu.h = 480;
    
    while(1)
    {
	//Quits the game
	if(SDL_PollEvent(&event) > 0)
	{
	    switch(event.type)
	    {
	    case SDL_QUIT:
		exit(0);
	    case SDL_KEYUP:
		if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		{
		    return;
		}
	    }
	}
	    
	SDL_RenderCopy(renderer, controls, &menu, &menu);
	SDL_RenderPresent(renderer);
	SDL_Delay(5);
    }
}

void scores(int score)
{
    //Fstream Object
    std::fstream myFstream;
    myFstream.open("bestScores.txt", std::fstream::in);
    
    SDL_Texture* scores = loadImage(renderer, "Scores.png");

    const Uint8 *keystate;

    SDL_Rect menu;
    menu.x = 0; menu.y = 0;
    menu.w = 640; menu.h = 480;

    //loadFont
    TTF_Font *font;
    font = TTF_OpenFont("8bitOperatorPlus8-Bold.ttf", 24);
    SDL_Color color;
    color.r = 89; color.g = 57; color.b = 26;

    //Convert int to char array
    SDL_Surface * textSurface = TTF_RenderText_Solid(font, std::to_string(score).c_str(), color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textDest;
    textDest.x = 215; textDest.y = 275; textDest.w = 200; textDest.h = 200; 

    //Type name
    SDL_Rect char1Dest; char char1 = ' ';
    SDL_Rect char2Dest; char char2 = ' ';
    SDL_Rect char3Dest; char char3 = ' ';
    SDL_Rect char4Dest; char char4 = ' ';

    char1Dest.x = 145; char1Dest.y = 150; char1Dest.w = 75; char1Dest.h = 70;
    char2Dest.x = 230; char2Dest.y = 150; char2Dest.w = 75; char2Dest.h = 70;
    char3Dest.x = 315; char3Dest.y = 150; char3Dest.w = 75; char3Dest.h = 70;
    char4Dest.x = 400; char4Dest.y = 150; char4Dest.w = 75; char4Dest.h = 70;


    SDL_Surface * char1Surface = TTF_RenderText_Solid(font, &char1, color);
    SDL_Texture * char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);

    SDL_Surface * char2Surface = TTF_RenderText_Solid(font, &char2, color);
    SDL_Texture * char2Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);

    SDL_Surface * char3Surface = TTF_RenderText_Solid(font, &char3, color);
    SDL_Texture * char3Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);

    SDL_Surface * char4Surface = TTF_RenderText_Solid(font, &char4, color);
    SDL_Texture * char4Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);

    int currentLetter = 1;
    
    while(1)
    {
	//Letter typing
	SDL_PumpEvents();
        keystate = SDL_GetKeyboardState(NULL);
	
	if(keystate[SDL_SCANCODE_Q])
	{
	    if(currentLetter == 1)
	    {
		char1 = 'Q';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }
	    
	    else if(currentLetter == 2)
	    {
		char2 = 'Q';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }
	    else if(currentLetter == 3)
	    {
		char3 = 'Q';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {
		char4 = 'Q';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }
		
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_W])
	{
	    if(currentLetter == 1)
	    {
		char1 = 'W';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }
	    else if(currentLetter == 2)
	    {
		char2 = 'W';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }
	    else if(currentLetter == 3)
	    {
		char3 = 'W';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {
		char4 = 'W';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_E])
	{
	    if(currentLetter == 1)
	    {
		char1 = 'E';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }
	    else if(currentLetter == 2)
	    {
		char2 = 'E';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }
	    else if(currentLetter == 3)
	    {
		char3 = 'E';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {
		char4 = 'E';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_R])
	{
	    if(currentLetter == 1)
	    {
		char1 = 'R';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }
	    else if(currentLetter == 2)
	    {
		char2 = 'R';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }
	    else if(currentLetter == 3)
	    {
		char3 = 'R';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {
		char4 = 'R';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_T])
	{
	    if(currentLetter == 1)
	    {
		char1 = 'T';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }
	    else if(currentLetter == 2)
	    {
		char2 = 'T';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }
	    else if(currentLetter == 3)
	    {
		char3 = 'T';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {
		char4 = 'T';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}
	
	if(keystate[SDL_SCANCODE_Y])
	{
	    if(currentLetter == 1)
	    {
		char1 = 'Y';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }
	    else if(currentLetter == 2)
	    {
		char2 = 'Y';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }
	    else if(currentLetter == 3)
	    {
		char3 = 'Y';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {
		char4 = 'Y';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_U])
	{
	    if(currentLetter == 1)
	    {
		char1 = 'U';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }
	    else if(currentLetter == 2)
	    {
		char2 = 'U';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }
	    else if(currentLetter == 3)
	    {
		char3 = 'U';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {
		char4 = 'U';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_I])
	{
	    if(currentLetter == 1)
	    {
		char1 = 'I';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }
	    else if(currentLetter == 2)
	    {
		char2 = 'I';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }
	    else if(currentLetter == 3)
	    {
		char3 = 'I';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {
		char4 = 'I';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_O])
	{
	    //First letter
	    if(currentLetter == 1)
	    {
		char1 = 'O';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }

	    //First letter
	    else if(currentLetter == 2)
	    {
		char2 = 'O';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }

	    //First letter
	    else if(currentLetter == 3)
	    {
		char3 = 'O';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {
		char4 = 'O';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_P])
	{
	    //First letter
	    if(currentLetter == 1)
	    {
		char1 = 'P';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }

	    //First letter
	    else if(currentLetter == 2)
	    {
		char2 = 'P';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }

	    //First letter
	    else if(currentLetter == 3)
	    {
		char3 = 'P';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	     {
		char4 = 'P';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	     }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_A])
	{
	    //First letter
	    if(currentLetter == 1)
	    {
		char1 = 'A';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }

	    //First letter
	    else if(currentLetter == 2)
	    {
		char2 = 'A';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }

	    //First letter
	    else if(currentLetter == 3)
	    {
		char3 = 'A';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {
		char4 = 'A';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_S])
	{
	    //First letter
	    if(currentLetter == 1)
	    {
		char1 = 'S';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }

	    //First letter
	    else if(currentLetter == 2)
	    {
		char2 = 'S';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }

	    //First letter
	    else if(currentLetter == 3)
	    {
		char3 = 'S';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {
		char4 = 'S';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_D])
	{
	    //First letter
	    if(currentLetter == 1)
	    {
		char1 = 'D';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }

	    //First letter
	    else if(currentLetter == 2)
	    {
		char2 = 'D';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }

	    //First letter
	    else if(currentLetter == 3)
	    {
		char3 = 'D';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {
		char4 = 'D';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_F])
	{
	    //First letter
	    if(currentLetter == 1)
	    {
		char1 = 'F';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }

	    //First letter
	    else if(currentLetter == 2)
	    {
		char2 = 'F';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }

	    //First letter
	    else if(currentLetter == 3)
	    {
		char3 = 'F';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {
		char4 = 'F';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_G])
	{
	    //First letter
	    if(currentLetter == 1)
	    {
		char1 = 'G';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }

	    //First letter
	    else if(currentLetter == 2)
	    {
		char2 = 'G';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }

	    //First letter
	    else if(currentLetter == 3)
	    {
		char3 = 'G';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {
		char4 = 'G';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_H])
	{
	    //First letter
	    if(currentLetter == 1)
	    {
		char1 = 'H';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }

	    //First letter
	    else if(currentLetter == 2)
	    {
		char2 = 'H';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }

	    //First letter
	    else if(currentLetter == 3)
	    {
		char3 = 'H';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {
		char4 = 'H';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_J])
	{
	    //First letter
	    if(currentLetter == 1)
	    {
		char1 = 'J';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }

	    //First letter
	    else if(currentLetter == 2)
	    {
		char2 = 'J';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }

	    //First letter
	    else if(currentLetter == 3)
	    {
		char3 = 'J';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {
		char4 = 'J';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_K])
	{
	    //First letter
	    if(currentLetter == 1)
	    {
		char1 = 'K';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }

	    //First letter
	    else if(currentLetter == 2)
	    {
		char2 = 'K';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }

	    //First letter
	    else if(currentLetter == 3)
	    {
		char3 = 'K';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {		
		char4 = 'K';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_L])
	{
	    //First letter
	    if(currentLetter == 1)
	    {
		char1 = 'L';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }

	    //First letter
	    else if(currentLetter == 2)
	    {
		char2 = 'L';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }

	    //First letter
	    else if(currentLetter == 3)
	    {
		char3 = 'L';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
		
	    }
	    else
	    {
		char4 = 'L';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_Z])
	{
	    //First letter
	    if(currentLetter == 1)
	    {
		char1 = 'Z';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }

	    //First letter
	    else if(currentLetter == 2)
	    {
		char2 = 'Z';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }

	    //First letter
	    else if(currentLetter == 3)
	    {
		char3 = 'Z';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {
		char4 = 'Z';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_X])
	{
	    //First letter
	    if(currentLetter == 1)
	    {
		char1 = 'X';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }

	    //First letter
	    else if(currentLetter == 2)
	    {
		char2 = 'X';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }

	    //First letter
	    else if(currentLetter == 3)
	    {
		char3 = 'X';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {
		char4 = 'X';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_C])
	{
	    //First letter
	    if(currentLetter == 1)
	    {
		char1 = 'C';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }

	    //First letter
	    else if(currentLetter == 2)
	    {
		char2 = 'C';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }

	    //First letter
	    else if(currentLetter == 3)
	    {
		char3 = 'C';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {
		char4 = 'C';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_V])
	{
	    //First letter
	    if(currentLetter == 1)
	    {
		char1 = 'V';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }

	    //First letter
	    else if(currentLetter == 2)
	    {
		char2 = 'V';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }

	    //First letter
	    else if(currentLetter == 3)
	    {
		char3 = 'V';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	     {
		char4 = 'V';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	     }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_B])
	{
	    //First letter
	    if(currentLetter == 1)
	    {
		char1 = 'B';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }

	    //First letter
	    else if(currentLetter == 2)
	    {
		char2 = 'B';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }

	    //First letter
	    else if(currentLetter == 3)
	    {
		char3 = 'B';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {
		char4 = 'B';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_N])
	{
	    //First letter
	    if(currentLetter == 1)
	    {
		char1 = 'N';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }

	    //First letter
	    else if(currentLetter == 2)
	    {
		char2 = 'N';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }

	    //First letter
	    else if(currentLetter == 3)
	    {
		char3 = 'N';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {
		char4 = 'N';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}
	
	if(keystate[SDL_SCANCODE_M])
	{
	    //First letter
	    if(currentLetter == 1)
	    {
		char1 = 'M';
		char1Surface = TTF_RenderText_Solid(font, &char1, color);
		char1Texture = SDL_CreateTextureFromSurface(renderer, char1Surface);
	    }

	    //First letter
	    else if(currentLetter == 2)
	    {
		char2 = 'M';
		char2Surface = TTF_RenderText_Solid(font, &char2, color);
		char2Texture = SDL_CreateTextureFromSurface(renderer, char2Surface);
	    }

	    //First letter
	    else if(currentLetter == 3)
	    {
		char3 = 'M';
		char3Surface = TTF_RenderText_Solid(font, &char3, color);
		char3Texture = SDL_CreateTextureFromSurface(renderer, char3Surface);
	    }
	    else
	    {
		char4 = 'M';
		char4Surface = TTF_RenderText_Solid(font, &char4, color);
		char4Texture = SDL_CreateTextureFromSurface(renderer, char4Surface);
	    }	
	    //Move to the next letter
	    currentLetter++; 
	}

	if(keystate[SDL_SCANCODE_BACKSPACE])
	{
	  if(currentLetter == 2)
	  {
	      
	  }
	    
	}
	
	//Quits the game
	if(SDL_PollEvent(&event) > 0)
	{
	    switch(event.type)
	    {
	    case SDL_QUIT:
		exit(0);
	    case SDL_KEYUP:
		if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		{
		    //TODO:output char1-4 and score to file
		    myFstream.close();
		    return;
		}	
	    }
	}

	
	
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, scores, &menu, &menu);
	SDL_RenderCopy(renderer, char1Texture, NULL, &char1Dest);
	SDL_RenderCopy(renderer, char2Texture, NULL, &char2Dest);
	SDL_RenderCopy(renderer, char3Texture, NULL, &char3Dest);
	SDL_RenderCopy(renderer, char4Texture, NULL, &char4Dest);
	SDL_RenderCopy(renderer, textTexture, NULL, &textDest);
	SDL_RenderPresent(renderer);
	SDL_Delay(60);
    }
}

void bestScore()
{
    //Fstream Object
    std::fstream myFstream;
    myFstream.open("bestScores.txt", std::fstream::in);
  
    SDL_Texture* scores = loadImage(renderer, "Scores.png");

    const Uint8 *keystate;

    SDL_Rect menu;
    menu.x = 0; menu.y = 0;
    menu.w = 640; menu.h = 480;

    //loadFont
    TTF_Font *font;
    font = TTF_OpenFont("8bitOperatorPlus8-Bold.ttf", 24);
    SDL_Color color;
    color.r = 89; color.g = 57; color.b = 26;

    //Convert int to char array
    SDL_Surface * textSurface = TTF_RenderText_Solid(font, std::to_string(score).c_str(), color);
    SDL_Texture* score1Texture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textDest1;
    textDest1.x = 220; textDest1.y = 275; textDest1.w = 200; textDest1.h = 200; 
    
    while(1)
    {
	//Quits the game
	if(SDL_PollEvent(&event) > 0)
	{
	    switch(event.type)
	    {
	    case SDL_QUIT:
		exit(0);
	    case SDL_KEYUP:
		if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		{
		    myFstream.close();
		    return;
		}	
	    }
	}

	SDL_RenderCopy(renderer, scores, &menu, &menu);
	SDL_RenderCopy(renderer, score1Texture, NULL, &textDest1);
	SDL_RenderPresent(renderer);
	SDL_Delay(5);
    }
}
