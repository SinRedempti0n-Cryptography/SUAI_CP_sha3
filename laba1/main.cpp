#include<iostream>
#include<string>
#include <math.h>
#include "keccak.h"
using namespace std;
typedef vector<char> vc;

int main() {

	string str = "";
	Keccak kcck(512);
	//cout << kcck.bits_to_string("01110000");





	//cout << str << "\n";
	cout << kcck.run_algorithm(str) << "\n";
	



	return 0;
}