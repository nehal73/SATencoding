# SATencoding
SAT encodings for various problems
To compile use g++ encoding_name.cpp -o encoding_name
To use ./encoding_name width output_cnf < input file

The input has to be dimacs format graphs starting with vertex number 1.

We have the following encodings:
1. sptw2sat: partition based special treewidth encoding
2. spel2sat: ordering based special treewidth encoding
3. path2sat: parition based pathwidth encoding
4. pathset2sat: ordering based (vertex separation number based) pathwidth encoding
