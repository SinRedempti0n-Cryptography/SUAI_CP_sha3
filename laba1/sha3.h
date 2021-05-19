#pragma once
#include <string>
#include <vector>

using namespace std;
typedef unsigned long long uint64;
typedef unsigned char uint8;

typedef vector<uint64> v64;

class sha3{
private:
    string  data;
    uint64  length;
    v64     h;
    int     output_length, rate, capacity, block_size;

    string  convertToCharArray(int, v64);
    void    iteration(string);
    void    permutation();
    void    keccak_theta();
    void    keccak_pi();
    void    keccak_chi();
    uint64  ROTL(uint64, int);
    uint64  ROTR(uint64, int);

public:
    sha3(int);
    void printHash(string);
};