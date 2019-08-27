#include "huffman.h"
#include <iostream>
#include <string>

enum funcEnum {
	entropy,
	tree,
	encode
};

void printEncodedMsg(vector<unsigned int> &msg) {
	cout << "Message after encoding is:" << endl;
	vector<unsigned int>::iterator it = msg.begin();
	for (; it != msg.end(); it++)
		cout << *it << " ";
	cout << endl;
}

int main(int argc, char* argv[]) {
	string functionCalled = argv[1];
	
	funcEnum function;
	if (functionCalled == "-entropy")
		function = entropy;
	else if (functionCalled == "-tree")
		function = tree;
	else if (functionCalled == "-encode")
		function = encode;


	bool correlated = false;
	if (argc > 2)
		correlated = true;
	
	string format;
	int xres, yres, maxIntensity;
	cin >> format;
	cin >> xres >> yres >> maxIntensity;

	vector<unsigned char> msg;
	int temp;
	int imageSize = xres * yres;
	for (int i = 0; i < imageSize; i++) {
		cin >> temp;
		msg.push_back(temp);
	}

	Huffman huffmanObj;
	vector<Symbol>* prob = new vector<Symbol>;
	vector<unsigned int> * encoded_msg = new vector<unsigned int>;
	int bitsNum = 0;

	switch (function) {
	case entropy:
		cout << "Entropy of pixels' values in input image = " << huffmanObj.compute_entropy(msg, prob) << endl;
		break;

	case tree:
		huffmanObj.compute_entropy(msg, prob);
		huffmanObj.build_tree(*prob);
		huffmanObj.print_code_table();
		break;

	case encode:
		if (correlated) {
			vector<unsigned char> difference_msg;
			huffmanObj.decorrelate(msg, difference_msg);
			huffmanObj.compute_entropy(difference_msg, prob);
			huffmanObj.build_tree(*prob);
			bitsNum = huffmanObj.encode(difference_msg, encoded_msg);
			cout << "Number of bytes in the input image is " << ceil(bitsNum / 8.0) << endl;
		}
		else {
			huffmanObj.compute_entropy(msg, prob);
			huffmanObj.build_tree(*prob);
			bitsNum = huffmanObj.encode(msg, encoded_msg);
			cout << "Number of bytes in the input image is " << ceil(bitsNum / 8.0) << endl;
		}
		printEncodedMsg(*encoded_msg);
		break;

	default:
		break;
	}

	delete prob, encoded_msg;
	prob = nullptr; encoded_msg = nullptr;
	
	return 0;
}