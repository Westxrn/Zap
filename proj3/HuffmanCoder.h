/**
 * File: HuffmanCoder.h
 * Description: Defines the HuffmanCoder class for encoding and decoding text 
 * files using Huffman coding. This class is responsible for building Huffman 
 * trees, generating codes for each character, and handling the serialization 
 * and deserialization of Huffman trees.
 * Author: Weston Starbird
 * Date: 2024-04-03
 */

#ifndef HUFFMANCODER_H
#define HUFFMANCODER_H

#include <string>
#include <unordered_map>
#include "HuffmanTreeNode.h"

class HuffmanCoder {
public:
    void encoder(const std::string& input_file, const std::string& output_file);
    void decoder(const std::string& input_file, const std::string& output_file);

private:
    std::unordered_map<char, int> countCharFrequencies(
        const std::string& input_file);
    
    HuffmanTreeNode* buildHuffmanTree(const std::unordered_map<char,
        int>& char_frequencies);
    
    void generateCharCodes(
        const HuffmanTreeNode* root, std::unordered_map<char, 
        std::string>& char_codes, std::string code = "");
    
    std::string encodeText(const std::string& input_text, 
        const std::unordered_map<char, std::string>& char_codes);

    std::string serializeHuffmanTree(const HuffmanTreeNode* root);

    HuffmanTreeNode* deserializeHuffmanTree(const std::string& serialized_tree);

    HuffmanTreeNode* deserializeHuffmanTreeHelper(
        const std::string &serialized_tree, int &index);

    std::string decodeText(const std::string &encoded_text, 
        const HuffmanTreeNode *root);

    std::string readFileContents(const std::string &input_file);

    void deleteHuffmanTree(HuffmanTreeNode *root);

};

#endif