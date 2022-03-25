/***********************************************************************
 * Harrison Quiring, hquiring
 * 2021 Spring CSE 101 pa6
 * DictionaryTest.c
 * Test of the functions in the C Dictionary ADT
 * ***********************************************************************/
#include<stdio.h>
#include "Dictionary.h"
int main()
{
    Dictionary D=newDictionary(1);
    Dictionary B=newDictionary(0);
    int testpassed;

    //SIZE TEST
    printf("SIZE TEST:");
    testpassed=1;
    if(size(D)!=0)
        testpassed=0;
    insert(D,"a",1);
    if(size(D)!=1)
        testpassed=0;
    insert(D,"b",2);
    if(size(D)!=2)
        testpassed=0;
    makeEmpty(D);
    if(size(D)!=0)
        testpassed=0;
    
    if(testpassed==1)
        printf("PASSED\n");
    else
        printf("FAILED\n");

    //UNIQUE TEST
    printf("UNIQUE TEST:");
    testpassed=1;
    if(getUnique(D)!=1)
        testpassed=0;
    if(getUnique(B)!=0)
        testpassed=0;
    
    if(testpassed==1)
        printf("PASSED\n");
    else
        printf("FAILED\n");
    
    //LOOKUP TEST
    printf("LOOKUP TEST:");
    testpassed=1;
    insert(D,"d",2);
    insert(D,"z",2);
    insert(D,"a",1);
    insert(D,"b",2);
    insert(D,"c",2);
    if(lookup(D,"d")==VAL_UNDEF)
        testpassed=0;
    if(lookup(D,"c")==VAL_UNDEF)
        testpassed=0;
    if(lookup(D,"y")!=VAL_UNDEF)
        testpassed=0;

    if(testpassed==1)
        printf("PASSED\n");
    else
        printf("FAILED\n");
    makeEmpty(D);
    
    //INSERT TEST
    printf("INSERT TEST:");
    testpassed=1;
    insert(D,"d",3);
    insert(D,"z",5);
    insert(D,"a",6);
    insert(D,"b",22);
    insert(D,"c",2);
    insert(B,"d",2);
    insert(B,"z",2);
    insert(B,"a",1);
    insert(B,"b",2);
    insert(B,"c",2);
    insert(B,"c",5);
    insert(B,"a",12);

    if(testpassed==1)
        printf("PASSED\n");
    else
        printf("FAILED\n");
    makeEmpty(D);
    makeEmpty(B);

    //DELETE TEST
    printf("DELETE TEST:");
    testpassed=1;
    insert(D,"d",3);
    insert(D,"z",5);
    insert(D,"a",6);
    insert(D,"b",22);
    insert(D,"c",2);
    delete(D,"z");
    delete(D,"b");
    insert(B,"d",2);
    insert(B,"z",2);
    insert(B,"a",1);
    insert(B,"b",2);
    insert(B,"c",2);
    insert(B,"c",5);
    insert(B,"a",12);
    delete(D,"a");
    delete(D,"c");

    if(testpassed==1)
        printf("PASSED\n");
    else
        printf("FAILED\n");
    makeEmpty(D);
    makeEmpty(B);

    //MAKEEMPTY TEST
    printf("MAKEEMPTY TEST:");
    testpassed=1;
    insert(D,"d",3);
    insert(D,"z",5);
    insert(D,"a",6);
    insert(D,"b",22);
    insert(D,"c",2);
    if(size(D)!=5)
        testpassed=0;
    makeEmpty(D);
    if(size(D)!=0)
        testpassed=0;
    insert(B,"d",2);
    insert(B,"z",2);
    insert(B,"a",1);
    insert(B,"b",2);
    insert(B,"c",2);
    insert(B,"c",5);
    insert(B,"a",12);
    if(size(B)!=7)
        testpassed=0;
    makeEmpty(B);
    if(size(B)!=0)
        testpassed=0;

    if(testpassed==1)
        printf("PASSED\n");
    else
        printf("FAILED\n");
    makeEmpty(D);
    makeEmpty(B);

    //BEGINFORWARD TEST
    printf("BEGINFORWARD TEST:");
    testpassed=1;
    insert(D,"g",1);
    insert(D,"a",2);
    insert(D,"z",3);
    insert(D,"b",4);
    insert(D,"y",5);
    if(beginForward(D)!=2)
        testpassed=0;
    delete(D,"a");
    if(beginForward(D)!=4)
        testpassed=0;
    delete(D,"b");
    if(beginForward(D)!=1)
        testpassed=0;
    
    if(testpassed==1)
        printf("PASSED\n");
    else
        printf("FAILED\n");
    makeEmpty(D);

    //BEGINREVERSE TEST
    printf("BEGINREVERSE TEST:");
    testpassed=1;
    insert(D,"g",1);
    insert(D,"a",2);
    insert(D,"z",3);
    insert(D,"b",4);
    insert(D,"y",5);
    if(beginReverse(D)!=3)
        testpassed=0;
    delete(D,"z");
    if(beginReverse(D)!=5)
        testpassed=0;
    delete(D,"y");
    if(beginReverse(D)!=1)
        testpassed=0;
    
    if(testpassed==1)
        printf("PASSED\n");
    else
        printf("FAILED\n");
    makeEmpty(D);

    //CURRENTKEY TEST
    printf("CURRENTKEY TEST:");
    testpassed=1;
    insert(D,"g",1);
    insert(D,"a",2);
    insert(D,"z",3);
    insert(D,"b",4);
    insert(D,"y",5);
    beginForward(D);
    if(currentKey(D)!="a")
        testpassed=0;
    next(D);
    if(currentKey(D)!="b")
        testpassed=0;
    prev(D);
    if(currentKey(D)!="a")
        testpassed=0;
    beginReverse(D);
    if(currentKey(D)!="z")
        testpassed=0;
    prev(D);
    if(currentKey(D)!="y")
        testpassed=0;
    
    if(testpassed==1)
        printf("PASSED\n");
    else
        printf("FAILED\n");
    makeEmpty(D);

    //CURRENTVAL TEST
    printf("CURRENTVAL TEST:");
    testpassed=1;
    insert(D,"g",1);
    insert(D,"a",2);
    insert(D,"z",3);
    insert(D,"b",4);
    insert(D,"y",5);
    beginForward(D);
    if(currentVal(D)!=2)
        testpassed=0;
    next(D);
    if(currentVal(D)!=4)
        testpassed=0;
    prev(D);
    if(currentVal(D)!=2)
        testpassed=0;
    beginReverse(D);
    if(currentVal(D)!=3)
        testpassed=0;
    prev(D);
    if(currentVal(D)!=5)
        testpassed=0;
    
    if(testpassed==1)
        printf("PASSED\n");
    else
        printf("FAILED\n");
    makeEmpty(D);

    //NEXT/PREV TEST
    printf("NEXT/PREV TEST:");
    testpassed=1;
    insert(D,"g",1);
    insert(D,"a",2);
    insert(D,"z",3);
    insert(D,"b",4);
    insert(D,"y",5);
    if(beginForward(D)!=2)
        testpassed=0;
    if(next(D)!=4)
        testpassed=0;
    if(prev(D)!=2)
        testpassed=0;
    if(beginReverse(D)!=3)
        testpassed=0;
    if(prev(D)!=5)
        testpassed=0;
    
    if(testpassed==1)
        printf("PASSED\n");
    else
        printf("FAILED\n");
    makeEmpty(D);

    return 0;
}