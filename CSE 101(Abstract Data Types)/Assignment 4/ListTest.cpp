/***********************************************************************
 * Harrison Quiring, hquiring
 * 2021 Spring CSE 101 pa4
 * ListTest.cpp
 * Test file for functions comprising the C++ List ADT
 * ***********************************************************************/
#include "List.cpp"
#include <iostream>
using namespace std;
int main()
{
    List MainEmptyList;
    List A;

    cout<<"isEmpty() test:";
    A=MainEmptyList;
    if(A.isEmpty()==false)
        cout<<" Failed\n";
    else
        cout<<" Passed\n";

    cout<<"size() test:";
    A=MainEmptyList;
    if(A.size()==0)
    {
        A.insertAfter(1);
        if(A.size()==1)
        {
            A.insertBefore(2);
            if(A.size()==2)
                cout<<" Passed\n";
            else
                cout<<" Failed\n";
        }
        else
            cout<<" Failed\n";
    }
    else
        cout<<" Failed\n";
    
    cout<<"position() test:";
    A=MainEmptyList;
    if(A.position()==0)
    {
        A.insertAfter(1);
        A.moveNext();
        if(A.position()==1)
        {
            A.insertBefore(2);
            if(A.position()==2)
                cout<<" Passed\n";
            else
                cout<<" Failed\n";
        }
        else
            cout<<" Failed\n";
    }
    else
        cout<<" Failed\n";
    
    cout<<"insertAfter() test:";
    A=MainEmptyList;
    A.insertAfter(1);
    A.insertAfter(2);
    A.insertAfter(3);
    A.insertAfter(4);
    cout<<" Passed\n";

    cout<<"insertBefore() test:";
    A=MainEmptyList;
    A.insertBefore(1);
    A.insertBefore(2);
    A.insertBefore(3);
    A.insertBefore(4);
    cout<<" Passed\n";

    cout<<"moveFront() test:";
    A=MainEmptyList;
    A.insertBefore(5);
    A.insertBefore(4);
    A.insertBefore(3);
    A.insertBefore(2);
    A.insertBefore(1);
    A.moveFront();
    if(A.position()!=0)
        cout<<" Failed\n";
    else
        cout<<" Passed\n";
    
    cout<<"moveBack() test:";
    A=MainEmptyList;
    A.insertAfter(5);
    A.insertAfter(4);
    A.insertAfter(3);
    A.insertAfter(2);
    A.insertAfter(1);
    A.moveBack();
    if(A.position()!=5)
        cout<<" Failed\n";
    else
        cout<<" Passed\n";
    
    cout<<"peekNext() test:";
    A=MainEmptyList;
    A.insertAfter(0);
    if(A.peekNext()==0)
    {
        A.moveNext();
        A.insertAfter(1);
        if(A.peekNext()==1)
        {
            A.moveNext();
            A.insertAfter(2);
            if(A.peekNext()==2)
                cout<<" Passed\n";
            else
                cout<<" Failed\n";
        }
        else
            cout<<" Failed\n";
    }
    else
        cout<<" Failed\n";

    cout<<"peekPrev() test:";
    A=MainEmptyList;
    A.insertBefore(0);
    if(A.peekPrev()==0)
    {
        A.movePrev();
        A.insertBefore(1);
        if(A.peekPrev()==1)
        {
            A.movePrev();
            A.insertBefore(2);
            if(A.peekPrev()==2)
                cout<<" Passed\n";
            else
                cout<<" Failed\n";
        }
        else
            cout<<" Failed\n";
    }
    else
        cout<<" Failed\n";

    cout<<"moveNext() test:";
    A=MainEmptyList;
    A.insertAfter(1);
    A.insertAfter(2);
    A.insertAfter(3);
    A.insertAfter(4);
    A.moveNext();
    A.moveNext();
    A.moveNext();
    A.moveNext();
    cout<<" Passed\n";

    cout<<"movePrev() test:";
    A=MainEmptyList;
    A.insertBefore(1);
    A.insertBefore(2);
    A.insertBefore(3);
    A.insertBefore(4);
    A.movePrev();
    A.movePrev();
    A.movePrev();
    A.movePrev();
    cout<<" Passed\n";

    cout<<"eraseAfter() test:";
    A=MainEmptyList;
    A.insertAfter(1);
    A.insertAfter(2);
    A.insertAfter(3);
    A.insertAfter(4);
    A.eraseAfter();
    if(A.moveNext()==3)
    {
        A.eraseAfter();
        if(A.moveNext()==1)
            cout<<" Passed\n";
        else
            cout<<" Failed\n";
    }
    else
        cout<<" Failed\n";

    cout<<"eraseBefore() test:";
    A=MainEmptyList;
    A.insertBefore(1);
    A.insertBefore(2);
    A.insertBefore(3);
    A.insertBefore(4);
    A.eraseBefore();
    if(A.movePrev()==3)
    {
        A.eraseBefore();
        if(A.movePrev()==1)
            cout<<" Passed\n";
        else
            cout<<" Failed\n";
    }
    else
        cout<<" Failed\n";

    cout<<"findNext() test:";
    A=MainEmptyList;
    A.insertAfter(4);
    A.insertAfter(3);
    A.insertAfter(2);
    A.insertAfter(1);
    A.findNext(2);
    if(A.position()==2)
    {
        A.findNext(4);
        if(A.position()==4)
            cout<<" Passed\n";
        else
            cout<<" Failed\n";
    }
    else
        cout<<" Failed\n";

    cout<<"findPrev() test:";
    A=MainEmptyList;
    A.insertBefore(4);
    A.insertBefore(3);
    A.insertBefore(2);
    A.insertBefore(1);
    A.findPrev(2);
    if(A.position()==2)
    {
        A.findPrev(4);
        if(A.position()==0)
            cout<<" Passed\n";
        else
            cout<<" Failed\n";
    }
    else
        cout<<" Failed\n";

    cout<<"cleanup() test:";
    A=MainEmptyList;
    A.insertAfter(4);
    A.insertAfter(3);
    A.insertAfter(2);
    A.insertAfter(1);
    A.insertBefore(4);
    A.insertBefore(3);
    A.insertBefore(2);
    A.insertBefore(1);
    A.cleanup();
    if(A.size()==4)
        cout<<" Passed\n";
    else
        cout<<" Failed\n";

    cout<<"clear() test:";
    A=MainEmptyList;
    A.insertAfter(4);
    A.insertAfter(3);
    A.insertAfter(2);
    A.insertAfter(1);
    A.insertBefore(4);
    A.insertBefore(3);
    A.insertBefore(2);
    A.insertBefore(1);
    A.clear();
    if(A.size()==0)
        cout<<" Passed\n";
    else
        cout<<" Failed\n";

    cout<<"to_string() test:";
    A=MainEmptyList;
    A.insertAfter(4);
    A.insertAfter(3);
    A.insertAfter(2);
    A.insertAfter(1);
    string teststring="1 2 3 4 ";
    if(A.to_string()==teststring)
        cout<<" Passed\n";
    else
        cout<<" Failed\n";

    cout<<"equals() test:";
    A=MainEmptyList;
    List B=MainEmptyList;
    if(B.equals(A)==true)
        cout<<" Passed\n";
    else
        cout<<" Failed\n";

    return 0;
}