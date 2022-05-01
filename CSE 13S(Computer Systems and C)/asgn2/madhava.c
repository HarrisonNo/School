//This contains the implementation of the Madhava series to approximate π and the function to return the number of computed terms.
#include "mathlib.h"
static int MadhavaPICounter = 0;

double pi_madhava(void) {
    double PIValue = 0;
    double RecentValue = 1; //Set to 1 just to pass initial for condition
    double RecentNumerator
        = -3,
        RecentDenominator; //Separate the two to have an easier time with exponents in the numerator
    for (double Loop = 0; absolute(RecentValue) > EPSILON; Loop++) {
        RecentNumerator /= -3; //easier way of doing exponents, relying on the previous value
        RecentDenominator = (2 * Loop + 1);
        RecentValue = RecentNumerator / RecentDenominator;
        PIValue += RecentValue;
        MadhavaPICounter++;
    }
    PIValue *= sqrt_newton(12); //Multiply by square root of 12 to get final pi value
    return PIValue;
}

int pi_madhava_terms(void) {
    return MadhavaPICounter;
}
