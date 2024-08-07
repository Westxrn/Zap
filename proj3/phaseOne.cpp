#include "phaseOne.h"



// Assume HuffmanTreeNode and NodeComparator are included via the header.

void count_freqs(std::istream &text) {
    std::unordered_map<char, int> frequencies;
    char ch; 
    while (text >> std::noskipws >> ch) {  
        // read every char including whitespace
        frequencies[ch]++; 
        // increment the frequency 
        //corresponding to the current char ch on the unordered mapd
    }
    for (const auto &pair : frequencies) { 
        // increment through pairs of frequencies and print them
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
}

std::string serialize_tree(HuffmanTreeNode *root) {
    if (!root) return ""; // using node as bool, '!' implies root is null
    if (root->isLeaf()) { // if we're at a leaf, return L 
        return std::string("L") + root->get_val(); // + value of the root
    }
    return std::string("I") + serialize_tree(root->get_left()) 
    + serialize_tree(root->get_right());
} // if we aren't at a leaf, keep recursing until we are, 
//adding string I to final output

HuffmanTreeNode* deserialize_tree(const std::string &serial_tree, int &pos) {
    if (pos >= serial_tree.size()) return nullptr; // base case 
    
    char type = serial_tree[pos++];
    if (type == 'L') {
        char val = serial_tree[pos++];
        // Leaf node
        return new HuffmanTreeNode(val, 1); 
        // Frequency is not used during deserialization
    } else {
        // Internal node
        HuffmanTreeNode *left = deserialize_tree(serial_tree, pos);
        HuffmanTreeNode *right = deserialize_tree(serial_tree, pos);
        return new HuffmanTreeNode('\0', 1, left, right); // do root last? 
    }
}

// wrapper function to handle deserialization from start
HuffmanTreeNode *deserialize_tree(const std::string &serial_tree) {
    int pos = 0;
    return deserialize_tree(serial_tree, pos);
}
