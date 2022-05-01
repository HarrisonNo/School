#include "names.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEED 2021

int ContainsALetter(char *);

int main(void) {
    char charinputplayers[100],
        charinputseed
            [100]; //The only limit for cleansing user input is that it has to be less than 100 characters, have a feeling that none of the edge cases will attempt an answer longer than 100 characters
    int inputplayers = 0, playerhaswon = 0, playerturnover = 0, randomnumbergenerated;
    unsigned int inputseed = 0;
    int playerpoints[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0 }; //Array is size 10 due to max limit of players and each player starts with 0 points
    int pointsearned[7] = { 0, 0, 10, 10, 15, 5, 5 };
    char rollednames[7][10]
        = { "on side", "on side", "on back", "upright", "on snout", "on ear", "on ear" };

    //User input for number of players, wow this turned out to be a lot of work just to cleanse user input
    fprintf(stdout, "How many players? ");
    fgets(charinputplayers, 100, stdin);
    if (ContainsALetter(&charinputplayers[0]) == 1 || atoi(charinputplayers) < 2
        || atoi(charinputplayers)
               > 10) //If player input contains a character or entered a value less than 2 or greater than 10 then it will default
    {
        fprintf(
            stderr, "Invalid number of players. Using 2 instead.\n"); //Spaces also count as invalid
        inputplayers = 2;
    } else {
        inputplayers = atoi(charinputplayers);
    }
    //User input for seed
    fprintf(stdout, "Random seed: ");
    fgets(charinputseed, 100, stdin);
    if (ContainsALetter(&charinputseed[0]) == 1 || atoi(charinputseed) < 0
        || strtoul(charinputseed, NULL, 10)
               > 4294967295) //If player input contains a character or entered a value less than 2 or greater than the max value for an unsigned int then it will default
    {
        fprintf(stderr, "Invalid random seed. Using 2021 instead.\n");
        inputseed = SEED;
    } else {
        inputseed = strtoul(charinputseed, NULL, 10);
    }
    srandom(inputseed); //sets the seed

    //Game begins
    for (int playernumber = 0; playernumber < inputplayers;
         playernumber++) //each player takes their turn
    {
        fprintf(stdout, "%s rolls the pig...", names[playernumber]);
        while (playerturnover == 0) //players turn continues until they roll "side"
        {
            randomnumbergenerated = random() % 7; //generates number between 1-6
            fprintf(stdout, " pig lands %s", rollednames[randomnumbergenerated]);
            playerpoints[playernumber] += pointsearned[randomnumbergenerated];
            if (playerpoints[playernumber] >= 100) //if a player has hit 100 points end the game
            {
                fprintf(stdout, "\n%s wins with %i points!\n", names[playernumber],
                    playerpoints[playernumber]);
                playerhaswon = 1;
                break;
            }
            if (strcmp(rollednames[randomnumbergenerated], "on side")
                == 0) //end players turn if pig landed on its side
            {
                fprintf(stdout, "\n");
                playerturnover = 1;
                break;
            }
        }
        playerturnover = 0;
        if (playerhaswon
            == 1) //have to place a break here in order to ensure the game does not continue on by moving to the next player
        {
            break;
        } else if (playernumber + 1 == inputplayers) {
            playernumber = -1;
        }
    }
    return 0;
}

//Checks to see if the input string contains a letter, returns true(1) or false(0)
int ContainsALetter(char *CharArray) {
    for (unsigned long long i = 0; i < strlen(CharArray) - 1;
         i++) //Using strlen-1 to eliminate the \n that appears in the chararray
    {
        if (isdigit(CharArray[i]) == 0) {
            return 1; //found a character in the string
        }
    }
    return 0; //did not find a character in the string
}
