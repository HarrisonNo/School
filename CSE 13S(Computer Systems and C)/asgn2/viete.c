//This contains the implementation of Viète’s formula to approximate π and the function to return the number of computed factors.
#include "mathlib.h"
static int VietePICounter = 0;

double pi_viete(void) {
    double PIValue = 1;
    double RecentValue = 0,
           RecentNumerator = 0; //Separate numerator to allow for more efficient looping
    for (
        int Loop = 1; absolute(1 - RecentValue) > EPSILON;
        Loop++) //RecentValue grows infinitely close to 1 so stop once the difference is less than EPSILON
    {
        RecentNumerator = sqrt_newton(2 + RecentNumerator);
        RecentValue = RecentNumerator / 2;
        PIValue *= RecentValue;
        VietePICounter++;
    }
    PIValue /= 2; //Divide by 2 and then flip to get final PI value
    PIValue = 1 / PIValue;
    return PIValue;
}

int pi_viete_factors(void) {
    return VietePICounter;
}
