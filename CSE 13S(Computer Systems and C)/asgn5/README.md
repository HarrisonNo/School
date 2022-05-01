asgn5: Huffman Coding

Usage: Allows the user to encode an input file via Huffman encoding, resulting in a compressed file that takes less space than the original.
Also allows the user to decode an already Huffman encoded input file and output a copy of the original unencoded file.

How to: Build the program with make
Run either ./encode or ./decode inputting any combination of inputs of: -h -v -i: ExampleInputUserFile.txt -o: ExampleOutputUserFile.txt
to be able to supply the program with an input file to either encode or decode and an output file to print the requested encoded/decoded version.
-v results in the program printing several statistics regarding the encoding/decoding process.

Known bugs: Memory errors for decode.c. Encode does function albeit not printing the desired result.