asgn1: Pass the Pigs

Usage: Allows the user to play a game of Pass the Pigs with 2-10 players.

How to: Build the program with clang -Wall -Wextra -Werror -Wpedantic -o pig pig.c
Run the compiled program.
Then input a number between 2-10 for the number of players and enter any number for the seed.

Known bugs: If the user inputs non-number characters for the number of players input, then it will default to 2 as expected but it will also then ask the user for the seed,
which is contrary to what the binary does. Where if the player enters a non-number character for players it will default to using standard values for both number of players and the seed. So IMO my version is an improvement but I don't know whether Long expects us to have the exact same flaws as his program so...