//This contains the implementation of the Bailey-Borwein-Plouffe formula to approximate π and the function to return the number of computed terms.
#include "mathlib.h"
static int BaileyPICounter = 0;

double pi_bbp(void) {
    double PIValue = 0;
    double RecentTotalValue
        = 1; //Starts at 1 just to ensure that it passes the initial for condition
    double RecentRightValue
        = 1,
        RecentLeftValue
        = 16; //Dividing up the formula into left and right hand sides for simplicity, RecentLeftValue starts at 16 since my program divides it
    //by 16 regardless of what loop and the loop should start with the left side being equal to 1
    for (double Loop = 0; RecentTotalValue > EPSILON; Loop++) {
        RecentRightValue = ((Loop * (120 * Loop + 151) + 47)
                            / (Loop * (Loop * (Loop * (512 * Loop + 1024) + 712) + 194)
                                + 15)); //Right hand side rewritten in Horner normal form
        RecentLeftValue
            /= 16; //Relies on the previous value of RecentLeftValue to save on time calculating the exponent
        RecentTotalValue = RecentLeftValue * RecentRightValue;
        PIValue += RecentTotalValue;
        BaileyPICounter++;
    }
    return PIValue;
}

int pi_bbp_terms(void) {
    return BaileyPICounter;
}
