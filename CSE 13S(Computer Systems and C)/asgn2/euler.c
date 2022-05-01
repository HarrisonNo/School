//This contains the implementation of Euler’s solution used to approximate π and the function to return the number of computed terms.
#include "mathlib.h"
static int EulerPICounter = 0;

double pi_euler(void) {
    double PIValue = 0;
    double RecentValue = 1;
    for (
        double Loop = 1; RecentValue > EPSILON;
        Loop++) //C apparently doesn't have implicit conversion so the loop value has to be a double
    {
        RecentValue = (1 / (Loop * Loop));
        PIValue += RecentValue;
        EulerPICounter++;
    }
    PIValue *= 6; //multiply it by 6 and square root to get the final value of pi
    PIValue = sqrt_newton(PIValue);
    return PIValue;
}

int pi_euler_terms(void) {
    return EulerPICounter;
}
