//Addison Stotler
//Date Modified: 11/02/2018

#ifndef LIST
#define LIST

#include "ListNode.h"
#include "SDL2/SDL.h"

class List
{
public:
    //Constructor
    List();

    //Getters
    Node * getHead() const;
    
    //Setters
    void insertHead(SDL_Rect platform);
    
private:
    Node * head_;
    Node * tail_;
    
};

#endif
