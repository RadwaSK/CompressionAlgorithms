#ifndef _HUFFMAN_H
#define _HUFFMAN_H

#include <vector>
#include <string>
using namespace std;

// Define a symbol struct here to hold the symbol (grayscale value)
// and its probability/frequency
struct Symbol {
	unsigned char val = NULL;
	double prob;
};

// Define the tree node struct here ...
struct Node {
	Symbol symbol;
	Node * left = nullptr;
	Node * right = nullptr;
};

class Huffman {
private:
	Node* tree;

	void printHelper(Node * subTreePtr, string str) const;
	Node * getCodeWord(Node * subTreePtr, unsigned char c, string & code) const;
	void destructTree(Node * subTreePtr);
public:
	// Constructor and destructor
	Huffman();
	~Huffman();

	//I would have stored the symbol array by then,
	//so I wouldn't need to resend the message vector,
	//I would compute the entropy using the vector I have in the instance
	double compute_entropy(const vector<unsigned char>& msg, vector<Symbol>* prob) const;

	// A function to build the Huffman tree given the computed
	// symbol probabilities.
	void build_tree(vector<Symbol>& prob);

	// A function to print the code table computed from the build_tree above.
	void print_code_table() const;

	// A function to encode the input message using the computed tree 
	// and store the output in the output array, where each entry
	// corresponds to the codeword for an input symbol in the message.
	//
	// The function should return the number of bits in the encoded 
	// message.
	int encode(vector<unsigned char>& msg, vector<unsigned int>* encoded_msg) const;

	void decorrelate(vector<unsigned char>& msg, vector<unsigned char>& encoded_msg) const;
};

#endif