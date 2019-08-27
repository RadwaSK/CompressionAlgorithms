#ifndef _LZ77_H
#define _LZ77_H

#include<bitset>
#include<vector>
using namespace std;

// Struct to hold the triplet for each entry
struct Triplet {
	// Starting position of the longest match in the previous string
	// i.e. number of symbols to look back.
	unsigned int j;

	// Length of the longest match i.e. number of symbols to copy
	// starting at the match position.
	unsigned int k; //I want its max

	// Uncompressed code of the symbol following the match. The number
	// of bits here will depend on the size of the alphabet in use.
	unsigned char c;
};

class LZ77 {
	//Any private members are here

public:
	// Constructor and destructor
	LZ77();
	~LZ77();

	// A function to encode the input message using LZ77.
	// 
	// Inputs:
	// - the input message
	// - S the size of the match buffer i.e. the maximum distance to look
	//   backward for a match
	// - T the size of the lookahead buffer   
	// - A the size of the alphabet
	//
	// The encoded message contains the triplets output from the input msg.
	//
	// The function should return the number of bits in the encoded 
	// message.
	int encode(vector<unsigned char>& msg, int S, int T, int A, vector<Triplet>* encoded_msg);

	// A function to decode a message encoded using LZ77.
	void decode(const vector<Triplet>& encoded_msg, vector<unsigned char>* decoded_msg);
	
	void decorrelate(vector<unsigned char>& msg, vector<unsigned char>& difference_msg) const;
};

#endif