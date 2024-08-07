/**
 * File: UnitTests.cpp
 * Description: Contains unit tests for verifying the functionality of the 
 * HuffmanCoder class methods, including tree construction, text encoding and 
 * decoding, and tree serialization/deserialization. Utilizes the ZapUtil 
 * library for tree comparison.
 * Author: Weston Starbird
 * Date: 2024-04-03
 */

#include <cassert> 
#include <sstream> 
#include <string> 
#include "phaseOne.h"
#include "HuffmanCoder.h"
#include "HuffmanTreeNode.h"
#include "ZapUtil.h"

// unit_tests for phaseOne --> translate to phaseTwo(same methods)

// test_count_freqs(): Checks accurate serialization of a simple Huffman tree.
void test_count_freqs() {
    std::stringstream input("aabbccc");
    std::stringstream output;
    
    // Save the old buffer
    std::streambuf *oldCoutStreamBuf = std::cout.rdbuf();
    
    // Redirect cout to our stringstream buffer or any other stream
    std::cout.rdbuf(output.rdbuf());

    // Call the function
    count_freqs(input);

    // Restore the original buffer before checking assertions to see 
    // standard cout output if needed
    std::cout.rdbuf(oldCoutStreamBuf);

    // Construct the expected string
    // Note: Order of output might vary due to the unordered nature of 
    // unordered_map
    std::string expected[] = {"a: 2\n", "b: 2\n", "c: 3\n"};
    std::string actual = output.str();

    // Validate
    for (const auto& exp : expected) {
        assert(actual.find(exp) != std::string::npos);
    }
}

// Test for serialize_tree function
void test_serialize_tree() {
    // Constructing a simple tree:
    //        (*)
    //       /   \
    //     (a)   (b)
    HuffmanTreeNode *left = new HuffmanTreeNode('a', 1);
    HuffmanTreeNode *right = new HuffmanTreeNode('b', 1);
    HuffmanTreeNode *root = new HuffmanTreeNode('\0', 1, left, right);

    std::string expected = "ILaLb";
    std::string result = serialize_tree(root);

    delete left;  // Clean up
    delete right;
    delete root;

    assert(result == expected);
}

// test for deserialize_tree function
void test_deserialize_tree() {
    std::string serial_tree = "ILaLb";
    HuffmanTreeNode *root = deserialize_tree(serial_tree);

    assert(root != nullptr);
    assert(root->get_left() != nullptr and root->get_right() != nullptr);
    assert(root->get_left()->get_val() == 'a');
    assert(root->get_right()->get_val() == 'b');

    // Clean up
    delete root; // Implement a proper recursive delete in HuffmanTreeNode 
    // to handle this
}

// phase2 tests below --> tests only private methods
// had to temporarily make each method public for testing
// deleteTree method / destructor tested via valgrind


// testEncodeDecodeText(): Tests encoding/decoding process for accuracy.
void testEncodeDecodeText() {
    HuffmanCoder hc;
    HuffmanTreeNode* root = makeFigure1Tree('\0');

    std::unordered_map<char, std::string> charCodes;
    hc.generateCharCodes(root, charCodes, "");
    std::string originalText = "aabbcc";
    std::string encodedText = hc.encodeText(originalText, charCodes);
    std::string decodedText = hc.decodeText(encodedText, root);

    assert(originalText == decodedText);
}

// testBuildHuffmanTree(): Validates Huffman tree construction from frequencies
void testBuildHuffmanTree() {
    std::unordered_map<char, int> frequencies = {{'a', 3}, {'b', 2}, {'c', 1}};
    HuffmanCoder hc;
    HuffmanTreeNode* root = hc.buildHuffmanTree(frequencies);

    // Check the structure manually or use treeEquals from ZapUtil if 
    // applicable
    assert(root->get_freq() == 6); // Total frequency
    assert(root->get_left() != nullptr);
    assert(root->get_right() != nullptr);

    // Assuming treeEquals is defined and works correctly
    HuffmanTreeNode* expectedRoot = makeFigure1Tree('\0'); // Adjust 
    // based on actual expected structure
    assert(treeEquals(root, expectedRoot, true, false));
}

// testEncodeDecodeTextComprehensive(): Comprehensive test on encoding/decoding
void testEncodeDecodeTextComprehensive() {
    HuffmanCoder hc;
    HuffmanTreeNode* root = makeFigure1Tree('\0');

    // Generate Huffman codes based on the tree structure
    std::unordered_map<char, std::string> charCodes;
    hc.generateCharCodes(root, charCodes, "");

    // Test various strings
    std::vector<std::string> testStrings = {"a", "b", "c", "d", "e", "f", 
                                                    "abcdef", "fabecd", ""};
    for (const auto& originalText : testStrings) {
        std::string encodedText = hc.encodeText(originalText, charCodes);
        std::string decodedText = hc.decodeText(encodedText, root);

        assert(originalText == decodedText);
    }
}

// testSerializeDeserializeTree(): Confirms serialization/deserialization 
// accuracy.
void testSerializeDeserializeTree() {
    HuffmanTreeNode* originalTree = makeFigure1Tree('\0');
    HuffmanCoder hc;

    // Serialize then deserialize the tree
    std::string serializedTree = hc.serializeHuffmanTree(originalTree);
    HuffmanTreeNode* deserializedTree = hc.deserializeHuffmanTree(
                                                        serializedTree);

    // Use treeEquals to assert tree structure equality
    assert(treeEquals(originalTree, deserializedTree, true, false));
}

