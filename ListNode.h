//Addison Stotle
//CSCI: 201
//Date Modified: 11/02/2017

#ifndef LISTNODE
#define LISTNODE

#include<iostream>
#include<SDL2/SDL.h>

//Node class for the list
class Node
{
public:
    //Constructor
    Node(SDL_Rect platform_);

    //Getters
    SDL_Rect getPlatform();
    Node * getNext() const;
    int getNumber();
    int getY();

    //Setters
    void setNext(Node * next);
    void setX(int x);
    void setY(int y);
    void setW(int w);
    void setH(int h);
    void setNumber(int number_);

private:
    SDL_Rect platform;
    Node * next_;
    int number;
};

#endif
