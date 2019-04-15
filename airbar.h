//Author: Addison Stotler
//airbar.h

#ifndef AIRBAR
#define AIRBAR

#include <SDL2/SDL.h>

class Airbar
{
  public:
    Airbar(SDL_Texture* airBarTexture_);

    //Getters

    //Setters
    
    //Renders the airbar in the upper left corner of the screen
    void renderAirbar(SDL_Renderer* renderer);

    //Changes the how much air is shown in the bar
    void updateAir();

  private:
    SDL_Rect srcRect;
    SDL_Rect destRect;
    SDL_Texture* airBarTexture;
    int airLevel;
    int airMax;
    int rechargeRate;
    int poofLine; //The y level for the poofLine
    
}

#endif
