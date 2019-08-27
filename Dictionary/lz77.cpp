#include "lz77.h"
#include <cmath>

//---------------CONSTRUCTOR & DESTRUCTOR---------------//
LZ77::LZ77() {

}

LZ77::~LZ77() {

}

//---------------PUBLIC FUNCTIONS---------------//

int LZ77::encode(vector<unsigned char>& msg, int S, int T, int A, vector<Triplet>* encoded_msg)
{
	int searchRetreat, advanceCount;
	Triplet longestTrip;
	vector<unsigned char>::iterator searchPtr = msg.begin(), originalSearchPtr;
	vector<unsigned char>::iterator lookPtr = msg.begin(), originalLookPtr, tempLook;
	Triplet tempTrip;
	//first triplet
	tempTrip.j = 0;
	tempTrip.k = 0;
	tempTrip.c = *lookPtr;
	encoded_msg->push_back(tempTrip);

	lookPtr++;

	while (lookPtr != msg.end()) {
		searchRetreat = 1; advanceCount = 0;

		//start with <c, 0, 0>
		tempTrip.j = 0;
		tempTrip.k = advanceCount;
		tempTrip.c = *lookPtr;
		//longestTrip is the temp for now
		longestTrip = tempTrip;

		//set orignal ptr's
		originalSearchPtr = searchPtr;
		originalLookPtr = tempLook = lookPtr;

		//I can't exceed S and I can't get out of the vector
		while (searchRetreat <= S && lookPtr != msg.end()) {
			if (*lookPtr == *searchPtr && advanceCount != T) {
				//advance
				lookPtr++; searchPtr++;
				//increment advances counts
				advanceCount++;
			}
			else {
				if (advanceCount > 0)
					tempTrip.j = searchRetreat;
				else
					tempTrip.j = 0;
				tempTrip.k = advanceCount;
				tempTrip.c = *lookPtr;
				//check if it's longer than longestTriplet
				if (advanceCount > longestTrip.k) {
					longestTrip = tempTrip;
					tempLook = lookPtr;
				}
				//retreat search pointer by one and recheck
				lookPtr = originalLookPtr;
				searchPtr = originalSearchPtr;
				if (searchPtr != msg.begin())
					searchPtr--;
				else
					break;
				originalSearchPtr = searchPtr;

				searchRetreat++;
				advanceCount = 0;
			}
		}
		//last location
		lookPtr = tempLook;
		searchPtr = lookPtr;
		//advance
		lookPtr++;
		//push longest triplet
		encoded_msg->push_back(longestTrip);
	}

	return (ceil(log2(S)) + ceil(log2(S) + T) + ceil(log2(A))) * encoded_msg->size();
}

void LZ77::decode(const vector<Triplet>& encoded_msg, vector<unsigned char>* decoded_msg)
{
	Triplet trip;
	int encoded_size = encoded_msg.size();
	//add first element
	
	int search = 0, lookAhead = 0;
	

	for (int i = 0; i < encoded_size; i++) {
		trip = encoded_msg[i];
		int match = trip.k;
		int offset = trip.j;

		search = lookAhead;
		search -= offset;

		int counter = 0;

		while (counter < match) {
			lookAhead++;
			counter++;
			decoded_msg->push_back((*decoded_msg)[search++]);
		}

		lookAhead++;
		decoded_msg->push_back(trip.c);
	}
}


void LZ77::decorrelate(vector<unsigned char>& msg, vector<unsigned char>& difference_msg) const {
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