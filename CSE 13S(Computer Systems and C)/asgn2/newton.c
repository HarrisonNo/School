//This contains the implementation of the square root approximation using Newton’s method and the function to return the number of computed iterations.
#include "mathlib.h"
static int NewtonSquareRootCounter = 0;

double sqrt_newton(double InputValue) {
    NewtonSquareRootCounter = 0;
    double RecentValue = 0; //z
    double SquareValue = 1; //y
    while (
        absolute(SquareValue - RecentValue) > EPSILON) //Mostly taken from Long's provided asgn2 pdf
    {
        RecentValue = SquareValue; //z=y
        SquareValue = .5 * (RecentValue + InputValue / RecentValue); //y=.5*(z+x/z)
        NewtonSquareRootCounter++;
    }
    return SquareValue;
}

int sqrt_newton_iters(void) {
    return NewtonSquareRootCounter;
}
