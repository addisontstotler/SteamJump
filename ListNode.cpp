//Addison Stotler
//CSCI: 201
//Date Modified: 11/02/2017

#include "ListNode.h"

//Node class
Node::Node(SDL_Rect platform_)
{
    platform = platform_;
    next_ = NULL;
}

SDL_Rect Node::getPlatform()
{
    return platform;
}

Node * Node::getNext() const
{
   return next_;
}

int Node::getNumber()
{ return number; }

int Node::getY()
{ return platform.y; }

void Node::setNext(Node * next)
{
   next_= next;
}

void Node::setX(int x)
{
    platform.x = x;
}

void Node::setY(int y)
{
    platform.y = y;
}

void Node::setW(int w)
{
    platform.w = w;
}

void Node:: setH(int h)
{
    platform.h = h;
}

void Node::setNumber(int number_)
{  number = number_; }
