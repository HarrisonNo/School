asgn7: The Great Firewall of Santa Cruz: Bloom Filters, Linked Lists, Binary Trees and Hash Tables

Usage: Allows the user detect any forbidden words entered into stdin.

How to: Build the program with make.
Run ./banhammer, enter any user input you want to check into stdin. Once finished entering user input, enter control+d to finish.
Optionally run with -s to see statistics.
Optionally run with -t X to manually specify the size for the hashtable.
Optionally run with -f X to manually specify the size for the bloom filter.
Needs "badspeak.txt" and "newspeak.txt" to be present for the program to scan which words are banned.

Known bugs: None known.