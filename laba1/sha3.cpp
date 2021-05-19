#include "sha3.h"

sha3::sha3(int output_length) {

	capacity = output_length * 2; // cap = output * 2 xex
	rate = 1600 - output_length * 2;
	this->output_length = output_length;
	this->block_size = rate / 8;
}

uint64 sha3::ROTL(uint64 x, int shift) {
	return x << shift | x >> (sizeof(uint64) * 8 - shift);
}

uint64 sha3::ROTR(uint64 x, int shift) {
	return x >> shift | x << (sizeof(uint64) * 8 - shift);
}

void sha3::keccak_pi() {
	uint64 A1 = h[1];
	h[1]	=	h[6]; 
	h[6]	=	h[9]; 
	h[9]	=	h[22]; 
	h[22]	=	h[14];
	h[14]	=	h[20]; 
	h[20]	=	h[2]; 
	h[2]	=	h[12]; 
	h[12]	=	h[13];
	h[13]	=	h[19]; 
	h[19]	=	h[23]; 
	h[23]	=	h[15]; 
	h[15]	=	h[4];
	h[4]	=	h[24]; 
	h[24]	=	h[21]; 
	h[21]	=	h[8]; 
	h[8]	=	h[16];
	h[16]	=	h[5]; 
	h[5]	=	h[3]; 
	h[3]	=	h[18]; 
	h[18]	=	h[17];
	h[17]	=	h[11]; 
	h[11]	=	h[7]; 
	h[7]	=	h[10]; 
	h[10]	=	A1;
}

void sha3::keccak_theta() {
	uint64_t C[5], D[5];
	for (size_t i = 0; i < 5; ++i)
		C[i] = h[i] ^ h[i + 5] ^ h[i + 10] ^ h[i + 15] ^ h[i + 20];

	D[0] = ROTL(C[1], 1) ^ C[4];
	D[1] = ROTL(C[2], 1) ^ C[0];
	D[2] = ROTL(C[3], 1) ^ C[1];
	D[3] = ROTL(C[4], 1) ^ C[2];
	D[4] = ROTL(C[0], 1) ^ C[3];

	for (size_t i = 0; i < 5; ++i) {
		h[i] ^= D[i];
		h[i + 5] ^= D[i];
		h[i + 10] ^= D[i];
		h[i + 15] ^= D[i];
		h[i + 20] ^= D[i];
	}
}

void sha3::keccak_chi() {
	for (size_t i = 0; i < 25; i += 5) {
		uint64_t A0 = h[0 + i], A1 = h[1 + i];
		h[0 + i] ^= ~A1 & h[2 + i];
		h[1 + i] ^= ~h[2 + i] & h[3 + i];
		h[2 + i] ^= ~h[3 + i] & h[4 + i];
		h[3 + i] ^= ~h[4 + i] & A0;
		h[4 + i] ^= ~A0 & A1;
	}
}

void sha3::permutation() {
	constexpr uint64_t keccak_round_constants[24] = {
		0x0000000000000001, 0x0000000000008082, 0x800000000000808A,
		0x8000000080008000, 0x000000000000808B, 0x0000000080000001,
		0x8000000080008081, 0x8000000000008009, 0x000000000000008A,
		0x0000000000000088, 0x0000000080008009, 0x000000008000000A,
		0x000000008000808B, 0x800000000000008B, 0x8000000000008089,
		0x8000000000008003, 0x8000000000008002, 0x8000000000000080,
		0x000000000000800A, 0x800000008000000A, 0x8000000080008081,
		0x8000000000008080, 0x0000000080000001, 0x8000000080008008,
	};

	for (int round = 0; round < 24; round++) {
		keccak_theta();

		h[1] = ROTL(h[1], 1);
		h[2] = ROTL(h[2], 62);
		h[3] = ROTL(h[3], 28);
		h[4] = ROTL(h[4], 27);
		h[5] = ROTL(h[5], 36);
		h[6] = ROTL(h[6], 44);
		h[7] = ROTL(h[7], 6);
		h[8] = ROTL(h[8], 55);
		h[9] = ROTL(h[9], 20);
		h[10] = ROTL(h[10], 3);
		h[11] = ROTL(h[11], 10);
		h[12] = ROTL(h[12], 43);
		h[13] = ROTL(h[13], 25);
		h[14] = ROTL(h[14], 39);
		h[15] = ROTL(h[15], 41);
		h[16] = ROTL(h[16], 45);
		h[17] = ROTL(h[17], 15);
		h[18] = ROTL(h[18], 21);
		h[19] = ROTL(h[19], 8);
		h[20] = ROTL(h[20], 18);
		h[21] = ROTL(h[21], 2);
		h[22] = ROTL(h[22], 61);
		h[23] = ROTL(h[23], 56);
		h[24] = ROTL(h[24], 14);

		keccak_pi();
		keccak_chi();

		h[0] ^= keccak_round_constants[round];
	}
}

void sha3::iteration(string input) {
	for (size_t i = 0; i < rate / 64; ++i)
		h[i] ^= uint64(input[8 * i + 0]) << 0 |
		uint64(input[8 * i + 1]) << 8 |
		uint64(input[8 * i + 2]) << 16 |
		uint64(input[8 * i + 3]) << 24 |
		uint64(input[8 * i + 4]) << 32 |
		uint64(input[8 * i + 5]) << 40 |
		uint64(input[8 * i + 6]) << 48 |
		uint64(input[8 * i + 7]) << 56;
	permutation();
}

string sha3::convertToCharArray(int len, v64 a){
	string arr(len, '\0');
	for (int j = 0; j < len / 8; j++)
		for (int i = 0; i < 8; i++)
			arr[j * 8 + i] = (unsigned char)((a[j] >> i * 8) & 0xFF);
	static const char hex_digits[] = "0123456789ABCDEF";

	std::string output;
	output.reserve(arr.length() * 2);
	for (unsigned char c : arr) {
		output.push_back(hex_digits[c >> 4]);
		output.push_back(hex_digits[c & 15]);
	}
	return output;
}

string sha3::printHash(string input) {
	this->data = input;
	this->h = v64(25, 0);

	for (size_t i = 0; i < data.size() / block_size; i++)
		iteration(data.substr(i * block_size, block_size));

	int len = data.size() / block_size;
	string buffer = data.substr(len, data.size() % block_size);
	buffer.resize(block_size, 0);
	buffer[len] |= 0x06;
	buffer[block_size - 1] |= 0x80;

	iteration(buffer);
	
	string hash = convertToCharArray(output_length / 8, h);
	return hash;
}

