//This contains the main()function which tests each of your math library functions.
#include "mathlib.h"

#include <stdio.h>
#include <stdlib.h> //necessary for strtod

#define _USE_MATH_DEFINES //Necessary to use math.h defines for some reason
#include <math.h>
#include <unistd.h>

#define OPTIONS "aebmrvshn"

void PrintWithFormat(char *, char *, int, double, double, double, int);
void PrintHelpPage();

int main(int NumberOfArgs, char *Strings[]) {
    if (NumberOfArgs == 1) //user input nothing(besides command to run program)
    {
        PrintHelpPage();
        return 0; //exit program if nothing was specified after printing help page
    }
    int opt = 0; //This is taken from the Long's asgn2.pdf
    int Verbose = 0;
    int Tests[6] = {
        0, 0, 0, 0, 0, 0
    }; //Represents true/false for tests: e, pi_euler, pi_bbp, pi_madhava, pi_viete, square_root_newton
    //The reason for storing in an array is to ensure that everything prints in the same exact order as the binary since the getopt would otherwise just print the first test the user requests
    while ((opt = getopt(NumberOfArgs, Strings, OPTIONS)) != -1) {
        switch (opt) {
        case 'e': //Runs e approximation test.
            Tests[0] = 1;
            break;
        case 'r': //Runs Euler sequence π approximation test.
            Tests[1] = 1;
            break;
        case 'b': //Runs Bailey-Borwein-Plouffe π approximation test.
            Tests[2] = 1;
            break;
        case 'm': //Runs Madhava π approximation test.
            Tests[3] = 1;
            break;
        case 'v': //Runs Viète π approximation test.
            Tests[4] = 1;
            break;
        case 'n': //Runs Newton-Raphson square root approximation tests.
            Tests[5] = 1;
            break;
        case 'h': //Display a help message detailing program usage.
            PrintHelpPage(); //Exit program after displaying help message
            return 0;
        case 'a': //Runs all tests.
            for (int i = 0; i < 6; i++) {
                Tests[i] = 1;
            } //Enables all tests
            break;
        case 's': //Enable printing of statistics to see computed terms and factors for each tested function.
            Verbose = 1; //Sets verbose to true
            break;
        }
    }
    double PIValue, SquareValue, EValue;
    if (Tests[0]
        == 1) //This could absolutely be done with storing much of this in different arrays and dictionaries but I also don't care enough
    {
        EValue
            = e(); //set a variable here just so that it does not need to recalculate the variable two different times
        PrintWithFormat("e()", "M_E", Verbose, EValue, M_E, absolute(EValue - M_E),
            e_terms()); //Really condense way of doing this, but imo looks a lot nicer than manually printing this out every single time
    }
    if (Tests[1] == 1) {
        PIValue = pi_euler();
        PrintWithFormat("pi_euler()", "M_PI", Verbose, PIValue, M_PI, absolute(PIValue - M_PI),
            pi_euler_terms());
    }
    if (Tests[2] == 1) {
        PIValue = pi_bbp();
        PrintWithFormat(
            "pi_bbp()", "M_PI", Verbose, PIValue, M_PI, absolute(PIValue - M_PI), pi_bbp_terms());
    }
    if (Tests[3] == 1) {
        PIValue = pi_madhava();
        PrintWithFormat("pi_madhava()", "M_PI", Verbose, PIValue, M_PI, absolute(PIValue - M_PI),
            pi_madhava_terms());
    }
    if (Tests[4] == 1) {
        PIValue = pi_viete();
        PrintWithFormat("pi_viete()", "M_PI", Verbose, PIValue, M_PI, absolute(PIValue - M_PI),
            pi_viete_factors());
    }
    if (Tests[5] == 1) //Newton-Raphson square
    {
        for (double Count = 0; Count <= 10; Count += .1) {
            SquareValue = sqrt_newton(Count);
            printf("sqrt_newton(%0.6f) = %0.15f, sqrt(%0.6f) = %0.15f, diff = %0.15f\n", Count,
                SquareValue, Count, sqrt(Count),
                absolute(
                    SquareValue
                    - sqrt(
                        Count))); //Basically the exact same as PrintWithFormat with the exception that it has to print two additional variables
            if (Verbose) {
                printf("sqrt_newton terms = %i\n", sqrt_newton_iters());
            }
        }
    }
    return 0;
}

void PrintWithFormat(char *FirstString, char *SecondString, int Verbose, double FirstStringValue,
    double SecondStringValue, double ThirdStringValue, int VerboseValue) {
    printf("%s = %0.15f, %s = %0.15f, diff = %0.15f\n", FirstString, FirstStringValue, SecondString,
        SecondStringValue,
        ThirdStringValue); //Print all necessary values according to the binary format
    if (Verbose) {
        printf("%s terms = %i\n", FirstString, VerboseValue); //Print verbose value if needed
    }
}
void PrintHelpPage() {
    printf("SYNOPSIS\n");
    printf("\tA test harness for the small numerical library.\n\n");
    printf("USAGE\n");
    printf("\t./mathlib-test [-aebmrvnsh]\n\n");
    printf("OPTIONS\n");
    printf("\t-a\tRuns all tests.\n");
    printf("\t-e\tRuns e tests.\n");
    printf("\t-b\tRuns BBP pi test.\n");
    printf("\t-m\tRuns Madhava pi test.\n");
    printf("\t-r\tRuns Euler pi test.\n");
    printf("\t-v\tRuns Viete pi test.\n");
    printf("\t-n\tRuns Newton square root tests.\n");
    printf("\t-s\tPrints verbose statistcs.\n");
    printf("\t-h\tDisplay program synopsis and usage.\n");
}
