#include "lz77.h"
#include <fstream>
#include <iostream>
#include <string>
#include <set>

//this function returns number of alphabet
int readDecoded(vector<unsigned char> & decoded_msg, string& format, int &xres, int &yres, int &maxintensity);
void readEncoded(vector<Triplet> & encoded_msg);
void printDefaults(string format, int xres, int yres, int maxintensity, ofstream & out);
void printEncoded(vector<Triplet>& encoded_msg);
void printDecoded(vector<unsigned char> &encoded_msg, int xres, int yres, ofstream & out);

enum funcEnum {
	encode,
	decode
};

int main(int argc, char* argv[]) {
	string functionCalled;

	functionCalled = argv[1];

	funcEnum function;
	if (functionCalled == "-encode") function = encode;
	else if (functionCalled == "-decode") function = decode;

	ofstream outFile;

	LZ77 lz77Obj;
	string format = "P2";
	int m, xres, yres, maxIntensity, S, T, A, before, after;

	vector<Triplet> encoded_msg;
	vector<unsigned char> decoded_msg;
	
	switch (function) {
	case encode:
		S = stoi(argv[2]);
		T = stoi(argv[3]);
		A = stoi(argv[4]);

		A = readDecoded(decoded_msg, format, xres, yres, maxIntensity);
		
		lz77Obj.encode(decoded_msg, S, T, A, &encoded_msg);
		cout << "Encoded msg: " << endl;
		printEncoded(encoded_msg);
		break;

	case decode:
		outFile = ofstream("output.pgm");
		xres = stoi(argv[2]);
		yres = stoi(argv[3]);
		maxIntensity = stoi(argv[4]);

		readEncoded(encoded_msg);
		
		lz77Obj.decode(encoded_msg, &decoded_msg);
		printDefaults(format, xres, yres, maxIntensity, outFile);
		printDecoded(decoded_msg, xres, yres, outFile);
		break;

	default:
		break;
	}

	return 0;
}

int readDecoded(vector<unsigned char> & decoded_msg, string& format, int &xres, int &yres, int &maxintensity) {
	cin >> format;
	cin >> xres >> yres >> maxintensity;

	set<unsigned char> uniques;
	int temp;
	int imageSize = xres * yres;
	for (int i = 0; i < imageSize; i++) {
		cin >> temp;
		decoded_msg.push_back(temp);
		uniques.insert(temp);
	}
	return uniques.size();
}

void readEncoded(vector<Triplet> & encoded_msg) {
	int encodedSize;
	cin >> encodedSize;

	Triplet trip;
	int temp;

	for (int i = 0; i < encodedSize; i++) {
		for (int j = 0; j < 3; j++) {
			cin >> temp;
			if (j == 0)
				trip.j = temp;
			else if (j == 1)
				trip.k = temp;
			else
				trip.c = temp;
		}
		encoded_msg.push_back(trip);
	}
}

void printDefaults(string format, int xres, int yres, int maxintensity, ofstream & out) {
	out << format << endl;
	out << xres << " " << yres << endl;
	out << maxintensity << endl;
}

void printEncoded(vector<Triplet>& encoded_msg) {
	int encodedSize = encoded_msg.size();
	cout << encodedSize << endl;
	for (int i = 0; i < encodedSize; i++)
		cout << encoded_msg[i].j << " " << encoded_msg[i].k << " " << (int)encoded_msg[i].c << endl;
	cout << endl;
}

void printDecoded(vector<unsigned char> &encoded_msg, int xres, int yres, ofstream & out) {
	int decoded_msg_size = encoded_msg.size();
	
	for (int i = 0; i < yres; i++) {
		for (int j = 0; j < xres; j++)
			out << (int)encoded_msg[i*xres+j] << " ";
		out << endl;
	}
	out << endl;
}
