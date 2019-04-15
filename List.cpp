//Addison Stotler
//CSCI: 201
//Date Modified: 11/02/2017

#include "List.h"
#include "ListNode.h"

List::List()
    : head_(NULL), tail_(NULL)
{}

Node * List::getHead() const
{
   return head_;
}

//insertHead:
//Inserts a new node to the front of the list
//Date Modified: 11/02/2017
void List::insertHead(SDL_Rect platform)
{
    //If the list is empty
    if(head_ == NULL)
    {
        Node * newNode = new Node(platform);
        head_ = newNode;
        tail_ = newNode;
    }
    //If the list already has nodes in it
    else
    {
        Node * newNode = new Node(platform);
        newNode->setNext(head_);
        head_ = newNode;
    }

}
