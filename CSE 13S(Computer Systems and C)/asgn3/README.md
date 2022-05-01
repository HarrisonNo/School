asgn3: Sorting: Putting your affairs in order

Usage: Allows the user to compare the efficiency of several sorting algorithms on different sized arrays.

How to: Build the program with clang -Wall -Wextra -Werror -Wpedantic -o sorting sorting.c
Run the compiled program inputting any combination of inputs of: -a -e -i -s -q -r -n -p -h :to view the results
of comparing the efficiency of Quick Sort, Insertion Sort, Heap Sort, and Shell Sort.

Known bugs: Shell sort correctly sorts the given array, but since Long included YIELD in his python code, my C translation evidently
did not match exactly with his. Resulting in my program having differing moves and compares than his provided binary. Again I iterate however,
that my shell sort does still correctly sort the given array and instead of trying to play "pin the tail on the donkey" trying to guess exactly how
Long implemented YIELD in his program, I will simply leave my working Shell Sort as is.