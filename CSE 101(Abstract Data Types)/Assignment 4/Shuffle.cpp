/***********************************************************************
 * Harrison Quiring, hquiring
 * 2021 Spring CSE 101 pa4
 * Shuffle.cpp
 * Client that takes 'n' from user and calculates the number of shuffles required to get back to original order of n-sized deck
 * ***********************************************************************/
#include <iostream>
#include "List.h"
using namespace std;


void Shuffle(List& D);
bool IsInCorrectOrder(List &D);

int main(int argc,char *argv[])
{
    //fileio stuff
    if( argc != 2 )
    {
      cerr << "Usage: "<<argv[0]<<" <int>"<<endl;
      return(EXIT_FAILURE);
    }
    int n=strtol(argv[1],argv,10);//taken from http://www.cplusplus.com/reference/cstdlib/strtol/
 
    //printing start of table
    cout<<"deck size\tshuffle count\n";//\t stands for a tab, who would have thunk
    for(int i=0;i<30;i++)//max of 10 is rather arbitrary but should be enough
    {
        cout<<"-";
    }
    cout<<endl;

    List L;
    for(int i=1;i<=n;i++)
    {
        cout<<i<<"\t\t";//printing deck size
        L.moveBack();
        L.insertBefore(i);
        L.moveFront();
        bool FinishedShuffling=false;
        for(int shufflecount=1;FinishedShuffling==false;shufflecount++)
        {
            Shuffle(L);
            if(IsInCorrectOrder(L))
            {
                cout<<shufflecount<<"\n";
                FinishedShuffling=true;
            }
        }//finished shuffling and got to original list

    }//finished printing up to n decks
    return 0;
}

void Shuffle(List &D)
{
    //Splitting the deck in two
    int Rightmax, Leftmax;
    D.moveFront();
    if(D.size()%2==0)//if even
    {
        Rightmax=Leftmax=D.size()/2;
    }
    if(D.size()%2!=0)//if odd
    {
        Leftmax=(D.size()-1)/2;
        Rightmax=(D.size()+1)/2;
    }
    List LeftList;
    for(int i=0;i<Leftmax;i++)
    {
        LeftList.insertBefore(D.moveNext());
    }
    List RightList;
    for(int i=0;i<Rightmax;i++)
    {
        RightList.insertBefore(D.moveNext());
    }
    //Merging the left and right decks
    D.clear();
    RightList.moveFront();
    LeftList.moveFront();
    for(int i=0;i<RightList.size();i++)
    {
        D.insertBefore(RightList.moveNext());
        if(i<LeftList.size())
        {
            D.insertBefore(LeftList.moveNext());
        }
    }
}

bool IsInCorrectOrder(List &D)
{
    D.moveFront();
    for(int i=1;i<=D.size();i++)
    {
        if(D.moveNext()!=i)
        {
            return false;
        }
    }
    return true;
}