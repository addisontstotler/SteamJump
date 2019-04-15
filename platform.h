//Author: Addison Stotler
//platform.h

#ifndef PLATFORM
#define PLATFORM

#include <SDL2/SDL.h>

class Platform
{
  plublic:
    Platform();

    //Getters

    //Setters
    
    //Checks to see if the character is colliding with the platform
    bool platformHit(SDL_Rect character);

    //Renders the platform: this may go in the map object instead
    void renderPlatform(SDL_Renderer* renderer);
    
  private:
    SDL_Rect srcRect;
    SDL_Rect destRect;
    SDL_Texture* texture;

}



#endif
