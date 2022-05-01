asgn4: The Perambulations of Denver Long

Usage: Allows the user to find the shortest possible Hamiltonian path starting from a starting location.

How to: Build the program with make
Run the compiled program inputting any combination of inputs of: -h -v -u -i: ExampleInputUserFile.txt -o: ExampleOutputUserFile.txt
to be able to supply the program with a graph and be supplied the desired shortest path in the output file.

Known bugs: A complete lack of DFS. At one point I did in fact have a DFS but I could not get it to loop back to the starting location properly
without resulting in some sort of segfault. Thus I scrapped it in order to prevent valgrind errors from appearing as when it crashed it would(obviously)
not free the required variables.