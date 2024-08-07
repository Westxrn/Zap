/**
 * File: main.cpp
 * Description: Main driver for the Huffman coding program. It processes 
 * command-line arguments to compress or decompress files using the 
 * HuffmanCoder 
 * class and reports the outcome of these operations.
 * Author: Weston Starbird
 * Date: 2024-04-03
 */

#include "HuffmanCoder.h"
#include <iostream>
#include <cstdlib>


/**
 * name:       main
 * purpose:    Serves as the entry point for the Huffman coding program, 
 *             handling command line arguments to either compress ("zap") or 
 *             decompress ("unzap") a given input file and write the result 
 *             to an output file.
 * arguments:  argc - the number of command line arguments passed to the 
 * program.
 *             argv - an array of pointers to the strings representing those 
 *             arguments.
 * returns:    0 on successful compression or decompression; 
 *             EXIT_FAILURE if there is an error in command line arguments or 
 *             during
 *             the operation.
 * effects:    Parses command line arguments to determine operation mode 
 *             (zap or unzap),
 *             input file name, and output file name. Depending on the mode, 
 *             it either 
 *             compresses or decompresses the content of the input file and 
 *             writes the 
 *             result to the output file. Prints to stderr and exits with 
 *             EXIT_FAILURE 
 *             if the command line arguments do not match expected format 
 *             or if an 
 *             unsupported mode is provided.
 */
int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: ./zap [zap | unzap] inputFile outputFile" 
                                                            << std::endl;
        return EXIT_FAILURE;
    }
    // read in first command line argument into string "mode"
    std::string mode(argv[1]);
    std::string input_file(argv[2]);
    std::string output_file(argv[3]);

    HuffmanCoder coder;
        // check what mode is, if command line format is wrong, print an error
        if (mode == "zap") {
            coder.encoder(input_file, output_file);
        } else if (mode == "unzap") {
            coder.decoder(input_file, output_file);
        } else {
            std::cerr << "Usage: ./zap [zap | unzap] inputFile outputFile" 
                                                                << std::endl;
            return EXIT_FAILURE;
        }

    return 0;
}