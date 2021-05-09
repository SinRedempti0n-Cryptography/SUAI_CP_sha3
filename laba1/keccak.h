#pragma once
//#include "hash.h" 
#include <string>
#include <vector> 
#include <bitset>
typedef unsigned __int8  uint8_t; 
typedef unsigned __int64 uint64_t; 


#include <stdint.h> 
using namespace std;

typedef vector<bitset<8>> vb;
typedef vector<char> vc;
typedef vector<vector<char>> vvc;
typedef vector<vector<vector<char>>> vvvc;



class Keccak
{
private:
    int output_length;
    int rate;
    int capacity;
    int n;

    vb data;
    vvc blocks;
    
    const int b = 1600;
    const int n_r = 24;
    const int l = 6;
    const int w = 64;

public:
    Keccak(int output_length);
    bitset<8> to_bits(unsigned char byte);
    string to_char(string byte);
    string bits_to_string(string string);
    char chars_xor(char c1, char c2);
    char chars_and(char c1, char c2);
    int mod(int a, int b);
    string run_algorithm(string str);
    vvc pad_function(string str);
    vc permutation_function(vc S);
    vvvc step_1(vvvc A);
    vvvc step_2(vvvc A);
    vvvc step_3(vvvc A);
    vvvc step_4(vvvc A);
    vvvc step_5(vvvc A, int i_r);
    vvvc Rnd(vvvc A, int i_r);
    char rc(int t);
 
};
