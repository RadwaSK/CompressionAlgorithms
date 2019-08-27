#include "huffman.h"
#include <map>
#include <set>
#include <cmath>
#include <algorithm>
#include <queue>
#include <iostream>

//---------------ADDITIONAL FUNCTIONS---------------//

bool operator>(const Node& lhs, const Node& rhs)
{
	return lhs.symbol.prob > rhs.symbol.prob;
}

unsigned int convertStringToInt(string s) {
	int code = 0; bool first = true;
	int size = s.size();
	for (int i = 0; i < size; i++) {
		char c = s[i];
		if (first) {
			if (c == '1')
				code = 1;
			first = false;
			continue;
		}
		code = code << 1;
		if (c == '1')
			code += 1;
	}
	return code;
}

bool isLeaf(Node * n) {
	return !n->left && !n->right;
}

//---------------CONSTRUCTOR & DESTRUCTOR---------------//

Huffman::Huffman() {
	tree = nullptr;
}

Huffman::~Huffman() {
	destructTree(tree);
}

//---------------PRIVATE FUNCTIONS---------------//

void Huffman::printHelper(Node * subTreePtr, string str) const {
	if (subTreePtr) {
		Symbol s = subTreePtr->symbol;
		if (isLeaf(subTreePtr)) {
			cout << (int)s.val << ": " << str << endl;
		}
		string temp = str;
		temp += "0";
		printHelper(subTreePtr->left, temp);
		temp = str;
		temp += "1";
		printHelper(subTreePtr->right, temp);
	}
}
//
//Node * Huffman::getCodeWord(Node * subTreePtr, unsigned char c, int & code, bool & first, int & leftShifts) const {
//	//if nullptr
//	if (!subTreePtr)
//		return subTreePtr;
//	
//	if (subTreePtr->symbol.val && subTreePtr->symbol.val == c) {
//		return subTreePtr;
//	}
//
//	Node * left = getCodeWord(subTreePtr->left, c, code, first, leftShifts);
//	if (left) {
//		if (first)
//			first = false;
//		leftShifts += 1;
//		return left;
//	}
//	else
//		leftShifts = 1;
//
//	Node * right = getCodeWord(subTreePtr->right, c, code, first, leftShifts);
//	if (right) {
//		if (!first)
//			code << leftShifts;
//		else
//			first = false;
//		code += 1;
//		return right;
//	}
//	else
//		leftShifts = 0;
//
//	return nullptr;
//}

Node * Huffman::getCodeWord(Node * subTreePtr, unsigned char c, string & code) const {
	//if nullptr
	if (!subTreePtr)
		return subTreePtr;

	if (subTreePtr->symbol.val && subTreePtr->symbol.val == c) {
		return subTreePtr;
	}

	code += "0";
	Node * left = getCodeWord(subTreePtr->left, c, code);
	if (left)
		return left;
	else
		code.erase(code.size() - 1, 1);

	code += '1';
	Node * right = getCodeWord(subTreePtr->right, c, code);
	if (right)
		return right;
	else
		code.erase(code.size() - 1, 1);

	return nullptr;
}

void Huffman::destructTree(Node * subTreePtr) {
	if (subTreePtr) {
		destructTree(subTreePtr->left);
		destructTree(subTreePtr->right);

		delete subTreePtr;
		subTreePtr = nullptr;
	}
}

//---------------PUBLIC FUNCTIONS---------------//

double Huffman::compute_entropy(const vector<unsigned char>& msg, vector<Symbol>* prob) const {
	map<unsigned char, float> repeatedNums;
	set<unsigned char> uniqueSet;
	double entropy = 0;
	int sz = msg.size();

	for (int i = 0; i < sz; i++) {
		repeatedNums[msg[i]]++;
		uniqueSet.insert(msg[i]);
	}
	//now I have set of all unique values in the image
	//and a map, each key with its number of repeatitions
	set<unsigned char>::iterator itSet = uniqueSet.begin();
	for (; itSet != uniqueSet.end(); itSet++) {
		double probability = (repeatedNums[*itSet] * 1.0) / sz;
		entropy -= probability * log2(probability);
		Symbol s;
		s.val = *itSet;
		s.prob = probability;
		prob->push_back(s);
	}
	return entropy;
}

void Huffman::build_tree(vector<Symbol>& prob) {
	//sort probabilities in asc order
	vector<Symbol>::iterator it = prob.begin();
	priority_queue<Node, vector<Node>, greater<Node>> q;

	//fill the queue with the symbols
	for (; it != prob.end(); it++) {
		Node fakeNode;
		fakeNode.symbol = *it;
		q.push(fakeNode);
	}

	Node *l = nullptr, *r = nullptr, *nPtr = nullptr;
	while (q.size() > 1) {
		Node n = q.top();

		//if the symbol taken from queue is a merged one
		if (!n.symbol.val) {
			l = new Node;
			l->symbol.prob = n.symbol.prob;
			l->symbol.val = NULL;
			l->left = n.left;
			l->right = n.right;
		}
		else {
			l = new Node;
			l->symbol = n.symbol;
		}
		q.pop();

		n = q.top();

		//if the symbol taken from queue is a merged one
		if (!n.symbol.val) {
			r = new Node;
			r->symbol.prob = n.symbol.prob;
			r->symbol.val = NULL;
			r->left = n.left;
			r->right = n.right;
		}
		else {
			r = new Node;
			r->symbol = n.symbol;
		}
		q.pop();

		nPtr = new Node;
		nPtr->symbol.val = NULL;
		nPtr->symbol.prob = l->symbol.prob + r->symbol.prob;
		nPtr->left = l; nPtr->right = r;
		q.push(*nPtr);
	}
	tree = nPtr;
}

void Huffman::print_code_table() const {
	printHelper(tree, "");
}

int Huffman::encode(vector<unsigned char>& msg, vector<unsigned int>* encoded_msg) const {
	int bitsNum = 0;
	vector<unsigned char>::iterator it = msg.begin();
	for (; it != msg.end(); it++) {
		string code = "";
		getCodeWord(tree, *it, code);
		bitsNum += code.size();
		encoded_msg->push_back(convertStringToInt(code));
	}
	return bitsNum;
}

void Huffman::decorrelate(vector<unsigned char>& msg, vector<unsigned char>& difference_msg) const {
	vector<unsigned char>::iterator it = msg.begin();
	bool first = true;
	for (int i = 0; it != msg.end(); it++, i++) {
		if (first) {
			first = false;
			difference_msg.push_back(*it);
		}
		else {
			difference_msg.push_back((int)*it - (int)msg[i - 1]);
		}
	}
}