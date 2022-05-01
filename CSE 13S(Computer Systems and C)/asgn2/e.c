//This contains the implementation of the Taylor series to approximate Euler’s number e and the function to return the number of computed terms.
#include "mathlib.h"
static int EulerECounter = 0;

double e(void) {
    double EValue = 1; //Starts at 1 for easier logic in the while loop
    double RecentDenValue = 1;
    for (int Loop = 1; (1 / RecentDenValue) > EPSILON;
         Loop++) //Continues loop until latest value being added is less than epsilon
    {
        RecentDenValue
            *= Loop; //Calculates the new denominator value using previous denominator value
        EValue += 1 / RecentDenValue; //Adds real value to the stored value of E
        EulerECounter++;
    }
    return EValue;
}

int e_terms(
    void) //Basically just a copy of the prev function modified to return the number of terms it went through
{
    return EulerECounter;
}
