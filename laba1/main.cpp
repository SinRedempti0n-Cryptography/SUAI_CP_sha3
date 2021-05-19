#include<iostream>
#include<string>
#include <math.h>
#include "sha3.h"



int main() {
    string message = "";
    sha3 sha(512);
    sha.printHash(message);
}