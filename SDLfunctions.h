//SDLfunctions.h
//Description: This file contains functions to handle different things in SDL

//Finctions List:
//setupSDL - Sets up SDL window
//loadImage(filename) - Takes a file name, then returns a texture of that image

//Date Modified: 10/01/2018

#ifndef SDLFUNCTIONS
#define SDLFUNCTIONS

#include <iostream>
#include <SDL2/SDL.h>
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h" 

//setupSDL:
//Starts SDL window in 480p
//Date Modified: 10/01/2018
void setupSDL(SDL_Window*& window, SDL_Renderer*& renderer)
{

    //Sets up the window
    window = SDL_CreateWindow("Steam Jump", SDL_WINDOWPOS_UNDEFINED,
			      SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);

    //Sets up the renderer
    renderer = SDL_CreateRenderer(window, -1,
				 SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

    //If there is an error creating the window this will print.
    if(window == NULL)
    {
	std::cout << "Could not create window: " << SDL_GetError() << std::endl;
    }

    //Audio Stuff
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024);
    Mix_AllocateChannels(16);
    Mix_ReserveChannels(16);
    Mix_GroupChannels(0,15,1);

    //TTF
    TTF_Init();

}

//loadImage:
//loadImage takes a filename and loads the according image into a texture.
//Date Modified: 10/01/18
SDL_Texture* loadImage(SDL_Renderer* renderer, const char* filename)
{
    SDL_Texture *texture;

    texture = IMG_LoadTexture(renderer, filename);
    if (texture == NULL)
    {
	printf("IMG_LoadTexture Error: %s\n", IMG_GetError() );
	exit(1);
    }                                                    
                                                             
    return texture;                                            
}

void do_sounds(Mix_Chunk* what)
{
    int channel;

    channel = Mix_GroupAvailable(1);
    if (channel == -1)
	channel = Mix_GroupOldest(1);
    Mix_PlayChannel(channel, what, 0);
}


#endif
