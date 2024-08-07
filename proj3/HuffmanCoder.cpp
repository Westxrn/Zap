/**
 * File: HuffmanCoder.cpp
 * Description: Implements Huffman coding algorithms for text compression and 
 * decompression, utilizing Huffman trees for efficient character encoding.
 * Author: Weston Starbird
 * Date: 2024-04-03
 */

#include "HuffmanCoder.h"
#include "BinaryIO.h"
#include <fstream>
#include <queue>
#include <stdexcept>
#include <sstream>
#include <iostream> 

/**
 * name:       encoder
 * purpose:    Encodes the content of an input file into Huffman encoded format 
 * and writes 
 *             the serialized Huffman tree and encoded content to an output 
 * file. Reports 
 *             success or states if the input file is empty.
 * arguments:  input_file - a string representing the path to the input file 
 * to be encoded.
 *             output_file - a string representing the path to the output file 
 * where the 
 *             serialized tree and encoded content will be written.
 * returns:    void
 * effects:    Writes to an output file and prints to stdout. The input file is 
 * read, and 
 *             its content is encoded. The output file will contain the 
 * serialized Huffman 
 *             tree followed by the encoded content. Prints a message to 
 * stdout on success 
 *             or if the input file is empty.
 */
void HuffmanCoder::encoder(const std::string& input_file, 
                            const std::string& output_file) {
        // Read input file contents
        std::string input_text = readFileContents(input_file);
        // Check if the input file is empty
        if (input_text.empty()) {
            std::cout << input_file << " is empty and cannot be compressed." 
                                                                << std::endl;
            return;
        }
        // Count character frequencies
        std::unordered_map<char, int> char_frequencies = 
                                countCharFrequencies(input_file);
        // Build Huffman tree
        HuffmanTreeNode* root = buildHuffmanTree(char_frequencies);
        // Generate character codes
        std::unordered_map<char, std::string> char_codes;
        generateCharCodes(root, char_codes);
        // Encode text
        std::string encoded_text = encodeText(input_text, char_codes);
        // Serialize Huffman tree
        std::string serialized_tree = serializeHuffmanTree(root);
        // Write to file
        BinaryIO binary_io;
        binary_io.writeFile(output_file, serialized_tree, encoded_text);
        // Clean up
        deleteHuffmanTree(root);
        std::cout << "Success! Encoded given text using " 
                                                    << encoded_text.size() 
                                                    << " bits." << std::endl;
}

/**
 * name:       decoder
 * purpose:    Decodes Huffman encoded content from an input file using the 
 * serialized 
 *             Huffman tree provided in the same file, writing the decoded 
 * text to an 
 *             output file.
 * arguments:  input_file - a string representing the path to the input file 
 * containing 
 *             the serialized Huffman tree and encoded content.
 *             output_file - a string representing the path to the output 
 * file where 
 *             decoded text is to be written.
 * returns:    void
 * effects:    Reads encoded data and a serialized tree from an input file, 
 * decodes the 
 *             data, and writes the decoded text to an output file. 
 * Modifies the output 
 *             file and reads from the input file.
 */
void HuffmanCoder::decoder(const std::string& input_file, 
                            const std::string& output_file) {
    BinaryIO binary_io;
    std::pair<std::string, std::string> file_data = 
                                        binary_io.readFile(input_file);
    std::string serialized_tree = file_data.first;
    std::string encoded_text = file_data.second;
    HuffmanTreeNode* root = deserializeHuffmanTree(serialized_tree);
    std::string decoded_text;
    if (root->isLeaf()) {
        bool allZeros = true;
        // instead of breaking, use allZeros flag to exit the loop early
        int length = encoded_text.length();
        for (int i = 0; i < length and allZeros; i++) {
            if (encoded_text[i] != '0') {
                allZeros = false;
            }
        }
        if (allZeros) {
            decoded_text = std::string(encoded_text.size(), root->get_val());
        } else {
            decoded_text = decodeText(encoded_text, root);
        }
    } else {
        decoded_text = decodeText(encoded_text, root);
    }
    std::ofstream output_file_stream(output_file); // write to file
    output_file_stream << decoded_text;
    output_file_stream.close();
    deleteHuffmanTree(root); // clean up
}

/**
 * name:       countCharFrequencies
 * purpose:    Counts the frequency of each character in the input file 
 * and returns a map 
 *             of characters to their frequencies.
 * arguments:  input_file - a string representing the path to the input 
 * file whose contents 
 *             are to be analyzed for character frequencies.
 * returns:    A std::unordered_map<char, int> mapping each character to 
 * its frequency in 
 *             the input file.
 * effects:    Reads from the input file. Throws a runtime_error if the 
 * file cannot be 
 *             opened.
 */
std::unordered_map<char, int> 
HuffmanCoder::countCharFrequencies(const std::string& input_file) {
    // create map to hold frequencys, key = chars, values = ints
    std::unordered_map<char, int> char_frequencies;
    std::ifstream file(input_file);
    if (not file) {
        throw std::runtime_error("Unable to open file " + input_file);
    } // throw error if can't open file

    char c;
    while (file.get(c)) {
        char_frequencies[c]++;
    } // code to count frequencies in frequency map
    file.close();
    return char_frequencies; // return the filled in map
}

/**
 * name:       buildHuffmanTree
 * purpose:    Builds a Huffman tree based on the frequencies of 
 * characters and returns 
 *             the root of the tree.
 * arguments:  char_frequencies - a map of characters to their frequencies.
 * returns:    A pointer to the root of the constructed Huffman tree.
 * effects:    Constructs a Huffman tree in dynamic memory. The caller 
 * is responsible 
 *             for deallocating the tree.
 */
HuffmanTreeNode* HuffmanCoder::buildHuffmanTree(const std::unordered_map<char, 
                                                    int>& char_frequencies) {
    // create a priority queue of HuffmanTreeNodes
    std::priority_queue<HuffmanTreeNode*, std::vector<HuffmanTreeNode*>, 
                                                        NodeComparator> pq;

    // add leaf nodes to the priority queue
    for (const auto& pair : char_frequencies) {
        pq.push(new HuffmanTreeNode(pair.first, pair.second));
    }
    // build the Huffman tree
    while (pq.size() > 1) {
        HuffmanTreeNode* left = pq.top();
        pq.pop();
        HuffmanTreeNode* right = pq.top();
        pq.pop();
        int combined_freq = left->get_freq() + right->get_freq();
        HuffmanTreeNode* parent = new HuffmanTreeNode('\0', combined_freq, 
                                                                left, right);
        pq.push(parent);
    }
    // return the root of the Huffman tree
    return pq.top();
}

/**
 * name:       generateCharCodes
 * purpose:    Generates Huffman codes for characters based on the 
 * provided Huffman tree 
 *             and stores them in a map.
 * arguments:  root - a pointer to the root of the Huffman tree.
 *             char_codes - a reference to a map where the generated 
 * Huffman codes will 
 *             be stored, mapping characters to their Huffman codes.
 *             code - a string representing the current Huffman code 
 * in the recursive 
 *             traversal (initially empty).
 * returns:    void
 * effects:    Modifies the char_codes map to include Huffman codes for 
 * characters.
 */
void HuffmanCoder::generateCharCodes(const HuffmanTreeNode* root, 
        std::unordered_map<char, std::string>& char_codes, std::string code) {
    if (not root) {
        return;
    }
    if (root->isLeaf()) {
        char_codes[root->get_val()] = code;
        return;
    } 
    // left = 0, right = 1 - need to reflect this in recursive calls
    generateCharCodes(root->get_left(), char_codes, code + "0");
    generateCharCodes(root->get_right(), char_codes, code + "1");
}

/**
 * name:       encodeText
 * purpose:    Encodes a given text using provided Huffman codes and 
 * returns the encoded 
 *             string.
 * arguments:  input_text - the text to be encoded.
 *             char_codes - a map of characters to their Huffman codes.
 * returns:    A string representing the encoded text.
 * effects:    None.
 */
std::string HuffmanCoder::encodeText(const std::string& input_text, 
                    const std::unordered_map<char, std::string>& char_codes) {
    std::string encoded_text;
    // Check if input text contains only a single unique character
    if (char_codes.size() == 1) {
        // Assign character code "0" to the unique character
        encoded_text = std::string(input_text.size(), '0');
    } else {
        // encode text using the character codes
        for (char c : input_text) { // iterate through input_text
            encoded_text += char_codes.at(c);
        } // add from map of char_codes to encoded_text using key char c
    }
    return encoded_text;
}

/**
 * name:       serializeHuffmanTree
 * purpose:    Serializes a Huffman tree into a string format for storage 
 * or transmission.
 * arguments:  root - a pointer to the root of the Huffman tree to be 
 * serialized.
 * returns:    A string representing the serialized Huffman tree.
 * effects:    None.
 */
std::string HuffmanCoder::serializeHuffmanTree(const HuffmanTreeNode* root) {
    if (not root) { // if tree is empty
        return "";
    }
    if (root->isLeaf()) {
        return "L" + std::string(1, root->get_val());
    } // "L" to denote leaf
    return "I" + serializeHuffmanTree(root->get_left()) 
                + serializeHuffmanTree(root->get_right());
}


/**
 * name:       deserializeHuffmanTree
 * purpose:    Constructs a Huffman tree from its serialized string 
 * representation.
 * arguments:  serialized_tree - a string representing the serialized 
 * Huffman tree.
 * returns:    A pointer to the root of the deserialized Huffman tree.
 * effects:    Allocates memory for a new Huffman tree. The caller is 
 * responsible for 
 *             deallocating the tree.
 */
HuffmanTreeNode* HuffmanCoder::deserializeHuffmanTree(
                                const std::string& serialized_tree) {
    if (serialized_tree.empty()) {
        return nullptr;
    }
    int index = 0;
    // call recursive helper
    return deserializeHuffmanTreeHelper(serialized_tree, index);
}

/**
 * name:       deserializeHuffmanTreeHelper
 * purpose:    Recursively rebuilds a Huffman tree from its serialized 
 * representation.
 * arguments:  serialized_tree - the serialized tree as a string.
 *             index - reference to the current position within the 
 * serialized string.
 * returns:    A pointer to a HuffmanTreeNode, either a newly constructed 
 * node or nullptr 
 *             if the end of the string is reached or an invalid structure is 
 * detected.
 * effects:    Allocates memory for HuffmanTreeNode(s) as it reconstructs the 
 * tree from 
 *             the serialized data. Adjusts the index to track progress through 
 * the string. 
 *             Caller is responsible for freeing the allocated memory.
 */
HuffmanTreeNode* HuffmanCoder::deserializeHuffmanTreeHelper(
                        const std::string& serialized_tree, int& index) {
    int size = serialized_tree.size();
    if (index >= size) {
        return nullptr;
    } // base case: index > size of tree meaning index is out of range
    char type = serialized_tree[index++];

    if (type == 'L') { // if at leaf, val = char at index+1
        char val = serialized_tree[index++];
        return new HuffmanTreeNode(val, 0); // return a new node with new char
    } else { // use recursive preorder to deserialize
        HuffmanTreeNode* left = deserializeHuffmanTreeHelper(serialized_tree, 
                                                                        index);
        HuffmanTreeNode* right = deserializeHuffmanTreeHelper(serialized_tree, 
                                                                        index);
        return new HuffmanTreeNode('\0', 0, left, right);
    }
}

/**
 * name:       decodeText
 * purpose:    Decodes a string of encoded text using a Huffman tree and 
 * returns the 
 *             original text.
 * arguments:  encoded_text - the encoded text to be decoded.
 *             root - a pointer to the root of the Huffman tree used for 
 * decoding.
 * returns:    A string representing the decoded original text.
 * effects:    Throws a runtime_error if the encoding does not match the 
 * Huffman tree.
 */
std::string HuffmanCoder::decodeText(const std::string& encoded_text, 
                                                const HuffmanTreeNode* root) {
    if (not root) throw std::runtime_error("Huffman tree is empty.");
    std::string decoded_text;
    const HuffmanTreeNode* curr = root;
    for (char bit : encoded_text) { // iterate through encoded text
        if (bit == '0') { // 0 = left, so get left here
            curr = curr->get_left();
        } else if (bit == '1') { // 1 = right, so get right here
            curr = curr->get_right();
        }
        if (curr == nullptr) {
            throw std::runtime_error("Encoding did not match Huffman tree.");
        } // throw but don't catch like demo
        if (curr->isLeaf()) { // if we're at a leaf, leaf value should be added
            decoded_text += curr->get_val();
            curr = root; // Reset to root for next character
        }
    }
    if (curr != root) {
        throw std::runtime_error("Encoding did not match Huffman tree.");
    } // throw but don't catch error 
    return decoded_text;
}

/**
 * name:       readFileContents
 * purpose:    Reads the entire content of a file into a string and returns it.
 * arguments:  input_file - a string representing the path to the file to be 
 * read.
 * returns:    A string containing the content of the file.
 * effects:    Reads from the input file. Throws a runtime_error if the 
 * file cannot be 
 *             opened.
 */
std::string HuffmanCoder::readFileContents(const std::string& input_file) {
    // Open the file in binary mode. This ensures that the file is read
    // byte-for-byte
    std::ifstream file(input_file, std::ios::binary);

    if (not file) {
        throw std::runtime_error("Unable to open file " + input_file);
    }

    // Create an output string stream. This stream will hold the 
    //contents of the file
    // in memory, allowing us to convert it to a string later.
    std::ostringstream buffer;
    // Use the stream buffer's member function .rdbuf() to obtain a pointer to 
    // file's buffer. Then, use the << operator to read the entire buffer into
    // the ostringstream buffer. This efficiently transfers the file's content
    // into the string stream.
    buffer << file.rdbuf(); 
    file.close();

    return buffer.str();
}

/**
 * name:       deleteHuffmanTree
 * purpose:    Recursively deletes a Huffman tree to free allocated memory.
 * arguments:  root - a pointer to the root of the Huffman tree to be deleted.
 * returns:    void
 * effects:    Recursively frees the memory allocated for the Huffman tree.
 */
void HuffmanCoder::deleteHuffmanTree(HuffmanTreeNode* root) {
    if (not root) { // if current node = nullptr
        return; 
    }
    // recursively delete the tree w/ preorder traversal
    deleteHuffmanTree(root->get_left());
    deleteHuffmanTree(root->get_right());
    delete root;
}
