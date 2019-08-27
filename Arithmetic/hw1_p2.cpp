#include "arithmetic.h"
#include <fstream>
#include <istream>
#include <iostream>
#include <string>
#include <sstream>
#include <bitset>

enum funcEnum {
	bin_to_dec,
	dec_to_bin,
	encode,
	decode
};

void readDecoded(vector<unsigned char> & msg, string& format, int &xres, int &yres, int &maxintensity);
void readEncoded(vector<bitset<32>> & msg);
void readLimits(vector<pair<unsigned char, pair<double, double>>> &charLimits);
void printDefaults(string format, int xres, int yres, int maxintensity, ofstream & out);
void printEncoded(vector<bitset<32>> &msg, ofstream & out);
void printDecoded(vector<unsigned char> &msg, int xres, int yres, ofstream & out);
void printCharLimits(vector<pair<unsigned char, pair<double, double>>> charLimits, ofstream & out);

int main(int argc, char* argv[]) {
	string functionCalled;
	stringstream arg;

	functionCalled = argv[1];


	funcEnum function;
	if (functionCalled == "-bin_to_dec") function = bin_to_dec;
	else if (functionCalled == "-dec_to_bin") function = dec_to_bin;
	else if (functionCalled == "-encode") function = encode;
	else if (functionCalled == "-decode") function = decode;

	ofstream outFile;
	if (function == encode)
		outFile = ofstream("image.enc");
	else if (function == decode)
		outFile = ofstream("output.pgm");

	Arithmetic arithmeticObj;
	string format = "P2", hexaString;
	int m, xres, yres, maxintensity, encodedSize;

	vector<pair<unsigned char, pair<double, double>>> limits;
	vector<bitset<32>> encoded_msg;
	vector<unsigned char> decoded_msg;
	unsigned int binaryInt;
	bitset<32> *binary;
	double decimal;

	switch (function) {
	case bin_to_dec:
		hexaString = argv[2];
		arg << hexaString;
		arg >> hex >> binaryInt;
		binary = new bitset<32>(binaryInt);
		cout << *binary << " is " << arithmeticObj.binary_to_decimal(*binary) << " in decimal." << endl;
		delete binary;
		break;

	case dec_to_bin:
		decimal = stod(argv[2]);
		
		cout << decimal << " is " << arithmeticObj.decimal_to_binary(decimal) << " in binary." << endl;
		break;

	case encode:
		m = stoi(argv[2]);
		
		readDecoded(decoded_msg, format, xres, yres, maxintensity);
		
		arithmeticObj.encode(decoded_msg, m, &encoded_msg);
		
		limits = arithmeticObj.getCharLimits();

		printEncoded(encoded_msg, outFile);

		printCharLimits(limits, outFile);
		
		break;

	case decode:
		m = stoi(argv[2]);
		xres = stoi(argv[3]);
		yres = stoi(argv[4]);
		maxintensity = stoi(argv[5]);

		readEncoded(encoded_msg);

		readLimits(limits);		
		
		arithmeticObj.setMsgSize(xres * yres);
		arithmeticObj.setCharLimits(limits);
		arithmeticObj.decode(encoded_msg, m, &decoded_msg);

		printDefaults(format, xres, yres, maxintensity, outFile);
		printDecoded(decoded_msg, xres, yres, outFile);
		break;

	default:
		break;
	}
	
	return 0;
}

void readDecoded(vector<unsigned char> & msg, string& format, int &xres, int &yres, int &maxintensity) {
	cin >> format;
	cin >> xres >> yres >> maxintensity;

	int temp;
	int imageSize = xres * yres;
	for (int i = 0; i < imageSize; i++) {
		cin >> temp;
		msg.push_back(temp);
	}
}

void readEncoded(vector<bitset<32>> & msg) {
	int encodedSize;
	cin >> encodedSize;
	
	string s;
	
	for (int i = 0; i < encodedSize; i++) {
		cin >> s;
		bitset<32> b (s);
		msg.push_back(b);
	}
}

void readLimits(vector<pair<unsigned char, pair<double, double>>> &charLimits) {
	
	double c;
	int i = 0;

	pair<unsigned char, pair<double, double>> p;
	
	while (cin >> c) {
		if (i == 0) 
			p.first = (int)c;
		else if (i == 1)
			p.second.first = c;
		else {
			i = -1;
			p.second.second = c;
			charLimits.push_back(p);
		}
		i++;
	}
}

void printDefaults(string format, int xres, int yres, int maxintensity, ofstream & out) {
	out << format << endl;
	out << xres << " " << yres << endl;
	out << maxintensity << endl;
}

void printEncoded(vector<bitset<32>> &msg, ofstream & out) {
	int encoded_msg_size = msg.size();
	out << encoded_msg_size << endl;
	cout << encoded_msg_size << endl;
	for (int i = 0; i < encoded_msg_size; i++) {
		out << msg[i] << " ";
		cout << msg[i] << " ";
	}
	out << endl;
	cout << endl;
}

void printDecoded(vector<unsigned char> &msg, int xres, int yres, ofstream & out) {
	int decoded_msg_size = msg.size();

	for (int i = 0; i < yres; i++) {
		for (int j = 0; j < xres; j++)
			out << (int)msg[i*xres + j] << " ";
		out << endl;
	}
	out << endl;
}

void printCharLimits(vector<pair<unsigned char, pair<double, double>>> charLimits, ofstream & out) {
	out << endl;
	cout << endl;
	vector<pair<unsigned char, pair<double, double>>>::iterator it = charLimits.begin();
	for (; it != charLimits.end(); it++) {
		out << (int)(*it).first << " " << (*it).second.first << " " << (*it).second.second << endl;
		cout << (int)(*it).first << " " << (*it).second.first << " " << (*it).second.second << endl;
	}
}