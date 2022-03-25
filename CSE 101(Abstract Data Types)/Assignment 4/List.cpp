/***********************************************************************
 * Harrison Quiring, hquiring
 * 2021 Spring CSE 101 pa4
 * List.cpp
 * Functions comprising the C++ List ADT
 * ***********************************************************************/

#include <stdlib.h>
#include <iostream>
#include <string>
#include "List.h"

using namespace std;
//*//CONSTRUCTORS
//Node Constructor
List::Node::Node(int x)//node constructor
 {
    data=x;
    next=nullptr;
    prev=nullptr;
 }
//List Constructor
List::List()
{
    frontDummy=new Node(-2147483646);//min value for an int
    backDummy=new Node(2147483647);//max value for an int
    frontDummy->next=backDummy;
    backDummy->prev=frontDummy;
    beforeCursor=frontDummy;
    afterCursor=backDummy;
    pos_cursor=0;
    num_elements=0;
}
//List copyConstructor
List::List(const List& L)
{
    //create empty list
    frontDummy=new Node(-2147483646);//min value for an int
    backDummy=new Node(2147483647);//max value for an int
    frontDummy->next=backDummy;
    backDummy->prev=frontDummy;
    beforeCursor=frontDummy;
    afterCursor=backDummy;
    pos_cursor=0;
    num_elements=0;
    //load elements of existing list into new list
    Node *fakecursor=L.frontDummy;
    while(fakecursor->next!=nullptr && fakecursor->next!=L.backDummy)
    {
        fakecursor=fakecursor->next;//start at first real node
        insertBefore(fakecursor->data);
    }
    while(position()>L.pos_cursor)//moves cursor to same spot as old list
    {
        movePrev();
    }
}
//List Destructor
List::~List()
{
    clear();
    delete(frontDummy);
    delete(backDummy);
}

//*//ACCESS FUNCTIONS

// Returns true if this List is empty, false otherwise.
bool List::isEmpty()
{
    if(num_elements==0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
// Returns the size of this List.
int List::size()
{
    return num_elements;
}
// Returns the position of the cursor in this List. The value returned
// will be in the range 0 to size().
int List::position()
{
    return pos_cursor;
}

//*//MANIPULATION PROCEDURES

// Moves cursor to position 0 in this List.
void List::moveFront()
{
    beforeCursor=frontDummy;
    pos_cursor=0;
    afterCursor=frontDummy->next;   
}
// Moves cursor to position size() in this List.
void List::moveBack()
{
    afterCursor=backDummy;
    pos_cursor=num_elements;
    beforeCursor=backDummy->prev;
}
// Returns the element after the cursor.
// pre: position()<size()
int List::peekNext()
{
    if(position()>=size())
    {
        cerr<<"WARNING OUT OF BOUNDS PEEKNEXT\n";
        exit(EXIT_FAILURE);
    }
    return afterCursor->data;
}
// Returns the element before the cursor.
// pre: position()>0
int List::peekPrev()
{
    if(position()<=0)
    {
        cerr<<"WARNING OUT OF BOUNDS PEEKPREV\n";
        exit(EXIT_FAILURE);
    }
    return beforeCursor->data;
}
// Advances cursor to next higher position. Returns the List element that
// was passed over. 
// pre: position()<size() 
int List::moveNext()
{
    if(position()>=size())
    {
        cerr<<"WARNING OUT OF BOUNDS MOVENEXT\n";
        exit(EXIT_FAILURE);
    }
    beforeCursor=afterCursor;
    afterCursor=afterCursor->next;
    pos_cursor+=1;
    return beforeCursor->data;
}
// Advances cursor to next lower position. Returns the List element that
// was passed over. 
// pre: position()>0
int List::movePrev()
{
    if(position()<=0)
    {
        cerr<<"WARNING OUT OF BOUNDS MOVEPREV\n";
        exit(EXIT_FAILURE);
    }
    afterCursor=beforeCursor;
    beforeCursor=beforeCursor->prev;
    pos_cursor-=1;
    return afterCursor->data;
}
// Inserts x after cursor.
void List::insertAfter(int x)
{
    Node* N=new Node(x);
    N->next=afterCursor;
    N->prev=beforeCursor;
    afterCursor->prev=N;
    beforeCursor->next=N;
    afterCursor=N;
    num_elements+=1;
}
// Inserts x before cursor.
void List::insertBefore(int x)
{
    Node* N=new Node(x);
    N->next=afterCursor;
    N->prev=beforeCursor;
    afterCursor->prev=N;
    beforeCursor->next=N;
    beforeCursor=N;
    num_elements+=1;
    pos_cursor+=1;
}
// Deletes element after cursor.
// pre: position()<size()
void List::eraseAfter()
{
    if(position()>=size())
    {
        cerr<<"WARNING OUT OF BOUNDS ERASEAFTER\n";
        exit(EXIT_FAILURE);
    }
    Node *Temp=afterCursor;
    afterCursor=afterCursor->next;
    beforeCursor->next=afterCursor;
    afterCursor->prev=beforeCursor;
    num_elements-=1;
    delete(Temp);
}
// Deletes element before cursor.
// pre: position()>0
void List::eraseBefore()
{
    if(pos_cursor<=0)
    {
        cerr<<"WARNING OUT OF BOUNDS ERASEBEFORE\n";
        exit(EXIT_FAILURE);
    }
    Node *Temp=beforeCursor;
    beforeCursor=beforeCursor->prev;
    afterCursor->prev=beforeCursor;
    beforeCursor->next=afterCursor;
    pos_cursor-=1;
    num_elements-=1;
    delete(Temp);
}
// Starting from the current cursor position, performs a linear search (in 
// the direction front-to-back) for the first occurrence of the element x.
// If x is found, places the cursor immediately after the found element (so 
// eraseBefore() would remove the found element), and returns the final 
// cursor position. If x is not found, places the cursor at position size(),
// and returns -1. 
int List::findNext(int x)
{
    while(afterCursor!=backDummy)
    {
        moveNext();
        if(peekPrev()==x)
        {
            return position();//if found the requested int
        }
    }
    return -1;
}
// Starting from the current cursor position, performs a linear search (in 
// the direction back-to-front) for the first occurrence of the element x.
// If x is found, places the cursor immediately before the found element (so 
// eraseAfter() would remove the found element), and returns the final 
// cursor position. If x is not found, places the cursor at position 0, and 
// returns -1. 
int List::findPrev(int x)
{
    while(beforeCursor!=frontDummy)
    {
        movePrev();
        if(peekNext()==x)
        {
            return position();//if found the requested int
        }
    }
    return -1;
}
// Removes any repeated elements in this List, leaving only unique data
// values. The order of the remaining elements is obtained by retaining
// the frontmost occurrance of each element, and removing all other 
// occurances. The cursor is not moved with respect to the retained 
// elements, i.e. it lies between the same two retained elements that it 
// did before cleanup() was called.
void List::cleanup()
{
    List* SecondaryList=new List();
    int cursorpos=0;
    Node* fakecursor=frontDummy->next;
    Node* cursortodelete;
    while(fakecursor->next!=nullptr)
    {
        SecondaryList->moveFront();
        if(SecondaryList->findNext(fakecursor->data)==-1)//returns -1 if number does not exist in secondary list
        {
            SecondaryList->moveBack();
            SecondaryList->insertAfter(fakecursor->data);
            fakecursor=fakecursor->next;
            cursorpos+=1;
        }
        else//deletes the number if it is already in the secondary list
        {
            fakecursor->prev->next=fakecursor->next;
            fakecursor->next->prev=fakecursor->prev;
            if(fakecursor==beforeCursor)
            {
                beforeCursor=fakecursor->prev;
            }
            if(fakecursor==afterCursor)
            {
                afterCursor=fakecursor->next;
            }
            if(cursorpos<position())
            {
                pos_cursor-=1;
            }
            num_elements-=1;
            cursortodelete=fakecursor;
            fakecursor=fakecursor->next;
            delete(cursortodelete);
        }
    }
    delete(SecondaryList);
}
// Deletes all elements in this List, setting it to the empty state.
void List::clear()
{
    moveFront();
    while(size()>0)
    {
        eraseAfter();
    }
}
// Returns a new List consisting of the elements of this List, followed
// the elements of L. The returned List's cursor will be at its front
// (position 0).
List List::concat(const List& L)
{
    List exportedlist=*this;//copies all of the nods from this list into the new exported list
    Node* fakecursor=L.frontDummy;//need to make a fake cursor as am not allowed to move the const list cursor
    fakecursor=fakecursor->next;//moving fakecursor to first real node
    while(fakecursor->next!=nullptr)//copies all of the nodes in the second list into the exported list
    {
        exportedlist.moveBack();
        exportedlist.insertAfter(fakecursor->data);
        fakecursor=fakecursor->next;//moving fakecursor to next
    }
    exportedlist.moveFront();
    return exportedlist;
}

//*//OTHER

// Returns a string representation of this List consisting of a comma 
// separated sequence of elements, surrounded by parentheses.
string List::to_string()
{
    string convertedlist="";
    Node* Fakecursor=frontDummy;
    while(Fakecursor->next!=backDummy)
    {
        Fakecursor=Fakecursor->next;
        convertedlist+=std::to_string(Fakecursor->data)+" ";//hopefully should cast data as a string, then append it to the string
    }
    return convertedlist;
}
// Returns true if and only if this List is the same integer sequence as R.
// The cursors in this List and in R are unchanged.
bool List::equals(const List& R)
{
    if(size()!=R.num_elements)
    {
        return false;
    }
    Node* fakecursor1=frontDummy;//make fake cursor and set to front
    Node* fakecursor2=R.frontDummy;
    while(fakecursor1->next!=nullptr)
    {
        fakecursor1=fakecursor1->next;
        fakecursor2=fakecursor2->next;
        if(fakecursor1->data!=fakecursor2->data)//checking to see if the two lists are equal at the same nodes
        {
            return false;
        }
    }
    return true;
}

//*//OVERRIDDEN OPERATORS

// Inserts string representation of L into stream.
ostream& operator<<( ostream& stream, List& L )
{
    return stream<<L.to_string();
}
// Returns true if and only if A is the same integer sequence as B. The 
// cursors in both Lists are unchanged are unchanged.
bool operator==( List& A, const List& B )
{
    return A.equals(B);
}
// Overwrites the state of this List with state of L.
List& List::operator=( const List& L )
{
    if(this != &L)//ensures that the two lists are not the same
    {
        List Temp=L;//makes a copy of L
        //swaps the copy fields with the fields of this
        swap(frontDummy,Temp.frontDummy);
        swap(backDummy,Temp.backDummy);
        swap(beforeCursor,Temp.beforeCursor);
        swap(afterCursor,Temp.afterCursor);
        swap(pos_cursor,Temp.pos_cursor);
        swap(num_elements,Temp.num_elements);
    }
    return *this;
}