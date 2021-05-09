#include <string>
#include <bitset>
#include "keccak.h"
#include <iostream>
using namespace std;



Keccak::Keccak(int output_length) {

	this->output_length = output_length;
	 
	if (output_length == 224) {
		rate = 1152;
		capacity = 448;
	} 
	else if (output_length == 256) {
		rate = 1088;
		capacity = 512;
	}
	else if (output_length == 384) {
		rate = 832;
		capacity = 768;
	}
	else if (output_length == 512) {
		rate = 576;
		capacity = 1024;
	}
}

bitset<8> Keccak::to_bits(unsigned char byte)
{
	return bitset<8>(byte);
}

string Keccak::to_char(string byte)
{
	bitset<8> b(byte, 0, 8, '0', '1');
	int a = b.to_ullong();
	string res = "";
	res += char(a);
	return res;
}


vvc Keccak::pad_function(string str) {
	
	for (int i = 0; i < str.size(); i++) 
		this->data.push_back(to_bits(str[i]));

	int count_i = 0;
	int count_j = 0;
	vc chars(rate);

	blocks.push_back(chars);
	for (int i = 0; i < data.size(); i++) {
		for (int j = 0; j < 8; j++) {
			if (data[i].test(j) == true) {
				blocks[count_i][count_j] = '1';
			}
			else {
				blocks[count_i][count_j] = '0';
			}
			
			count_j++;
			if (count_j == rate) {
				count_j = 0;
				count_i++;
				blocks.push_back(chars);
			}
		}
	}

	if ((str.size() * 8) % rate != 0) {
		if ((str.size() * 8) % rate == 1) {
			this->n = int((str.size() * 8) / rate) + 2;
			blocks[count_i][count_j] = '1';
			count_i++;
			blocks.push_back(chars);
			for (int j = 0; j < rate - 1; j++) {
				blocks[count_i][j] = '0';
			}
			blocks[count_i][rate-1] = '1';
		}
		else {
			this->n = int((str.size() * 8) / rate) + 1;
			blocks[count_i][count_j] = '1';
			count_j++;
			for (int j = count_j; j < rate - 1; j++) {
				blocks[count_i][j] = '0';
			}
			blocks[count_i][rate-1] = '1';
		}
	}
	else {
		this->n = int((str.size() * 8) / rate) + 1;
		blocks[count_i][0] = '1';
		for (int j = 1; j < rate - 1; j++) {
			blocks[count_i][j] = '0';
		}
		blocks[count_i][rate-1] = '1';
	}

	return blocks;
}

string Keccak::run_algorithm(string str) {

	pad_function(str);
	int size_i = blocks.size();
	int size_j = blocks[0].size();

	for (int i = 0; i < size_i; i++) {
		for (int j = size_j; j < b; j++) {
			blocks[i].push_back('0');
		}
	}
	vc S;
	for (int i = 0; i < b; i++) {
		S.push_back('0');
	}
	
	for (int i = 0; i < size_i; i++) {
		//cout << "BLOCK: " << i << "\n";
		for (int j = 0; j < b; j++) {
			blocks[i][j] = chars_xor(blocks[i][j], S[j]);
		}
		S = permutation_function(blocks[i]);
	}

	vc Z;

	while (Z.size() < output_length) {
		for (int i = 0; i < rate; i++) {
			Z.push_back(S[i]);
		}
		S = permutation_function(S);
	}

	string res_z = "";

	for (int i = 0; i < output_length; i++) {
		res_z += Z[i];
	}



	string res = bits_to_string(res_z);
	return res;
}

vc Keccak::permutation_function(vc S) {
	
	vvvc A(5);
	for (auto& a : A) {
		vvc tmp(5);
		a = tmp;
		for (auto& a2 : a) {
			vc tmp2;
			for (int k = 0; k < w; k++) {
				tmp2.push_back('0');
			}
			a2 = tmp2;
		}
	}

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			for (int k = 0; k < w; k++) {
				A[i][j][k] = S[(5 * i + j) * w + k];
			}
		}
	}

	vvvc A2;

	for (int i = 0; i < n_r; i++) {
		A2 = Rnd(A, i);
	}

	vc S2(b);

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			for (int k = 0; k < w; k++) {
				S2[(5 * i + j) * w + k] = A2[i][j][k]; 
			}
		}
	}

	return S2;
}

string Keccak::bits_to_string(string str) {

	string result = "";
	string tmp = "";
	int count = 0;
	for (int i = 0; i < str.length(); i++) {
		tmp += str[i];
		count++;
		if (count == 8) {
			result += to_char(str);
			tmp = "";
			count = 0;
		}
	}

	return result;
}

char Keccak::chars_xor(char c1, char c2) {
	if (c1 == '1' && c2 == '0' || c1 == '0' && c2 == '1') {
		return '1';
	}
	else {
		return '0';
	}
}

char Keccak::chars_and(char c1, char c2) {
	if (c1 == '1' && c2 == '1') {
		return '1';
	}
	else {
		return '0';
	}
}

int Keccak::mod(int a, int b) {

	int res = a;

	if (a >= 0) {
		while (res >= b) {
			res = res - b;
		}
	}
	else {
		while (res < 0) {
			res = res + b;
		}
	}

	return res;
}

vvvc Keccak::step_1(vvvc A) {
	vvvc A2(5);
	for (auto& a : A2) {
		vvc tmp(5);
		a = tmp;
		for (auto& a2 : a) {
			vc tmp2;
			for (int k = 0; k < w; k++) {
				tmp2.push_back('0');
			}
			a2 = tmp2;
		}
	}
	
	vc tmp;
	for (int k = 0; k < w; k++) {
		tmp.push_back('0');
	}
	vvc C(5);
	for (auto& a : C) {
		a = tmp;
	}
	
	for (int i = 0; i < 5; i++) {
		for (int k = 0; k < w; k++) {
			for (int j = 0; j < 5; j++) {
				C[i][k] = chars_xor(C[i][k], A[i][j][k]);
			}
		}
	}

	vvc D(5);
	for (auto& a : D) {
		a = tmp;
	}
	
	for (int i = 0; i < 5; i++) {
		for (int k = 0; k < w; k++) {
			D[i][k] = chars_xor(C[mod(i - 1, 5)][k], C[mod(i + 1, 5)][mod(k - 1, w)]);
		}
	}
	
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			for (int k = 0; k < w; k++) {
				A2[i][j][k] = chars_xor(A[i][j][k], D[i][k]);
			}
		}
	}
	return A2;
}

vvvc Keccak::step_2(vvvc A) {
	vvvc A2(5);
	for (auto& a : A2) {
		vvc tmp(5);
		a = tmp;
		for (auto& a2 : a) {
			vc tmp2;
			for (int k = 0; k < w; k++) {
				tmp2.push_back('0');
			}
			a2 = tmp2;
		}
	}

	for (int k = 0; k < w; k++) {
		A2[0][0][k] = A[0][0][k];
	}

	int i = 1;
	int j = 0;

	for (int t = 0; t < 23; t++) {
		for (int k = 0; k < w; k++) {
			A2[i][j][k] = A[i][j][mod(k - int((t+1)*(t+2)/2), w)];
		}
		int tmp = i;
		i = j;
		j = mod(2 * tmp + 3 * j, 5);
	}
	return A2;
}

vvvc Keccak::step_3(vvvc A) {
	vvvc A2(5);
	for (auto& a : A2) {
		vvc tmp(5);
		a = tmp;
		for (auto& a2 : a) {
			vc tmp2;
			for (int k = 0; k < w; k++) {
				tmp2.push_back('0');
			}
			a2 = tmp2;
		}
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			for (int k = 0; k < w; k++) {
				A2[i][j][k] = A[mod(i + 3 * j, 5)][i][k];
			}
		}
	}

	return A2;
}

vvvc Keccak::step_4(vvvc A) {
	vvvc A2(5);
	for (auto& a : A2) {
		vvc tmp(5);
		a = tmp;
		for (auto& a2 : a) {
			vc tmp2;
			for (int k = 0; k < w; k++) {
				tmp2.push_back('0');
			}
			a2 = tmp2;
		}
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			for (int k = 0; k < w; k++) {
				A2[i][j][k] = chars_xor(A[i][j][k],
					chars_and(chars_xor(A[mod(i + 1, 5)][j][k], '1'),
						A[mod(i + 2, 5)][j][k]));
					A[mod(i + 3 * j, 5)][i][k];
			}
		}
	}

	return A2;
}

vvvc Keccak::step_5(vvvc A, int i_r) {
	vvvc A2(5);
	for (auto& a : A2) {
		vvc tmp(5);
		a = tmp;
		for (auto& a2 : a) {
			vc tmp2;
			for (int k = 0; k < w; k++) {
				tmp2.push_back('0');
			}
			a2 = tmp2;
		}
	}

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			for (int k = 0; k < w; k++) {
				A2[i][j][k] = A[i][j][k];
			}
		}
	}

	vc RC;
	for (int k = 0; k < w; k++) {
		RC.push_back('0');
	}

	for (int i = 0; i < 6; i++) {
		RC[int(pow(2, i)) - 1] = rc(i + 7 * i_r);
	}

	for (int k = 0; k < w; k++) {
		A2[0][0][k] = chars_xor(A[0][0][k], RC[k]);
	}

	return A2;
}

char Keccak::rc(int t) {

	int m = mod(t, 255);

	if (m == 0) {
		return '1';
	}

	string R = "10000000";

	for (int i = 0; i < m; i++) {
		R = "0" + R;
		R[0] = chars_xor(R[0], R[8]);
		R[4] = chars_xor(R[4], R[8]);
		R[5] = chars_xor(R[5], R[8]);
		R[6] = chars_xor(R[6], R[8]);
		R = string(R, 0, 8);
	}

	return R[0];
}

vvvc Keccak::Rnd(vvvc A, int i_r) {
	return step_5(step_4(step_3(step_2(step_1(A)))), i_r);
}


