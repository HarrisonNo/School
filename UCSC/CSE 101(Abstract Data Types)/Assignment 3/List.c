/***********************************************************************
 * Harrison Quiring, hquiring
 * 2021 Spring CSE 101 pa3
 * List.c
 * Functions comprising the List ADT
 * ***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "List.h"

typedef struct nodeobject
    {
        int data;
        struct nodeobject* next;
        struct nodeobject* previous;
    }nodeobject;
typedef nodeobject* node;
typedef struct listobject
{
    node front;
    node back;
    node cursor;
    int length;
    int index;
}listobject;



//Constructors-Destructors
List newList(void)
{
    List L;
    L=calloc(1,sizeof(listobject));
    L -> front=NULL;
    L -> back=NULL;
    L->cursor=NULL;
    L -> length=0;
    L->index=-1;
    return L;
}
void freeList(List* pL)
{
    if(pL!=NULL && *pL!=NULL)
    {
        clear(*pL);
        free(*pL);
        *pL=NULL;
    }
}
node newNode(int data)
{
    node N=calloc(1,sizeof(listobject));
    N->data=data;
    N->next=NULL;
    N->previous=NULL;
    return N;
}
void freeNode(node* pN)
{
    if(pN!=NULL && *pN!=NULL)
    {
        free(*pN);
        *pN=NULL;
    }
}


//Access functions

int length(List L)
{
   if(L==NULL)
   {
       printf("List Error: calling getLength() on NULL List reference\n");
       exit(EXIT_FAILURE);
   }
    return (L->length);
}
int index(List L)
{
    if(L->cursor==NULL)
   {
       return (-1);
   }
    return (L->index);
}
int front(List L)
{
    if(L==NULL)
    {
        printf("List Error: calling front() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(length(L)==0)
    {
        printf("List Error: calling front() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    return(L->front->data);
}
int back(List L)
{
   if(L==NULL)
    {
        printf("List Error: calling back() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(length(L)==0)
    {
        printf("List Error: calling back() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    return(L->back->data);
}
int get(List L)
{
    if(length(L)==0)
    {
        printf("List Error: calling get() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    if(index(L)<0)
    {
        printf("List Error: calling get() on an undefined cursor\n");
        exit(EXIT_FAILURE);
    }
    return(L->cursor->data);
}
void moveFront(List L)
{
    if(L==NULL)
    {
        exit(EXIT_SUCCESS);
    }
    if(length(L)>0)
    {
        L->cursor=L->front;
        L->index=0;
    }
}
void moveNext(List L)
{
    if(L->cursor==NULL)
    {
        printf("List Error: calling moveNext() on an undefined cursor\n");
        exit(EXIT_FAILURE);
    }
    if(L->cursor->next!=NULL)
    {
       L->index+=1;
    }
    else
    {
        L->index=-1;
    }
    L->cursor=L->cursor->next;
}
int equals(List A, List B)
{
    if(length(A)!=length(B))
    {
        return 0;//returning false
    }
    moveFront(A);
    moveFront(B);
    for(int i=0;i<length(A);i++)
    {
        if(get(A)!=get(B))
        {
            return 0;//returning false
        }
        moveNext(A);
        moveNext(B);
    }
    moveFront(A);
    moveFront(B);
    return 1;//returning true
}

// Manipulation procedures
void deleteFront(List L)
{
    if(length(L)==0)
    {
        printf("List Error: calling deleteFront() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    node N=L->front;
    if(index(L)>=0)
    {
        L->index--;
    }
    if(length(L)>1)
    {
        L->front=L->front->next;
        L->front->previous=NULL;
    }
    else if(length(L)==1)
    {
        L->back=L->front=NULL;
    }
    L->length--;
    freeNode(&N);
}
void clear(List L)
{
    if(L!=NULL)
    {
        while(length(L)>0)
        {
            moveFront(L);
            delete(L);
        }
    }
}
void set(List L, int x)
{
    if(length(L)==0)
    {
        printf("List Error: calling insertBefore() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    if(index(L)<0)
    {
        printf("List Error: calling insertBefore() on an undefined cursor\n");
        exit(EXIT_FAILURE);
    }   
    L->cursor->data=x;
}

void moveBack(List L)
{
    if(L==NULL)
    {
        exit(EXIT_SUCCESS);
    }
    L->cursor=L->back;
    L->index=length(L)-1;
}
void movePrev(List L)
{
    if(L->cursor==NULL)
    {
        printf("List Error: calling movePrev() on an undefined cursor\n");
        exit(EXIT_FAILURE);
    }
    if(L->cursor->previous!=NULL)
    {
        L->index-=1;
    } 
    L->cursor=L->cursor->previous;
}

void prepend(List L, int x)
{
node N= newNode(x);
    if(L==NULL)
    {
        printf("List Error: calling prepend() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(length(L)==0)
    {
        L->front=L->back=N;
    }
    else
    {
        N->next=L->front;
        L->front->previous=N;
        L->front=N;
    }
    L->length++;
    if(index(L)!=-1)
    {
        L->index++;
    }
}
void append(List L, int x)
{
    node N= newNode(x);
    if(L==NULL)
    {
        printf("List Error: calling append() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(length(L)==0)
    {
        L->front=L->back=N;
    }
    else
    {
        N->previous=L->back;
        L->back->next=N;
        L->back=N;
    }
    L->length++;
}
void insertBefore(List L, int x)
{
    if(length(L)==0)
    {
        printf("List Error: calling insertBefore() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    if(index(L)<0)
    {
        printf("List Error: calling insertBefore() on an undefined cursor\n");
        exit(EXIT_FAILURE);
    }
    node N=newNode(x);
    N->next=L->cursor;
    N->previous=L->cursor->previous;
    L->cursor->previous=N;//Fixing cursor prev.
    L->index++;
    L->length++;
    if(index(L)>1)//adjusting the node to the left of the new node if the new node is not at the start
    {
        movePrev(L);
        movePrev(L);
        L->cursor->next=N;
        moveNext(L); //move cursor back to starting position
        moveNext(L);
    }
    else//readjusting front node since new node is now front
    {
        L->front=N;
    }
}
void insertAfter(List L, int x)
{
    if(length(L)==0)
    {
        printf("List Error: calling insertBefore() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    if(index(L)<0)
    {
        printf("List Error: calling insertBefore() on an undefined cursor\n");
        exit(EXIT_FAILURE);
    }
    node N=newNode(x);
    N->previous=L->cursor;
    N->next=L->cursor->next;
    L->length+=1;
    if(L->cursor->next!=NULL)//adjusting the node to the right of the new node if the new node is not at the end
    {
        L->cursor->next=N;//Fixing cursor prev.
        moveNext(L);
        moveNext(L);
        L->cursor->previous=N;
        movePrev(L); //move cursor back to starting position
        movePrev(L);
    }
    else//readjusting front node since new node is now front
    {
        L->cursor->next=N;//Fixing cursor prev.
        L->back=N;
    }

}
void deleteBack(List L)
{
    if(length(L)==0)
    {
        printf("List Error: calling deleteBack() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    node N=L->back;
    if(index(L)==length(L)-1)
    {
        L->cursor=NULL;
        L->index=-1;
    }
    if(length(L)>1)
    {
        L->back=L->back->previous;
        L->back->next=NULL;
    }
    else if(length(L)==1)
    {
        L->back=L->front=NULL;
    }
    L->length--;
   freeNode(&N);
}
void delete(List L)
{
    if(length(L)==0)
    {
        printf("List Error: calling insertBefore() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    if(index(L)<0)
    {
        printf("List Error: calling insertBefore() on an undefined cursor\n");
        exit(EXIT_FAILURE);
    }
    node locationholder=L->cursor;
    if(locationholder->previous!=NULL)//Adjusting node to the left if it exists
    {
        movePrev(L);
        L->cursor->next=locationholder->next;
        if(locationholder->data==back(L))
        {
            L->back=L->cursor;
        }
        moveNext(L);
    }
    if(locationholder->next!=NULL)//Adjusting node to the right if it exists
    {
        moveNext(L);
        L->cursor->previous=locationholder->previous;
        if(locationholder->data==front(L))
        {
            L->front=L->cursor;
        }
        movePrev(L);
    }
    freeNode(&locationholder);//deleting the cursor node
    L->index=-1;
    L->length--;
}

// Other operations -----------------------------------------------------------

void printList(FILE* out, List L)//SOURCE: https://stackoverflow.com/questions/18950835/printing-to-a-file-in-c/18950872
{
    moveFront(L);
    for(int i=0;i<length(L);i++)
    {
        fprintf(out,"%d ",get(L));//printing to the file
        moveNext(L);
    }
}
List copyList(List L)
{
    List SecondaryList=newList();
    moveFront(L);
    for(int i=0;i<length(L);i++)
    {
        append(SecondaryList,get(L));
        moveNext(L);
    }
    moveFront(L);
    return SecondaryList;
}

void copyIntoExistingList(List L, List EL)
{
    moveFront(L);
    for(int i=0;i<length(L);i++)
    {
        append(EL,get(L));
        moveNext(L);
    }
    moveFront(L);
}