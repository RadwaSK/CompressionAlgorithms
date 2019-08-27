#include "arithmetic.h"
#include <set>
#include <cmath>

//---------------CONSTRUCTOR & DESTRUCTOR---------------//
Arithmetic::Arithmetic() {
	
}

Arithmetic::~Arithmetic() {

}

//---------------PRIVATE FUNCTIONS---------------//

void Arithmetic::calcProbabilities(const vector<unsigned char> & msg, vector<pair<unsigned char, double>> & probs)
{
	map<unsigned char, int> counterMap;
	int sz = msg.size();

	for (int i = 0; i < sz; i++)
		counterMap[msg[i]]++;

	int uniques = 0;
	for (auto e : counterMap) {
		uniques++;
		pair<unsigned char, double> p;
		p.first = e.first;
		double probability = (e.second * 1.0) / sz;
		p.second = probability;
		probs.push_back(p);
	}
}

unsigned char Arithmetic::findChar(double t)
{
	for (auto e : charLimits) {
		if (t >= e.second.first && t < e.second.second)
			return e.first;
	}
	return 0;
}

//---------------PUBLIC FUNCTIONS---------------//

void Arithmetic::setCharLimits(vector<pair<unsigned char, pair<double, double>>> &limits)
{
	vector<pair<unsigned char, pair<double, double>>>::iterator it = limits.begin();
	
	for (; it != limits.end(); it++) 
		charLimits[(*it).first] = (*it).second;
}

vector<pair<unsigned char, pair<double, double>>> Arithmetic::getCharLimits() const
{
	vector<pair<unsigned char, pair<double, double>>> limits;
	for (auto e : charLimits) {
		limits.push_back(e);
	}
	return limits;
}

void Arithmetic::setMsgSize(int s)
{
	msgSize = s;
}

double Arithmetic::binary_to_decimal(const bitset<32>& fraction)
{
	double decimal = 0;
	for (int i = 31; i >= 0; --i) {
		if (fraction[i] == 1)
			decimal += pow(2, -(32 - i));
	}
	return decimal;
}

bitset<32> Arithmetic::decimal_to_binary(double fraction)
{
	bitset<32> binary;
	binary.reset();
	for (int i = 31; fraction != 0.0 && i >= 0; --i) {
		fraction *= 2;
		if ((int)fraction == 1)
			binary[i] = 1;
		else
			binary[i] = 0;
		fraction -= (int)fraction;
	}
	return binary;
}

int Arithmetic::encode(const vector<unsigned char>& msg, int num_symbols_per_block, vector<bitset<32>>* encoded_msg)
{
	double lo, uo, l, u;
	msgSize = msg.size();
	int tagsSize = ceil((1.0 * msgSize) / num_symbols_per_block);
	double * tagsArr = new double[tagsSize];
	int m = num_symbols_per_block;

	vector<pair<unsigned char, double>> probs;
	calcProbabilities(msg, probs);
	int uniq = probs.size();

	//store L and U for each character
	unsigned char c;
	double prob, accumProb = 0;
	for (int i = 0; i < uniq; i++) {
		c = probs[i].first;
		prob = probs[i].second;
		charLimits[c].first = accumProb;
		charLimits[c].second = accumProb + prob;
		accumProb += prob;
	}
	//calculating tags
	for (int i = 0, indx = 0; i < msgSize;) {
		lo = l = charLimits[msg[i]].first; uo = u = charLimits[msg[i++]].second;
		for (int j = 0; j < m - 1 && i < msgSize; j++, i++) {
			l = lo + (uo - lo) * charLimits[msg[i]].first;
			u = lo + (uo - lo) * charLimits[msg[i]].second;
			lo = l; uo = u;
		}
		tagsArr[indx] = (u + l) / 2.0;
		//convert tag to binary, store it and its limits
		bitset<32> tagBinary = decimal_to_binary(tagsArr[indx++]);
		encoded_msg->push_back(tagBinary);
		tagsLimits[tagBinary.to_string()].first = l;
		tagsLimits[tagBinary.to_string()].second = u;
	}

	delete[] tagsArr;
	return encoded_msg->size() * 32;
}

void Arithmetic::decode(const vector<bitset<32>>& encoded_msg, int num_symbols_per_block, vector<unsigned char>* decoded_msg)
{
	int encodedSize = encoded_msg.size();
	unsigned char c;
	double lo, uo, l, u, t, Tag;
	int m = num_symbols_per_block;
	
	for (int i = 0; i < encodedSize; i++) {
		lo = l = 0; uo = u = 1;
		Tag = binary_to_decimal(encoded_msg[i]);
		for (int j = 0; j < m && (i*m + j) < msgSize; j++) {
			t = (Tag - l) / (u - l);
			c = findChar(t);
			decoded_msg->push_back(c);
			
			l = lo + (uo - lo) * charLimits[c].first;
			u = lo + (uo - lo) * charLimits[c].second;
			lo = l; uo = u;
		}
	}
	
}

double Arithmetic::getCompressionRate(const vector<unsigned char> & msg, vector<bitset<32>> * encoded_msg) {
	int beforeCompression = msg.size() * 8;
	int afterCompression = 0;
	int size = encoded_msg->size();
	double l, r;
	bitset<32> tagBinary;
	for (int i = 0; i < size; i++) {
		tagBinary = (*encoded_msg)[i];
		l = tagsLimits[tagBinary.to_string()].first;
		r = tagsLimits[tagBinary.to_string()].second;
		afterCompression += ceil( log2(1 / ((r - l) + 1e-18) ) ) + 1;
	}
	return (beforeCompression * 1.0) / afterCompression;
}
