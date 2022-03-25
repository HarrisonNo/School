/***********************************************************************
 * Harrison Quiring, hquiring
 * 2021 Spring CSE 101 pa5
 * BigInteger.cpp
 * Functions comprising the C++ BigInteger ADT
 * ***********************************************************************/
//#include <string>
#include"BigInteger.h"
#include <cmath>
using namespace std;

//GLOBAL VARIABLES
long power=1;//can be 1<=power<=9,SET TO 9 BEFORE SUBMITTING
long base=pow(10,power);//SET TO 1 BILLION BEFORE SUBMITTING

// Class Constructors & Destructors ----------------------------------------

// Constructor that creates a new BigInteger in the zero state: 
// signum=0, digits=().
BigInteger::BigInteger()
{
    signum=0;
    List digits;
}
// Constructor that creates a new BigInteger from the string s.
// pre: s is a non-empty string consisting of (at least one) base 10 digit
// {0,1,2,3,4,5,6,7,8,9}, and an optional sign {+,-} prefix.
BigInteger::BigInteger(string s)
{
    if(s.length()==0)
    {
        cerr<<"BigInteger Error: constructor called on empty string";
    }
    if(s.find_first_not_of("+-0123456789")!=string::npos)//finds first non number character, SOURCE:https://stackoverflow.com/questions/8888748/how-to-check-if-given-c-string-or-char-contains-only-digits
    {
        cerr<<"BigInteger Error: constructor called on non-numeric string";
    }
    int counter=0;
    long minstringlength=0;
    string holdingstring;
    string CopyOfS=s;
    if(s[0]=='+' && s.find_first_not_of("+-0")!=string::npos)//checking to see if there are any numbers in addition to a +/-
    {
        minstringlength=1;//need to offset the loop so that it doesn't attempt to add +/- into a long
        signum=1;
    }
    else if(s[0]=='-' && s.find_first_not_of("+-0")!=string::npos)
    {
        minstringlength=1;
        signum=-1;
    }
    else if(s.find_first_not_of("0")!=string::npos)
    {
        signum=1;
    }
    while(CopyOfS.size()>unsigned(minstringlength))//goes from right to left in the string
    {
        if(counter==power)//in order to maintatin the correct power of the individual integers I append them to a string and then insert the string into the list
        {
            digits.insertAfter(stoi(holdingstring));
            counter=0;
            holdingstring.clear();
        }
        counter++;
        holdingstring.insert(0,1,CopyOfS.back());//inserting the back of the string(that holds all the numbers) into the front of the string that holds the numbers to be inserted into a single list node 
        CopyOfS.pop_back();
    }
    if(holdingstring.length()!=0)//need to check if the holding string is empty, it will not be empty if the biginteger is not evenly divisible by the power
    {
        digits.insertBefore(stoi(holdingstring));
    }
}
// Constructor that copies the BigInteger N.
BigInteger::BigInteger(const BigInteger& N)
{
    signum=N.signum;
    digits=N.digits;//relies on an overloaded operator from the list ADT
}
// Destructor (optional)
//~BigInteger()

// Access functions --------------------------------------------------------

// Returns -1, 1 or 0 according to whether this BigInteger is positive, 
// negative or 0, respectively.
int BigInteger::sign()
{
    return signum;
}
// Returns -1, 1 or 0 according to whether this BigInteger is less than N,
// greater than N or equal to N, respectively.
int BigInteger::compare(BigInteger N)
{
    if(signum!=N.signum)//checking if the two signs are different
    {
        if(signum==1)
        {
            return 1;
        }
        else if(signum==-1)
        {
            return -1;
        }
        else//checking if top is zero
        {
            if(N.signum==1)
            {
                return -1;
            }
            else if(N.signum==-1)
            {
                return 1;
            }
        }
    }
    if(digits.size()>N.digits.size())
    {
        return 1;//returning greater than
    }
    else if(digits.size()<N.digits.size())
    {
        return -1;//returning less than
    }
    digits.moveFront();
    N.digits.moveFront();
    int mainlistval, seclistval;
    for(int i=0;i<digits.size();i++)//checking each member of each digits list
    {
        mainlistval=digits.moveNext();
        seclistval=N.digits.moveNext();
        if(mainlistval>seclistval)
        {
            //ReturnToStartingCursorPos();
            return 1;
        }
        else if(mainlistval<seclistval)
        {
            //ReturnToStartingCursorPos();
            return -1;
        }
    }
    //ReturnToStartingCursorPos();
    return 0;//returning equal to
}

// Manipulation procedures -------------------------------------------------

// Re-sets this BigInteger to the zero state.
void BigInteger::makeZero()
{
    digits.clear();
    signum=0;
}
// If this BigInteger is zero, does nothing, otherwise reverses the sign of 
// this BigInteger positive <--> negative. 
void BigInteger::negate()
{
    if(signum!=0)
    {
        signum*=-1;
    }
}

// BigInteger Arithmetic operations ----------------------------------------

// Returns a BigInteger representing the sum of this and N.
BigInteger BigInteger::add(const BigInteger& N)
{
    BigInteger returnBI(*this), nBI(N);
    nBI.digits.moveBack();
    returnBI.digits.moveBack();
    if(signum!=N.signum || signum==0)//checks if there are not the same or if one is zero, logic is to pass as much off as possible to sub() so that all weird subtracting exceptions are handled there
    {
        if(signum==0)
        {
            returnBI=N;
            return returnBI;
        }
        else if(N.signum==0)
        {
            returnBI=*this;
            return returnBI;
        }
        if(signum==1)
        {
            nBI.negate();//flip sign of second BI
            return returnBI.sub(nBI);
        }
        else
        {
            nBI.negate();
            return nBI.sub(returnBI);
        }
    }
    int addtotal, newleftvalue;
    while(returnBI.digits.position()>0 && nBI.digits.position()>0)//loop through list starting at back
    {
        addtotal=nBI.digits.movePrev()+returnBI.digits.movePrev();
        if(addtotal>=base)//check to see if added total is higher than the base
        {
            addtotal-=base;
            if(returnBI.digits.position()!=0)
            {
                newleftvalue=returnBI.digits.peekPrev()+1;
                returnBI.digits.eraseBefore();
                returnBI.digits.insertBefore(newleftvalue);//setting the value on the left to be the old value +1 to account for the carryover
            }
            else
            {
                returnBI.digits.insertBefore(1);
            }
        }
        returnBI.digits.eraseAfter();
        returnBI.digits.insertAfter(addtotal);
    }
    while(nBI.digits.position()>0)
    {
        returnBI.digits.insertAfter(nBI.digits.movePrev());
    }
    return returnBI;
}
// Returns a BigInteger representing the difference of this and N.
BigInteger BigInteger::sub(const BigInteger& N)
{
    BigInteger thisBI(*this), nBI(N);
    nBI.digits.moveBack();
    thisBI.digits.moveBack();
    if(signum==0)
    {
        thisBI=N;
        return thisBI;
    }
    else if(N.signum==0)
    {
        return thisBI;
    }
    if(N.signum==-1)
    {
        nBI.negate();//turning copy of N positive
        if(thisBI.signum==1)
        {
            return thisBI.add(nBI);
        }
        else
        {
            thisBI.negate();
            return nBI.sub(thisBI);//doing some serious flips, but this should ensure that I am subtracting two positive numbers which hopefully will make the logic of sub() way easier to create
        }
    }
    else if(signum==-1)//subtracting a negative and a positive is the same as adding two negatives
    {
        nBI.negate();
        return thisBI.add(nBI);
    }
    //actual logic of sub begins here, previous bit was to ensure that sub was taking two pos lists
    //this is my attempt to do sub with none of the helper functions
    long resultingvalue,thisBIvalue,nBIvalue,lengthdiff=abs(thisBI.digits.size()-nBI.digits.size());
    int compareresult;
    bool subtracted=false;
    compareresult=thisBI.compare(nBI);
    BigInteger returnBI;
    if(compareresult==0)//if the two are equal then immediately return a zero BigInt
    {
        return returnBI;//returns empty BigInt
    }
    thisBI.digits.moveFront();
    nBI.digits.moveFront();
    for(int i=0;i<lengthdiff;i++)//adding zeroes to the front of one of the BigInts if they have different amounts of nodes
    {
        if(compareresult==1)
        {
            nBI.digits.insertAfter(0);
        }
        else if(compareresult==-1)
        {
            thisBI.digits.insertAfter(0);
        }
    }
    if(compareresult==1)
    {
        returnBI.signum=1;
    }
    else
    {
        returnBI.signum=-1;
    }
    thisBI.digits.moveBack();
    nBI.digits.moveBack();
    //loops until it hits the end of one of the lists
    while(thisBI.digits.position()>0 && nBI.digits.position()>0)//loop through list starting at back
    {
        thisBIvalue=thisBI.digits.movePrev();
        nBIvalue=nBI.digits.movePrev();
        if(compareresult==1)//if this is greater than n
        {
            if(thisBIvalue>nBIvalue)
            {
                resultingvalue=thisBIvalue-nBIvalue;
                returnBI.digits.insertAfter(resultingvalue);
                if(thisBI.digits.position()>0)
                {
                    subtracted=true;
                }
                //cout<<"ATHE OUTPUT LIST SO FAR IS"<<returnBI<<endl;//DEBUGGING
            }
            if(thisBIvalue<nBIvalue)
            {
                resultingvalue=base-nBIvalue+thisBIvalue;
                //cout<<"THE VALUE TO BE ADDED IS "<<resultingvalue<<endl;//DEBUGGING
                returnBI.digits.insertAfter(resultingvalue);
                if(thisBI.digits.position()>0)
                {
                    subtracted=true;
                }
                //cout<<"BTHE OUTPUT LIST SO FAR IS"<<returnBI<<endl;//DEBUGGING
            }
            else//if the two nodes are equal
            {
                returnBI.digits.insertAfter(0);
                //cout<<"CTHE OUTPUT LIST SO FAR IS"<<returnBI<<endl;//DEBUGGING
            }
        }
        else if(compareresult==-1)//if less than
        {
            if(thisBIvalue>nBIvalue)
            {
                resultingvalue=base-thisBIvalue+nBIvalue;
                returnBI.digits.insertAfter(resultingvalue);
                if(thisBI.digits.position()>0)
                {
                    subtracted=true;
                }
                //cout<<"DTHE OUTPUT LIST SO FAR IS"<<returnBI<<endl; //DEBUGGING
            }
            if(thisBIvalue<nBIvalue)
            {
                resultingvalue=nBIvalue-thisBIvalue;
                returnBI.digits.insertAfter(resultingvalue);
                if(thisBI.digits.position()>0)
                {
                    subtracted=true;
                }
                //cout<<"ETHE OUTPUT LIST SO FAR IS"<<returnBI<<endl;//DEBUGGING
            }
            else//if the two nodes are equal
            {
                returnBI.digits.insertAfter(0);
                //cout<<"FTHE OUTPUT LIST SO FAR IS"<<returnBI<<endl; //DEBUGGING
            }
        }
    }
    //borrowing from very front
    thisBI.digits.moveFront();
    nBI.digits.moveFront();
    long numberoferasedzeroes=0;
    while(returnBI.digits.peekNext()==0)//for whatever reason it insists on adding a zero to the front so this is to take care of any unnecessary zeroes
    {
        returnBI.digits.eraseAfter();
        numberoferasedzeroes++;
    }
    if(subtracted==true && numberoferasedzeroes<2)
    {
        returnBI.digits.insertBefore(returnBI.digits.peekNext()-1);//modifying only the first node initially if it made any subtraction
        returnBI.digits.eraseAfter();
        returnBI.digits.moveFront();
        //now modifying the second node
        if(returnBI.digits.moveNext()==0)
        {
            returnBI.digits.eraseBefore();
        }
        if(returnBI.digits.peekNext()>0)
        {
            returnBI.digits.insertBefore(returnBI.digits.peekNext()-1);
            returnBI.digits.eraseAfter();
        }
        else
        {
            returnBI.digits.insertBefore(base-1);
            returnBI.digits.eraseAfter();
        }
    }
    return returnBI;
}
// Returns a BigInteger representing the product of this and N. 
BigInteger BigInteger::mult(const BigInteger& N)
{
    BigInteger returnBI;
    if(signum==0 || N.signum==0)
    {
        return returnBI;//immediately returns zero if one is zero
    }
    BigInteger thisBI(*this), nBI(N);
    nBI.digits.moveBack();
    thisBI.digits.moveBack();
    if(thisBI.signum==-1 || nBI.signum==-1)//making sign negative if one of the BIs is negative
    {
        returnBI.signum=-1;
    }
    else
    {
        returnBI.signum=1;
    }
    long thisBIvalue, nBIvalue, totalvalue;
    long thisBIposition=-1,nBIposition=-1;//since the multiplication starts at the back, I am keeping separate position variables that count the number of times-1 it has called movePrev
    while(nBI.digits.position()!=0)//looping through second(outer) number
    {
        nBIvalue=nBI.digits.movePrev();
        nBIposition++;
        nBIvalue=nBIvalue*pow(base,nBIposition);//setting the true value of the node
        thisBI.digits.moveBack();
        thisBIposition=-1;
        while(thisBI.digits.position()!=0)//looping through first(inner) number
        {
            thisBIvalue=thisBI.digits.movePrev();
            //cout<<"INITIAL thisBIvalue IS "<<thisBIvalue<<endl;//DEBUGGING
            thisBIposition++;
            thisBIvalue=thisBIvalue*pow(base,thisBIposition);//setting the true value of the node
            //calculating the multiplied value
            totalvalue=thisBIvalue*nBIvalue;
            //cout<<"ADDING "<<totalvalue<<" TO THE PREVIOUS VALUE, CALCULATED BY MULTIPLYING "<<thisBIvalue<<" AND "<<nBIvalue<<endl;//DEBUGGING
            BigInteger tempBI(std::to_string(totalvalue));
            tempBI.signum=returnBI.signum;
            returnBI+=tempBI;
        }
    }
    //cout<<"ENDING CURRENT LOOP\n";//DEBUGGING
    return returnBI;
}

// Other Functions ---------------------------------------------------------

// Returns a string representation of this BigInteger consisting of its
// base 10 digits. If this BigInteger is negative, the returned string 
// will begin with a negative sign '-'. If this BigInteger is zero, the
// returned string will consist of the character '0' only.
string BigInteger::to_string()
{
    string returnedstring,tempstring;
    if(signum==0)
    {
        return "0";
    }
    else if(signum==-1)
    {
        returnedstring.push_back('-');
    }
    digits.moveFront();
    for(int i=0;i<digits.size();i++)
    {
        tempstring=std::to_string(digits.moveNext());//need to add into a temp node first in order to check if it has enough 0's in front
        if(digits.position()!=1)//check to make sure to not add insignificant 0's if at the very front
        {
            while(tempstring.length()<unsigned(power))
            {
                tempstring.insert(0,1,'0');//adding enough insignifcant 0's to this node
            }
        }
        returnedstring.append(tempstring);//iterates through each node in the list and converts it to string then appends it to returnedstring
    }
    return returnedstring;
}

// Overriden Operators -----------------------------------------------------

// Inserts string representation of N into stream.
std::ostream& operator<<( std::ostream& stream, BigInteger N )
{
    return stream<<N.to_string();      
}
// Returns true if and only if A equals B. 
bool operator==( BigInteger& A, const BigInteger& B )
{
    if(A.signum!=B.signum)
    {
        return false;
    }
    if(A.digits==B.digits)//checks the two lists using the list ADT
    {
        return true;
    }
    return false;
}
// Returns true if and only if A is less than B. 
bool operator<( BigInteger& A, const BigInteger& B )
{
    if(A==B)//checking if equal
    {
        return false;
    }
    List fakeBlist=B.digits;
    A.digits.moveFront();
    fakeBlist.moveFront();
    for(int i=0;i<A.digits.size();i++)
    {
        if(A.digits.moveNext()*A.signum>fakeBlist.moveNext()*B.signum)//checking if greater than
        {
            return false;
        }
    }
    return true;
}
// Returns true if and only if A is less than or equal to B. 
bool operator<=( BigInteger& A, const BigInteger& B )
{
    if(A==B)
    {
        return true;
    }
    List fakeBlist=B.digits;
    A.digits.moveFront();
    fakeBlist.moveFront();
    for(int i=0;i<A.digits.size();i++)
    {
        if(A.digits.moveNext()*A.signum>fakeBlist.moveNext()*B.signum)//checking if greater than
        {
            return false;
        }
    }
    return true;
}
// Returns true if and only if A is greater than B. 
bool operator>( BigInteger& A, const BigInteger& B )
{
    if(A==B)
    {
        return false;
    }
    List fakeBlist=B.digits;
    A.digits.moveFront();
    fakeBlist.moveFront();
    for(int i=0;i<A.digits.size();i++)
    {
        if(A.digits.moveNext()*A.signum<fakeBlist.moveNext()*B.signum)//checking if less than
        {
            return false;
        }
    }
    return true;
}
// Returns true if and only if A is greater than or equal to B. 
bool operator>=( BigInteger& A, const BigInteger& B )
{
    if(A==B)
    {
        return true;
    }
    List fakeBlist=B.digits;
    A.digits.moveFront();
    fakeBlist.moveFront();
    for(int i=0;i<A.digits.size();i++)
    {
        if(A.digits.moveNext()*A.signum<fakeBlist.moveNext()*B.signum)//checking if less than
        {
            return false;
        }
    }
    return true;
}
// Returns the sum A+B. 
BigInteger operator+( BigInteger A, const BigInteger& B )
{
    return A.add(B);
}
// Overwrites A with the sum A+B. 
BigInteger operator+=( BigInteger& A, const BigInteger& B )
{
    A=A.add(B);
    return A;
}
// Returns the difference A-B. 
BigInteger operator-( BigInteger A, const BigInteger& B )
{
    return A.sub(B);
}
// Overwrites A with the difference A-B. 
BigInteger operator-=( BigInteger& A, const BigInteger& B )
{
    A=A.sub(B);
    return A;
}
// Returns the product A*B. 
BigInteger operator*( BigInteger A, const BigInteger& B )
{
    return A.mult(B);
}
// Overwrites A with the product A*B. 
BigInteger operator*=( BigInteger& A, const BigInteger& B )
{
    A=A.mult(B);
    return A;
}